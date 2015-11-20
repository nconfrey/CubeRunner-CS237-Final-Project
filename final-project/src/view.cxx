/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include "plane.hxx"
#include "map-cell.hxx"
#include "buffer-cache.hxx"
#include "texture-cache.hxx"

static void Error (int err, const char *msg);
static GLFWwindow *InitGLFW (int wid, int ht, const char *title);

// animation time step (100Hz)
#define TIME_STEP	0.001

#define YELLOW_COLOR  cs237::color4f(1.0f, 1.0f, 0.0f, 1.0f);    /*!< Yellow color */

static void SetViewport (GLFWwindow *win)
{
    int fbWid, fbHt;
    glfwGetFramebufferSize (win, &fbWid, &fbHt);
    glViewport(0, 0 , fbWid, fbHt);

}

/* The vertices for the cube */ 
static cs237::vec3f cubeVertices[8]= {cs237::vec3f(-1.0f,  -1.0f,  1.0f), //0
                   cs237::vec3f (-1.0f,  1.0f,  1.0f), //1
                   cs237::vec3f ( 1.0f,  1.0f,  1.0f), //2
                   cs237::vec3f( 1.0f,  -1.0f,  1.0f), //3
                   cs237::vec3f (-1.0f,  -1.0f, -1.0f), //4
                   cs237::vec3f (-1.0f,  1.0f, -1.0f), //5
                   cs237::vec3f ( 1.0f,  1.0f, -1.0f), //6
                   cs237::vec3f ( 1.0f,  -1.0f, -1.0f)}; //7 

/* the indices that allow us to create the cube. */ 
static const uint16_t cubeIndices[36] = {
    0,2,1,  0,3,2,
    4,3,0,  4,7,3,
    4,1,5,  4,0,1,
    3,6,2,  3,7,6,
    1,6,5,  1,2,6,
    7,5,6,  7,4,5
  };  

static const cs237::color4f cubeColor = YELLOW_COLOR; 


/***** class View member functions *****/

View::View (Map *map)
    : _map(map), _errorLimit(2.0), _isVis(true), _window(nullptr), _wireframe(true),
      _bCache(new BufferCache()), _tCache(new TextureCache())
{
}

void View::Init (int wid, int ht)
{

    this->_window = InitGLFW(wid, ht, this->_map->Name().c_str());

  // attach the view to the window so we can get it from callbacks
    glfwSetWindowUserPointer (this->_window, this);

  // Compute the bounding box for the entire map
    this->_mapBBox = cs237::AABBd(
	cs237::vec3d(0.0, double(this->_map->MinElevation()), 0.0),
	cs237::vec3d(
	    double(this->_map->hScale()) * double(this->_map->Width()),
	    double(this->_map->MaxElevation()),
	    double(this->_map->hScale()) * double(this->_map->Height())));

  // Place the viewer in the center of cell(0,0), just above the
  // cell's bounding box.
    cs237::AABBd bb = this->_map->Cell(0,0)->Tile(0).BBox();
    cs237::vec3d pos = bb.center();
    pos.y = bb.maxY() + 0.01 * (bb.maxX() - bb.minX());

  // The camera's direction is toward the bulk of the terrain
    cs237::vec3d  at;
    if ((this->_map->nRows() == 1) && (this->_map->nCols() == 1)) {
	at = pos + cs237::vec3d(1.0, -0.25, 1.0);
    }
    else {
	at = pos + cs237::vec3d(double(this->_map->nCols()-1), 0.0, double(this->_map->nRows()-1));
    }

    this->_cam.move(pos, at, cs237::vec3d(0.0, 1.0, 0.0));

  // set the FOV and near/far planes
    this->_cam.setFOV (60.0);
    this->_cam.setNearFar (10.0, 1.5 * double(this->_map->CellWidth()) * double(this->_map->hScale()));
    this->Resize (wid, ht);

  // initialize shaders
    /* YOUR CODE HERE */
    this->InitRenderers();

    /* ADDITIONAL INITIALIZATION */
    this->projectionMat = this->Camera().projTransform();
    this->UpdateModelViewMat ();

  // initialize animation state
    this->_lastStep =
    this->_lastFrameTime = glfwGetTime();

}

#define SIN_ONE_DEGREE	0.0174524064373f
#define COS_ONE_DEGREE	0.999847695156f
#define SQRT_2 1.41421356237f

