#include "camera-animation.hxx"

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//

cameraAnimation::cameraAnimation(cs237::vec3d startingPos, cs237::vec3d endingPos, 
				cs237::vec3f startingLook, cs237::vec3f endingLook,
				cs237::vec3f startingUp, cs237::vec3f endingUp,
				float transSpeed, float turnSpeed, float upSpeed,
				View *view)
{
	this->startingPos = startingPos;
	this->endingPos = endingPos;
	this->startingLook = startingLook;
	this->endingLook = endingLook;
	this->startingUp = startingUp;
	this->endingUp = endingUp;
	this->turnSpeed = turnSpeed;
	this->transSpeed = transSpeed;
	this->upSpeed = upSpeed;
	this->view = view;

	//internal state init
	isTranslated = isRotated = isStarted = isPaused = false;
}

cameraAnimation::~cameraAnimation()
{}


//========================= STARTING, STOPPING, AND RENDERING =========================//

void cameraAnimation::handleFrame(float dt)
{
	if(this->isStarted & ! this->isPaused){

	}
}


//=========================  =========================//




//=========================  =========================//