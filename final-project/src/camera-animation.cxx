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

void cameraAnimation::start()
{
	this->isStarted = true;
	this->view->initCamera(this->startingPos, this->startingLook, this->startingUp);
	printf("created an animation camera at %f %f %f, looking at %f %f %f, with the up vector %f %f %f\n",
		this->startingPos[0],this->startingPos[1],this->startingPos[2],
		this->startingLook[0],this->startingLook[1],this->startingLook[2],
		this->startingUp[0],this->startingUp[1],this->startingUp[2]);
}

void cameraAnimation::stop()
{
	isTranslated = isRotated = isStarted = isPaused = false;
}

void cameraAnimation::pause()
{
	this->isPaused = ! this->isPaused;
}

void cameraAnimation::handleFrame(float dt)
{
	if(this->isStarted & ! this->isPaused & !(isTranslated & isRotated)){
		this->translateCam(dt);
		this->rotateCamTowardsTarget(dt);
		//this->rotateCamTowardsTargetUp(float dt);
	}

	//isTranslated = this->checkIsTranslated();
	//isRotated = this->checkIsRotated();
}


//========================= CAMERA MOVEMENT =========================//

void cameraAnimation::translateCam(float dt)
{
	//get the direction to our destination
    cs237::vec3d delta = this->endingPos - this->view->Camera().position();

    if((std::abs(delta[0]) > 0.2 | std::abs(delta[1]) > 0.2 | std::abs(delta[2]) > 0.2)){
    //printf("should translate\n");
      delta = normalize(delta);
      //printf("move direction %f %f %f\n", delta[0], delta[1], delta[2]);
      //move in that direction by speed
      //printf("%f\n", this->transSpeed);
      //printf("%f\n", dt);
      delta = delta * (double)this->transSpeed * (double)dt;
      //printf("about to translate cam by %f %f %f\n", delta[0], delta[1], delta[2]);
      //printf("get buffered yo\n");
      this->view->translateCamAnimation(cs237::vec3d(delta[0], delta[1], delta[2]));

      //move our target look as well
      this->endingLook = this->endingLook + cs237::toFloat(delta);
    } else {
    	this->isTranslated = true;
    }
}


void cameraAnimation::rotateCamTowardsTarget(float dt)
{
	cs237::vec3f lookvec = this->view->getCameraLookVec();

	cs237::vec3f translatedEndingLook = this->endingLook - cs237::toFloat(this->view->Camera().position());

	//get the total distance between us and the target
	float theta = dot(normalize(lookvec), normalize(translatedEndingLook));
	//printf("theta is %f\n", theta);

	//check how far we have to go
 	if(std::abs(theta) < 0.99f){
 		//get the axis 
    	cs237::vec3f axis = normalize(cross(normalize(lookvec), normalize(translatedEndingLook)));
    	//printf("axis is %f %f %f\n", axis[0], axis[1], axis[2]);
    	//we turn speed degrees per second 
    	theta = this->turnSpeed * dt;
    	this->view->rotateCamAnimation(theta, axis);
    	//printf("rotated by %f\n", theta);
 	} else {
 		//our vector is there
 		//boom
 		this->isRotated = true;
 	}
}


//========================= COMPLETION POLLING =========================//
bool cameraAnimation::isDone()
{
	return  isRotated & isTranslated;
}





