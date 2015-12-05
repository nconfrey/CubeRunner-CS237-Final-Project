/*! \file view.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "cube.hxx"
#include "map.hxx"
#include "camera.hxx"
#include <vector>
#include "render.hxx"
#include "map-cell.hxx"
#include "skybox.hxx"

#define SIN_ONE_DEGREE  0.0174524064373f
#define COS_ONE_DEGREE  0.999847695156f
#define SQRT_2 1.41421356237f

class View {
  public:

  //! construct a viewer for the map
    View (Map *map);

  //! return the view's window
    GLFWwindow *Window() const { return this->_window; }

  //! initialize the view (i.e., allocate its window)
    void Init (int wid, int ht);

  //! method to handle display of the view
    void Render ();

    void Recursive_Render_Chunk(Tile *t, Renderer *r, int row, int col);

    bool inFrustum(Tile *t);

    void Render_Chunk(Tile *t, Renderer *r, cs237::mat4f const &modelViewMat, int row, int col, float morphFactor);

  //! animation method
    void Animate ();

    void toggleWireframe();

    void toggleLighting();

    void toggleFog();

    float getErrorLimit();

    void setErrorLimit(float newlim);

//=====rotate camera controls======//

    //rotate camera around an arbitrary axis
    void rotateCam(float theta, cs237::vec3f axis);

    void rotateCamUpDown(float theta);

    void rotateCamLeftRight(float theta);

    void rotateCamRoll(float theta);


    //=====translate camera and look at point=====/

    //translate cam along arbitrary axis, without rotating view at all
    void translateCam(cs237::vec3f offset);

    void translateCamViewAxis(float dis);

    void translateCamStrafeAxis(float dis);

    void translateCamUpAxis(float dis);



    void windowShouldClose();

  //! handle kwyboard input
    void HandleKey (int key, int scancode, int action, int mods);

  //! handle mouse entering/leaving the view's window
    void HandleMouseEnter (bool entered);

  //! handle mouse movement
    void HandleMouseMove (double x, double y);

  //! handle mouse buttons
  //! \param button the mouse button that was pressed
  //! \param action was the button pressed or released?
  //! \param mods   the state of the modifier keys
    void HandleMouseButton (int button, int action, int mods);

  /*! \brief initialize the renderers by loading and compiling their shaders.
   *  Note that this function needs to be called after the current
   *  OpenGL context has been set.
   */
    void InitRenderers ();

    //calculate the model view matrix for the given camera position
    void UpdateModelViewMat ();

  //! handle resizing the view
    void Resize (int wid, int ht);

  //! handle visibility change
    void SetVisible (bool isVis) { this->_isVis = isVis; }

  //! should this view close?
    bool shouldClose () const { return glfwWindowShouldClose(this->_window); }

  //! is the view in wireframe mode?
    bool wireframeMode () const { return this->_wireframe; }

  //! the view's current camera state
    class Camera const &Camera () const { return this->_cam; }

    cs237::mat4f ModelViewMat() { return modelViewMat; }
    cs237::mat4f ProjectionMat() { return projectionMat; }//<! the camera's projection matrix

  //! the view's current error limit
    float ErrorLimit () const { return this->_errorLimit; }

    //calculate screen space error
    float SSE(Tile *t);

  //! the cache of VAO objects for representing chunks
    class BufferCache *VAOCache () const { return this->_bCache; }

  //! the cache of OpenGL textures for the map tiles
    class TextureCache *TxtCache () const { return this->_tCache; }

  //! the cache of normals for the map tiles
    class TextureCache *NormCache () const { return this->_nCache; }

  private:
    Map		*_map;		//!< the map being rendered
    class Camera _cam;		//!< tracks viewer position, etc.
    float	_errorLimit;	//!< screen-space error limit
    bool	_isVis;		//!< true when this window is visible
    GLFWwindow	*_window;	//!< the main window 
    int		_fbWid;		//!< current framebuffer width
    int		_fbHt;		//!< current framebuffer height
    double	_lastFrameTime;	//!< time of last frame
    double	_lastStep;	//!< time of last animation step
    cs237::AABBd _mapBBox;	//!< a bounding box around the entire map

    Sunlight sun; //container for the light info
    Skybox *skybox;
    Cube *cube;

    cs237::mat4f modelViewMat; //!< the current model-view matrix
    cs237::mat4f projectionMat; //<! the camera's projection matrix

    Renderer * wfRender;
    Renderer * fRender;

  // resource management
    class BufferCache	*_bCache;	//! cache of OpenGL VAO objects used for chunks
    class TextureCache	*_tCache;	//! cache of OpenGL textures
    class TextureCache *_nCache; //! cache of normals

    //internal logic
    bool _fogOn;
    bool _lightingOn;
    bool _wireframe; //!< true if we are rendering the wireframe

    //smooth camera movement
    cs237::vec3f lookTarget;
    cs237::vec3f upTarget;
    float turnSpeed;
    bool smoothCam;
    void rotateTargetVector(float theta, cs237::vec3f axis);
    void rotateTargetVector_inner(float theta, cs237::vec3f axis, bool recursiveCall);
    void rotateTowardsTarget(float dt);

    void rotateUpTarget(float theta); //rotates the up vector around the look axis for camera rolling
    void rotateUpTarget_inner(float theta, bool recursiveCell);
    void rotateTowardsTargetUp(float dt);


};

#endif // !_VIEW_HXX_
