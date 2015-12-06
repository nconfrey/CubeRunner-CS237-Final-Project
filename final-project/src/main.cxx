/*! \file main.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "world.hxx"
#include "map.hxx"
#include "map-cell.hxx"
#include "view.hxx"
#include <unistd.h>


#define SPEED 2.0f
#define ROLLSPEED 2.0f

/***** callbacks *****
 *
 * These callback functions are wrappers for the methods of the View class
 */

/*! \brief Run the simulation and then redraw the animation.
 */
void Display (GLFWwindow *win)
{
    reinterpret_cast<View *>(glfwGetWindowUserPointer(win))->Render();

} /* end of Display */

/*! \brief Window resize callback.
 *  \param wid the new width of the window.
 *  \param ht the new height of the window. 
 */
void Reshape (GLFWwindow *win, int wid, int ht)
{
    reinterpret_cast<View *>(glfwGetWindowUserPointer(win))->Resize (wid, ht);

} /* end of Reshape */

/*! \brief Keyboard-event callback.
 *  \param win the window receiving the event
 *  \param key The keyboard code of the key
 *  \param scancode The system-specific scancode of the key.
 *  \param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
 *  \param mods the state of the keyboard modifier keys.
 */
void Key (GLFWwindow *win, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE)
	return;

    View *view = (View *)(glfwGetWindowUserPointer(win));

    float errlim = view->getErrorLimit();

    switch (key) {
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
      if (mods == 0)
        view->windowShouldClose();
      break;
    case GLFW_KEY_W: // toggle wireframe mode
      view->toggleWireframe();
      break;
    case GLFW_KEY_EQUAL:
        if (mods == GLFW_MOD_SHIFT) { // shift+'=' is '+'
      // decrease error tolerance
      if (errlim > 0.5)
        view->setErrorLimit(errlim / SQRT_2);
      }
      break;
    case GLFW_KEY_KP_ADD:  // keypad '+'
      if (mods == 0) {
         // decrease error tolerance
      if (errlim > 0.5)
        view->setErrorLimit(errlim / SQRT_2);
      }
      break;
    case GLFW_KEY_MINUS:
      if (mods == 0) {
          // increase error tolerance
        view->setErrorLimit(errlim * SQRT_2);
      }
      break;
    case GLFW_KEY_KP_SUBTRACT:  // keypad '-'
      if (mods == 0) {
          // increase error tolerance
        view->setErrorLimit(errlim * SQRT_2);
      }
     break;
     //OLD CAMERA CONTROLS
    case GLFW_KEY_S:
      view->rotateCamUpDown(-SPEED);
      break;
    case GLFW_KEY_X:
      view->rotateCamUpDown(SPEED);
      break;
    case GLFW_KEY_Z:
      view->rotateCamLeftRight(SPEED);
      break;
    case GLFW_KEY_C:
      view->rotateCamLeftRight(-SPEED);
      break;
    case GLFW_KEY_A:
      view->rotateCamRoll(ROLLSPEED);
      break;
    case GLFW_KEY_D:
      view->rotateCamRoll(-ROLLSPEED);
      break;
      //PLAYER MOVEMENT CONTROLS
    case GLFW_KEY_LEFT:
      break;
    case GLFW_KEY_RIGHT:
      break;
    case GLFW_KEY_UP:
      view->translateCamViewAxis(5.0);
      break;
    case GLFW_KEY_DOWN:
      view->translateCamViewAxis(-5.0);
      break;
    case GLFW_KEY_F:
      view->toggleFog();
      break;
    case GLFW_KEY_L:
      view->toggleLighting();
      break;
    default: // ignore all other keys
      return;
  }

} /* Key */

/*! \brief The mouse enter/leave callback function
 */
void MouseEnter (GLFWwindow *win, int entered)
{
    reinterpret_cast<View *>(glfwGetWindowUserPointer(win))->HandleMouseEnter(entered != 0);

}

/*! \brief The mouse motion callback function
 */
void MouseMotion (GLFWwindow *win, double xPos, double yPos)
{
    reinterpret_cast<View *>(glfwGetWindowUserPointer(win))->HandleMouseMove(xPos, yPos);

}

/*! \brief The mouse button callback function
 */
void MouseButton (GLFWwindow *win, int button, int action, int mods)
{
    reinterpret_cast<View *>(glfwGetWindowUserPointer(win))->HandleMouseButton(button, action, mods);

}

/*! \brief Visibility-change callback: sets the visibility state of the view.
 *  \param state the current state of the window; GL_TRUE for iconified, GL_FALSE otherwise.
 */
void Visible (GLFWwindow *win, int state)
{
    reinterpret_cast<View *>(glfwGetWindowUserPointer(win))->SetVisible (state == GL_TRUE);

} /* end of Visible */

//! main entrypoint
//
int main (int argc, const char **argv)
{
    Map map;


  // get the mapfile
    if (argc < 2) {
	std::cerr << "usage: proj5 <map-dir>" << std::endl;
	return 1;
    }
    std::string mapDir(argv[1]);
    std::clog << "loading " << mapDir << std::endl;
    if (! map.LoadMap (mapDir)) {
	return 1;
    }

    std::clog << "loading cells" << std::endl;
    for (int r = 0;  r < map.nRows(); r++) {
	for (int c = 0;  c < map.nCols();  c++) {
	    map.Cell(r, c)->Load();
	}
    }

    std::clog << "initializing view" << std::endl;
    View *view = new View (&map);
    view->Init (1024, 768);

    World *world = new World(view);

  // initialize the callback functions
    glfwSetWindowRefreshCallback (view->Window(), Display);
    glfwSetWindowSizeCallback (view->Window(), Reshape);
    glfwSetWindowIconifyCallback (view->Window(), Visible);
    glfwSetKeyCallback (view->Window(), Key);
    glfwSetCursorEnterCallback (view->Window(), MouseEnter);
    glfwSetCursorPosCallback (view->Window(), MouseMotion);
    glfwSetMouseButtonCallback (view->Window(), MouseButton);

    //frame information
    double now;
    float dt = 0;
    float lastFrameTime = glfwGetTime(); 

    while (! view->shouldClose()) {
    	//view->Render ();

    	//view->Animate ();

        now = glfwGetTime();
        dt = float(now - lastFrameTime);
        lastFrameTime = now;
        //printf("time: %f\n", now);
        world->handleFrame(now, dt);

    	usleep (1000);	// sleep for 1mS to avoid excessive polling

    	glfwPollEvents ();
    }

    glfwTerminate ();

    return EXIT_SUCCESS;
    
}