void View::HandleKey (int key, int scancode, int action, int mods)
{

    switch (key) {
      case GLFW_KEY_ESCAPE:
      case GLFW_KEY_Q:
	if (mods == 0)
	    glfwSetWindowShouldClose (this->_window, true);
	break;
      case GLFW_KEY_W: // toggle wireframe mode
	this->_wireframe = !this->_wireframe;
	break;
      case GLFW_KEY_EQUAL:
	if (mods == GLFW_MOD_SHIFT) { // shift+'=' is '+'
	  // decrease error tolerance
	    if (this->_errorLimit > 0.5)
		this->_errorLimit /= SQRT_2;
	}
	break;
      case GLFW_KEY_KP_ADD:  // keypad '+'
	if (mods == 0) {
	  // decrease error tolerance
	    if (this->_errorLimit > 0.5)
		this->_errorLimit /= SQRT_2;
	}
	break;
      case GLFW_KEY_MINUS:
	if (mods == 0) {
	  // increase error tolerance
	    this->_errorLimit *= SQRT_2;
	}
	break;
      case GLFW_KEY_KP_SUBTRACT:  // keypad '-'
	if (mods == 0) {
	  // increase error tolerance
	    this->_errorLimit *= SQRT_2;
	}
	break;
      case GLFW_KEY_LEFT:
      this->_cam.move(this->Camera().position()+cs237::vec3d(-10, 0.0, 0.0));
      break;
      case GLFW_KEY_RIGHT:
      this->_cam.move(this->Camera().position()+cs237::vec3d(10, 0.0, 0.0));
      break;
      case GLFW_KEY_UP:
      this->_cam.move(this->Camera().position()-cs237::vec3d(0.0, 0.0, 10));
      break;
      case GLFW_KEY_DOWN:
      this->_cam.move(this->Camera().position()-cs237::vec3d(0.0, 0.0, -10));
      break;
      default: // ignore all other keys
	return;
    }

//std::cout << "after handleKey: " << this->_cam << std::endl;

}

void View::HandleMouseEnter (bool entered)
{
}

void View::HandleMouseMove (double x, double y)
{
}

void View::HandleMouseButton (int button, int action, int mods)
{
}

void View::Animate ()
{
    double now = glfwGetTime();
    double dt = now - this->_lastStep;
    if (dt >= TIME_STEP) {
	this->_lastStep = now;
	/* ANIMATE STUFF */
    }

}

void View::Resize (int wid, int ht)
{
    glfwGetFramebufferSize (this->_window, &this->_fbWid, &this->_fbHt);
    glViewport(0, 0 , this->_fbWid, this->_fbHt);

  // default error limit is 1%
    this->_errorLimit = float(this->_fbHt) / 100.0f;

    this->_cam.setViewport (this->_fbWid, this->_fbHt);

}

/* InitRenderers:
 */
void View::InitRenderers ()
{
    this->wfRender = new WireframeRenderer();
    //this->fRender = new ForwardRenderer();
}

void View::UpdateModelViewMat ()
{
  this->modelViewMat = this->Camera().ModelViewMatrix();
}

/* main render loop */

void View::Render ()
{
    if (! this->_isVis)
    return;

    double now = glfwGetTime();
    float dt = float(now - this->_lastFrameTime);
    this->_lastFrameTime = now;

    //update model view mat
    this->UpdateModelViewMat();

    //clear the screen
    glClearColor (0.2f, 0.2f, 0.4f, 1.0f);  // clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //choose renderer
    Renderer *r;
    if(this->_wireframe)
      r = this->wfRender;
    else
      r = this->wfRender; //eventually this will be the other renderer

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    r->Enable(this->projectionMat);


    //print camera
    //printf("%f, %f, %f\n", this->_cam.position ()[0],this->_cam.position ()[1],this->_cam.position ()[2]);
    //printf("%f, %f, %f\n", this->_cam.direction ()[0],this->_cam.direction()[1],this->_cam.direction()[2]);
    //printf("%f, %f, %f\n", this->_cam.up ()[0],this->_cam.up ()[1],this->_cam.up ()[2]);

    //TESTCUBE
    /*Mesh *cube = new Mesh(GL_TRIANGLES);
    cube->LoadVertices(8, cubeVertices);
    cube->LoadIndices(36, cubeIndices);
    cube->SetColor(cs237::color3f(0.0f, 0.85f, 0.313f));
    cube->SetToWorldMatrix(cs237::translate(cs237::vec3f(0.0f,0.0f,0.0f)));
    r->Render(this->modelViewMat, cube, 0);*/

    //loop through all cells in map
    for(int row = 0; row < this->_map->nRows(); row++){
      for(int col = 0; col < this->_map->nCols(); col++){
        //get the cell from the map
        Cell *c = this->_map->Cell(row, col);
        //get the tile from the cell
        Tile *t = &(c->Tile(0)); //this is the root of the quad tree
        struct Chunk chu = t->Chunk();
        for(int ci = 0; ci < 100; ci++){
          //printf("x=%d", chu._vertices[ci]._x);
        }

        //here we check if it is in the view frustrum

        //call function to render at appropriate level of detail
        Recursive_Render_Chunk(t, r);

      }
    }

    glfwSwapBuffers (this->_window);

}

