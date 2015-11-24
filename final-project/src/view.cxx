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
#include "buffer-cache.hxx"
#include "texture-cache.hxx"

static void Error (int err, const char *msg);
static GLFWwindow *InitGLFW (int wid, int ht, const char *title);

// animation time step (100Hz)
#define TIME_STEP	0.001

static void SetViewport (GLFWwindow *win)
{
    int fbWid, fbHt;
    glfwGetFramebufferSize (win, &fbWid, &fbHt);
    glViewport(0, 0 , fbWid, fbHt);

}

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

    //init light state
    this->sun.lightDir = this->_map->SunDirection ();
    this->sun.lightInten = this->_map->SunIntensity();
    this->sun.lightAmb = this->_map->AmbientIntensity();

    //init caches
    this->_tCache = new TextureCache();
    this->_nCache = new TextureCache();
    this->_bCache = new BufferCache();

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
    this->fRender = new FullRenderer();
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
    glClearColor (1.0f, 1.0f, 1.0f, 1.0f);  // clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //choose renderer
    Renderer *r;
    if(this->_wireframe){
      r = this->wfRender;
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      r = this->fRender; //eventually this will be the other renderer
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    r->Enable(this->projectionMat, this->sun);

    //loop through all cells in map
    for(int row = 0; row < this->_map->nRows(); row++){
      for(int col = 0; col < this->_map->nCols(); col++){
        //get the cell from the map
        Cell *c = this->_map->Cell(row, col);
        //get the tile from the cell
        Tile *t = &(c->Tile(0)); //this is the root of the quad tree

        //call function to render at appropriate level of detail
        Recursive_Render_Chunk(t, r);

      }
    }

    glfwSwapBuffers (this->_window);

}

float View::SSE(Tile *t)
{
  double dist = t->BBox().distanceToPt(this->_cam.position());
  return this->_cam.screenError(dist, t->Chunk()._maxError);
}

void View::Recursive_Render_Chunk(Tile *t, Renderer *r)
{
  //here we also have to get texture and rnomal information from the respective tqt's

  //calculate SSE
  float sse = this->SSE(t); //replace with real equation

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
    struct Chunk const c = t->Chunk();

    //scales for the vertices
    float hscale = t->Cell()->hScale() / 3.0;
    float vscale = t->Cell()->vScale();

    //get the tqt's
    TQT::TextureQTree *texq = t->Cell()->ColorTQT();
    TQT::TextureQTree *normq = t->Cell()->NormTQT();
    //from the trees, get the tex
    printf("%d : %d %d\n", t->LOD(), t->Cell()->Row(), t->Cell()->Col());
    //Texture *tex = this->TxtCache()->Make(texq, t->LOD(), t->Cell()->Row(), t->Cell()->Col());
    Texture *tex = this->TxtCache()->Make(texq, t->LOD(), 2, 2);
    Texture *norm = this->NormCache()->Make(normq, t->LOD(), t->Cell()->Row(), t->Cell()->Col());
    //use them
    tex->Use(0);
    norm->Use(1);

    //apparently we have a function for this
    VAO *vao = new VAO();
    vao->Load(c);

    //get the cell nw corner position
    cs237::vec3d nw = t->Cell()->_map->NWCellCorner(t->Cell()->Row(), t->Cell()->Col());
    //convert that to the tile's nw corner
    //double factor = t->Cell()->Width() / t->Cell()->xzDim(); //get the width per row
    cs237::vec3d nw_tile = cs237::vec3d(t->NWCol(), 0, t->NWRow());

    //render the chunk
    r->RenderChunk(modelViewMat, vao, hscale, vscale, t->Width(), nw, nw_tile); //cell width, cell NW

    //free vao
    printf("%f %f\n", nw_tile[0], nw_tile[1]);
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
