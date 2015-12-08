#include "world.hxx"

#define OFFSET 1
#define NLEVELS 1
#define NANIMES 2

//========================= LEVEL MAKER =========================//
cs237::color4f pallette1[5] = {cs237::color4f(0.5, 0.5, 0.5, 1.0), cs237::color4f(0.0, 0.0, 0.0,1.0), 
	cs237::color4f(1.0, 0.2, 1.0, 1.0), cs237::color4f(0.2, 1.0, 1.0, 1.0), cs237::color4f(1.0, 1.0, 1.0, 1.0)} ;

void World::generateLevels(View *v)
{
	Level *level1 = new Level(40, 1, 100, 500, 2, 60, pallette1, 5, v->Camera().position(), 100.0f, v->getSun(),
		true, cs237::color3f(0.0f, 0.0f, 0.0f), 0.00375f);
	this->levels[0] = level1;
}

void World::generateAnimes(View *v)
{
	cs237::vec3f up1 = cs237::vec3f(0.0, 1.0, 0.0);
	cs237::vec3f up2 = cs237::vec3f(0.0, 0.0, 1.0);
	cameraAnimation *anime1 = new cameraAnimation(cs237::vec3d(-100.0, 20.0, 100.0), cs237::vec3d(-100.0, 20.0, 100.0),
												  cs237::vec3f(-100.0, 20.0, 120.0), cs237::vec3f(-80.0, 20.0, 113.0),
												  up1, up1,
												  0.1f, 10.0f, 10.0f,
												  view);
	cameraAnimation *anime2 = new cameraAnimation(cs237::vec3d(-100.0, 20.0, 100.0), cs237::vec3d(-100.0, 20.0, 100.0),
												  cs237::vec3f(-100.0, 20.0, 120.0), cs237::vec3f(-50.0, 20.0, 103.0),
												  up1, up1,
												  5.0f, 5.0f, 5.0f,
												  view);
	/*cameraAnimation *anime2 = new cameraAnimation(cs237::vec3d(0.0, 50.0, 100.0), cs237::vec3d(0.0, 50.0, 300.0),
												  cs237::vec3f(0, 20.0, 100.0), cs237::vec3f(0.0, 20.0, 100.0),
												  up2, up2,
												  5.0f, 0.1f, 5.0f,
												  view);*/
	this->animes[0] = anime1;
	this->animes[1] = anime2;
}


//========================= CONSTRUCTOR AND DESTRUCTOR =========================//

//for now we will hard code levels, eventually we would like to load these dynamically
World::World(View *v)
{
	//create level array
	this->numLevels = NLEVELS;
	this->levels = new Level *[NLEVELS];
	//for each level, initalize it with preset data
	//we can make a CREATE LEVEL 1 function, etc
	this->generateLevels(v);

	//set the x edges
	this->xEdge = 100.0f;

	//grab the view
	this->view = v;

	//create the player
	this->player = new Player(v->getSun());

	//make sure all values are intialized to their correct values
	this->restart();

	//set up camera animations
	this->nAnimations = NANIMES;
	this->curAnimation = 0;
	this->animes = new cameraAnimation *[NANIMES];
	this->generateAnimes(view);

	//set the state to title screen
	this->state = TITLE;
}

World::~World()
{
/*	//destroy each level
	for(int i = 0; i<this->numLevels; i++){
		//this->levels[i]->~Level();
	}

	//destroy player
	//this->player->~Player();*/
}

void World::restart()
{	
	//reset world member variables
	this->score = 0.0f;
	this->curAnimation = 0;
	this->curLevel = 0;
	this->tod = 0.0f;
	this->tsd = 0.0f;
	this->player->reset();
	this->view->initCamera(cs237::vec3d(0.0, 20.0, -20), cs237::vec3f(0.0, 20.0f, 10.0), cs237::vec3f(0.0, 1.0, 0.0));
	this->generateLevels(this->view);
}

//========================= EVENT HANDLERS =========================//


//handle a given event based on our state
int World::handleEvent(EventType t)
{
	switch(t)
	{
		case NEWGAME:
			return this->handleEventNEWGAME();
		case COLLISION:
			return this->handleEventCOLLISION();
		case PAUSEBUTTON:
			return this->handleEventPAUSEBUTTON();
		case BACKTOTITLE:
			return this->handleEventBACKTOTITLE();
		default:
			fprintf(stderr, "Error, event type %d not a known event type\n", t);
			exit(-1);
	}
}

//for all event handlers, switch on our current state

//player pressed the new game key while at the title, paused, or dead
//disabled during a game so you don't accidentally hit n and lose progress
//if you wanna die in mid game just hit a cube, fool
int World::handleEventNEWGAME()
{
	cameraAnimation *cur;
	switch(this->state)
	{
		case TITLE:
			cur = this->animes[this->curAnimation];
			cur->stop();
			this->restart(); 
			this->state = RUNNING;
			return 0;
		case DEAD: 
			this->restart();
			this->state = RUNNING;
			return 0;
		case PAUSED:
			this->restart();
			this->state = RUNNING;
			return 0;
		default:
			fprintf(stderr, "Error, cannot handle a NEWGAME event in state %d\n", this->state);
			return this->state + OFFSET; //offset so we can distinguish returning the state enumerated to 0
								   		//from 0 indicating a successful return
										//offset is defined at the top of this file
	}
}