bool View::inFrustum(Tile *t)
{
  cs237::AABBd boundingBox = t->BBox();
  Plane *frust = new Plane();
  Plane **frustum = frust->extractPlanes(this->Camera().ModelViewMatrix());
  int totalIn = 0;

  //Test all 8 corners of the bounding box with the 6 planes of the view frustum
  for(int frustPlane = 0; frustPlane < 6; frustPlane++)
  {
    //counting how many corners are in the planes
    int inCount = 8;
    int somewhatIn = 1;
    for(int i = 0; i < 8; i++)
    {
      //test this corner against all of the planes
      cs237::vec3d c = boundingBox.corner(i);
      if(frustum[frustPlane]->ClassifyPoint(c) == Plane::OUTSIDE)
      {
        somewhatIn = 0;
        inCount--;
      }
    }

    if(inCount == 0)
    {
      //all the corners of the bounding box were outside of the planes
      printf("All of the corners are outside the planes for face [%d]\n", frustPlane);
      return false;
    }
    totalIn += somewhatIn;
  }

  printf("total corners in is: %d", totalIn);
  if(totalIn == 6)
    return true; // all of the corners are inside the view

  //otherwise, we are partially in which we will consider as an in for now
  //will come back to this later when optimizing

  printf("base case return\n");
  return true;
}

float View::SSE(Tile *t)
{
  double dist = t->BBox().distanceToPt(this->_cam.position());
  return this->_cam.screenError(dist, t->Chunk()._maxError);
}

void View::Recursive_Render_Chunk(Tile *t, Renderer *r)
{
  //check to see if this tile is in the view frustum to save time
  if(!inFrustum(t))
    return;


  //here we also have to get texture and rnomal information from the respective tqt's

  //calculate SSE
  float sse = this->SSE(t); //replace with real equation
  int child_null_flag = 0;

  if(sse <= this->ErrorLimit()){
    //error within tolerance, so we render this node
    this->Render_Chunk(t, r, modelViewMat);
  } else {
    //verify that children are not null
    if(t->NumChildren() < 4){
      this->Render_Chunk(t, r, this->modelViewMat); //we can't go down another level, so we have to render this one
    } else {
      //so check all the children
      for(int j = 0; j < t->NumChildren(); j++){
        Recursive_Render_Chunk(t->Child(j), r);
      }
    }
  }
}

void View::Render_Chunk(Tile *t, Renderer *r, cs237::mat4f const &modelViewMat)
{
    //this->Dump(std::cout);

    struct Chunk const c = t->Chunk();
    Mesh *m = new Mesh(GL_TRIANGLE_STRIP);

    //scale the vertex array and create vec3 array
    float hscale = t->Cell()->hScale() /3.0f;
    float vscale = t->Cell()->vScale();
    
    //create new vertices array to store transformed values in
    //eventualy we can do morphing here as well
    /*Vertex * mod_vs = new Vertex[c._nVertices];

    for(int i = 0; i<c._nVertices; i++){
      mod_vs[i]._x = c._vertices[i]._x * hscale;
      mod_vs[i]._y = c._vertices[i]._y * vscale;
      mod_vs[i]._z = c._vertices[i]._z * hscale;
    }

    m->LoadVertices(c._nVertices, mod_vs);

    m->LoadIndices(c._nIndices, c._indices);*/

    //apparently we have a function for this
    m->vao = new VAO();
    m->vao->Load(c);

    //for now we manually set color, but eventually we need to change this to get the color from the tree
    m->SetColor(cs237::color3f(0.0, 0.85, 0.313));
    m->SetToWorldMatrix(cs237::translate(cs237::vec3f(0,0,0)));
    r->Render(modelViewMat, m, hscale, vscale);

    //free m
}


/***** Local utility functions *****/

static void Error (int err, const char *msg)
{
    std::cerr << "[GLFW ERROR " << err << "] " << msg << "\n" << std::endl;
}

static GLFWwindow *InitGLFW (int wid, int ht, const char *title)
{
    glfwSetErrorCallback (Error);

    glfwInit ();

  // Check the GLFW version
    {
	int major;
	glfwGetVersion (&major, NULL, NULL);
	if (major < 3) {
	    std::cerr << "CS237 requires GLFW 3.0 or later\n" << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(wid, ht, title, NULL, NULL);
    if (window == nullptr) {
	exit (EXIT_FAILURE);
    }

    glfwMakeContextCurrent (window);

    SetViewport (window);


  // Check the OpenGL version
    {
	GLint major, minor;
	glGetIntegerv (GL_MAJOR_VERSION, &major);
	glGetIntegerv (GL_MINOR_VERSION, &minor);
	if ((major < 4) || ((major == 4) && (minor < 1))) {
	    std::cerr << "CS237 requires OpenGL 4.1 or later; found " << major << "." << minor << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    return window;
}
