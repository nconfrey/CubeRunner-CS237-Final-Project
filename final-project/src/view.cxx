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

static void SetViewport (GLFWwindow *win)
{
    int fbWid, fbHt;
    glfwGetFramebufferSize (win, &fbWid, &fbHt);
    glViewport(0, 0 , fbWid, fbHt);

}

/***** class View member functions *****/

View::View (Map *map)
    : _map(map), _errorLimit(2.0), _isVis(true), _window(nullptr),
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
    this->_cam.setNearFar (10.0, 2*1.5 * double(this->_map->CellWidth()) * double(this->_map->hScale()));
    this->Resize (wid, ht);

  // initialize shaders
    /* YOUR CODE HERE */
    this->InitRenderers();
    this->skybox = new Skybox(1000,1000);
    this->cube = new Cube();

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

    //logic intialization
    this->_wireframe = true;
    this->_lightingOn = true;
    this->_fogOn = true;

}

void View::HandleKey (int key, int scancode, int action, int mods)
{

}

void View::toggleWireframe()
{
  this->_wireframe = !this->_wireframe;
}

void View::toggleLighting()
{
  this->_lightingOn = !this->_lightingOn;
}

void View::toggleFog()
{
  this->_fogOn = !this->_fogOn;
}

float View::getErrorLimit()
{
  return this->_errorLimit;
}

void View::setErrorLimit(float newlim)
{
  this->_errorLimit = newlim;
}

//=====rotate camera controls======//

//rotate camera around an arbitrary axis
void View::rotateCam(float theta, cs237::vec3f axis)
{
  this->_cam.rotateCam(theta, axis);
}

void View::rotateCamUpDown(float theta)
{
  this->_cam.rotateCamUpDown(theta);
}

void View::rotateCamLeftRight(float theta)
{
  this->_cam.rotateCamLeftRight(theta);
}

void View::rotateCamRoll(float theta)
{
  this->_cam.rotateCamRoll(theta);
}


//=====translate camera and look at point=====/

//translate cam along arbitrary axis, without rotating view at all
void View::translateCam(cs237::vec3f offset)
{

}

void View::translateCamViewAxis(float dis)
{

}

void View::translateCamStrafeAxis(float dis)
{

}

void View::translateCamUpAxis(float dis)
{

}



void View::windowShouldClose()
{
  glfwSetWindowShouldClose (this->_window, true);
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

    //update model view mat
    this->UpdateModelViewMat();

    //clear the screen
    glClearColor (1.0f, 1.0f, 1.0f, 1.0f);  // clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox->Render(this->projectionMat, this->modelViewMat);
    //cube->Render(cs237::vec3f(0,0,0), cs237::color3f(0,0,0), this->modelViewMat, this->projectionMat);

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
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xffff);
    for(int row = 0; row < this->_map->nRows(); row++){
      for(int col = 0; col < this->_map->nCols(); col++){
        //get the cell from the map
        Cell *c = this->_map->Cell(row, col);
        //get the tile from the cell
        Tile *t = &(c->Tile(0)); //this is the root of the quad tree

        //call function to render at appropriate level of detail
        Recursive_Render_Chunk(t, r, 0, 0);

      }
    }
    glDisable(GL_PRIMITIVE_RESTART);


    glfwSwapBuffers (this->_window);

}

bool View::inFrustum(Tile *t)
{
  //printf("considering tile [%d][%d]", t->NWRow(), t->NWCol());
  cs237::AABBd boundingBox = t->BBox();
  Plane *frust = new Plane();
  
  //extract the planes in camera space
  Plane **frustum = frust->extractPlanes(this->Camera(), this->Camera().projTransform() * this->Camera().ModelViewMatrix());

  //Plane **frustum = frust->extractPlanes(this->Camera());
  printf("Ref:\n far plane is %f\n near plane is %f\n", this->Camera().far(), this->Camera().near());
  cs237::mat3x3d mv = cs237::mat3x3d(cs237::lookAt(cs237::vec3d(0.0, 0.0, 0.0),
                          cs237::toDouble(this->Camera().direction()),
                          cs237::toDouble(this->Camera().up())));

  for(int i = 0; i < 6; i++)
  {
    //frustum[i]->NormalizePlane();
    frustum[i]->transformPlane(mv, mv * -(this->Camera().position()));
    printf("Plane [%d]: %f, %f, %f, %f\n", i, frustum[i]->a, frustum[i]->b, frustum[i]->c, frustum[i]->d);
  }

  int totalIn = 0;

  //Test all 8 corners of the bounding box with the 6 planes of the view frustum
  for(int frustPlane = 0; frustPlane < 6; frustPlane++)
  {
    printf("CONSIDERING PLANE [%d]\n", frustPlane);
    //counting how many corners are in the planes
    int inCount = 8;
    int somewhatIn = 1;
    for(int i = 0; i < 8; i++)
    {
      //test this corner against all of the planes
      cs237::vec3d c = boundingBox.corner(i);
      printf("corner [%d] is %f, %f, %f\n", i, c.x, c.y, c.z);
      
      if(frustum[frustPlane]->ClassifyPoint(c, this->Camera().projTransform() * this->Camera().ModelViewMatrix()) == Plane::OUTSIDE)
      {
        //printf("the plane [%d] makes it outside\n", frustPlane);
        somewhatIn = 0;
        inCount--;
      }
    }
      
      printf("\n\n");

    if(inCount == 0)
    {
      //all the corners of the bounding box were outside of the planes
      printf("CULLED BY FACE [%d]\n", frustPlane);
      return false;
    }
    totalIn += somewhatIn;
  }
  //exit(-1);

  //printf("total corners in is: %d", totalIn);
  if(totalIn == 6)
    return true; // all of the corners are inside the view

  //otherwise, we are partially in which we will consider as an in for now
  //will come back to this later when optimizing

  //printf("base case return\n");
  return true;
}