//player collided with a cube while in running
int World::handleEventCOLLISION()
{
	switch(this->state)
	{
		case RUNNING:
			this->killPlayer();
			this->state = DEAD;
			return 0;
		default:
			fprintf(stderr, "Error, cannot handle a COLLISION event in state %d\n", this->state);
			return this->state + OFFSET;
	}
}

//player pressed the pause button while runnning the game or in title
int World::handleEventPAUSEBUTTON()
{
	cameraAnimation *cur;
	switch(this->state)
	{
		case RUNNING:
			this->state = PAUSED;
			return 0;
		case TITLE:
			cur = this->animes[this->curAnimation];
			cur->pause();
			return 0;
		default: 
			fprintf(stderr,"Error, cannot handle a PAUSEBUTTON event in state %d\n", this->state);
			return this->state + OFFSET;
	}
}

//player pressed the backtotitle key when dead or paused
int World::handleEventBACKTOTITLE()
{
	switch(this->state)
	{
		case DEAD:
			this->state = TITLE;
			return 0;
		case PAUSED:
			this->state = TITLE;
			return 0;
		default:
			fprintf(stderr,"Error, cannot handle a BACKTOTITLE event in state %d\n", this->state);
			return this->state + OFFSET;
	}
}

//this is called every tick in main
int World::handleFrame(float t, float dt)
{
	switch(this->state)
	{
		case TITLE:
			//render some kind of informative screen
			this->renderAnimations(dt);
			this->renderWorld();
			break;
		case RUNNING:
			//check if we have any collisions
			if(this->checkForCollisions() != 0){
				this->tod = t;
				this->handleEventCOLLISION();
				break;
			} //if here, no collisions
			this->renderWorld(); //renders ground, player, cubes
			this->view->Animate();
			this->updateScore(dt); 
			this->updatePlayerPosition(dt);
			break;
		case PAUSED:
			this->renderWorld();
			//render some kind of informative screen
			break;
		case DEAD:
			tsd += dt;
			//we can have some sort of fog or particle effect here based on
			//lerping between tsd and some predefined value
			this->renderWorld();
			break;
		default:
			fprintf(stderr, "CRITICAL ERROR: INVALID STATE %d\n", this->state);
			exit(-1);
	}
	return 0;
}


//========================= PER FRAME FUNCTIONS =========================//


void World::renderWorld()
{
	if (! this->view->visible())
    	return;

	//clear the screen
    glClearColor (1.0f, 1.0f, 1.0f, 1.0f);  // clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //update model view mat
    this->view->UpdateModelViewMat();

	this->view->Render(); //draw the heighfield and skybox
	this->levels[curLevel]->RenderAllCubes(this->view->Camera()); //draw every cube in the current level
	
	this->player->Render(this->view->Camera()); //draw the player

	//draw the window to the window
	glfwSwapBuffers (this->view->Window());
}

void World::renderAnimations(float dt)
{
	//seeif our animation is active and move on if not
	cameraAnimation *cur = this->animes[this->curAnimation];
	if(cur->isDone()){
		printf("was done\n");
		cur->stop();
		this->curAnimation = (this->curAnimation + 1) % this->nAnimations;
		cur = this->animes[this->curAnimation];
	}
	if(! cur->started())
		cur->start();

	//render a frame for that animation
	cur->handleFrame(dt);
}

void World::updateScore(float dt)
{
	float mult = this->levels[curLevel]->getScoreMult(); //get the score multiplier from the current level
	this->addToScore(mult * dt); //multiply it by dt and add it to the current score
}

void World::updatePlayerPosition(float dt)
{
	float vel = this->levels[curLevel]->getVelocity(); //get the velocity from the current level
	this->view->translateCamZAxis(dt*vel); //move the camera as well
	this->player->addToZPos(dt * vel); //multiply it by dt and add it to the current z position of the player
	this->player->updateXPos(this->view->Camera()); //update the x pos to match the camera
}



//========================= COLLISION CHECKING =========================//

int World::checkForCollisions()
{
	//check if the player's x coord is out of bounds
	if(std::abs(this->player->getPos()[0]) > this->xEdge){
		return 2;
	}
	//check if the player is touching any cube
	if(this->levels[curLevel]->intersectsAnyCube(this->player->getAABB())){
		printf("COLLISION YOU LOSER\n");
		//exit(-1);
		return 1;
	}

	//we good
	return 0;
}

void World::killPlayer()
{
	this->player->setColorInUse(1);
}


//========================= ACCESS MEMBER VARIABLES =========================//

float World::getScore()
{
	return this->score;
}

float World::addToScore(float add)
{
	this->score += add;
	return this->score;
}







