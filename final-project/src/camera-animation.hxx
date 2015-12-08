#ifndef _CAMERA_ANIMATION_
#define _CAMERA_ANIMATION_

#include "camera.hxx"
#include "view.hxx"

class cameraAnimation {

public:
	cameraAnimation(cs237::vec3d startingPos, cs237::vec3d endingPos, 
					cs237::vec3f startingLook, cs237::vec3f endingLook,
					cs237::vec3f startingUp, cs237::vec3f endingUp,
					float transSpeed, float turnSpeed, float upSpeed,
					View *view);
	~cameraAnimation();

	//update all vectors based on the time passed
	void handleFrame(float dt);

	//tells the camera to start moving towards its targets
	void start();

	//tells the camera to stop permanently
	void stop();

	//stops camera movement until pause is called again
	void pause();

	//return true if the animation has arrived at its targets
	//returns false otherwise
	bool isDone();

	bool started() const {return isStarted;}

private:
	cs237::vec3d startingPos;
	cs237::vec3f startingLook;
	cs237::vec3f startingUp;
	cs237::vec3d endingPos;
	cs237::vec3f endingLook;
	cs237::vec3f endingUp;
	float transSpeed;
	float turnSpeed;
	float upSpeed;

	//bool checkIsTranslated(); //check if we are done moving
	//bool checkIsRotated(); //check if we are done rotating

	//internal state vars
	bool isTranslated;
	bool isRotated;
	bool isStarted;
	bool isPaused;

	//the view to do all this animation for
	View *view;

	//handle the movement
	void translateCam(float dt);
	void rotateCamTowardsTarget(float dt);

};




#endif // ! _CAMERA_ANIMATION_