float View::SSE(Tile *t)
{
  double dist = t->BBox().distanceToPt(this->_cam.position());
  return this->_cam.screenError(dist, t->Chunk()._maxError);
}

void View::Recursive_Render_Chunk(Tile *t, Renderer *r, int row, int col)
{
  //check to see if this tile is in the view frustum to save time
  //if(!inFrustum(t))
  // return;

  //calculate SSE
  float sse = this->SSE(t);
  float sseLim = this->ErrorLimit();
  float morphFactor = std::max(0.0f, std::min(1.0f, (2.0f*sse/sseLim - 1.0f))); //clamp to 1

  if(sse <= sseLim){
    //error within tolerance, so we render this node
    this->Render_Chunk(t, r, modelViewMat, row, col, morphFactor);
  } else {
    //verify that children are not null
    if(t->NumChildren() < 4 || t->LOD() >= t->Cell()->Depth()){
      this->Render_Chunk(t, r, this->modelViewMat, row, col, morphFactor); //we can't go down another level, so we have to render this one
    } else {
      //so check all the children
      //we are gonna double the number of rows and cols when we go down a LOD
      for(int j = 0; j < t->NumChildren(); j++){
        int userow = row; int usecol = col;
        //if(t->LOD() < t->Cell()->ColorTQT()->Depth() && t->LOD() < t->Cell()->NormTQT()->Depth()){
          //we can go deeper
          userow*=2; usecol*=2;
          if(j == 1) {usecol++;} //we're on the right side
          if(j == 2) {userow++; usecol++;} //we're on the right and bottom
          if(j == 3) {userow++;} //we're on the bottom
        //}
        //printf("%d, %d", userow, usecol);
        Recursive_Render_Chunk(t->Child(j), r, userow, usecol);
      }
    }
  }
}

void View::Render_Chunk(Tile *t, Renderer *r, cs237::mat4f const &modelViewMat, int row, int col, float morphFactor)
{
    struct Chunk const c = t->Chunk();

    //scales for the vertices
    float hscale = t->Cell()->hScale();
    float vscale = t->Cell()->vScale();

    //first, check what information is already chaced for us in the tile struct
    VAO *vao = t->VAO();
    Texture *tex = t->Tex();
    Texture *norm = t->Norm();
    //nullptr indicates it was not already cached
    if(vao == nullptr){
      //create it in the bufferchace
      vao = this->VAOCache()->Acquire();
      vao->Load(c);
      t->setVAO(vao);
    }
    if(tex == nullptr){
      TQT::TextureQTree *texq = t->Cell()->ColorTQT();
      int depth = std::min(t->LOD(), texq->Depth()-1);
      tex = this->TxtCache()->Make(texq, depth, row, col);
      t->setTex(tex);
    }
    if(norm == nullptr){
      TQT::TextureQTree *normq = t->Cell()->NormTQT();
      int depth = std::min(t->LOD(), normq->Depth()-1);
      norm = this->NormCache()->Make(normq, depth, row, col);
    }

    //use them
    tex->Use(0);
    norm->Use(1);

    //get the cell nw corner position for the cell
    cs237::vec3d nw = t->Cell()->_map->NWCellCorner(t->Cell()->Row(), t->Cell()->Col());
    //get the nw corner for the tile
    cs237::vec3d nw_tile = cs237::vec3d(t->NWCol(), 0, t->NWRow());

    //create uniforms struct;
    uniforms *u = new uniforms;
    u->hscale = hscale;
    u->vscale = vscale;
    u->tw = t->Width();
    u->nw_pos = nw;
    u->nw_tile = nw_tile;
    u->hasfog = t->Cell()->_map->hasFog() & this->_fogOn;
    u->fogcolor = t->Cell()->_map->FogColor();
    u->fogdensity = t->Cell()->_map->FogDensity();
    u->morphFactor = morphFactor;
    u->lightingOn = this->_lightingOn;

    //render the chunk
    r->RenderChunk(modelViewMat, vao, u); //cell width, cell NW
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
