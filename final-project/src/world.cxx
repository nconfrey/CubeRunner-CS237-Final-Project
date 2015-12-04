#include "world.hxx"

#define OFFSET 1
#define NLEVELS 1

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//

//for now we will hard code levels, eventually we would like to load these dynamically
World::World()
{
	//create level array
	this->numLevels = NLEVELS;
	this->levels = new Level *[NLEVELS];
	//for each level, initalize it with preset data
	//we can make a CREATE LEVEL 1 function, etc

	//set the x edges
	this->xEdge = 10.0f;

	//create the player
	this->player = new Player();

	//make sure all values are intialized to their correct values
	this->restart();

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
	this->curLevel = 0;
	this->tod = 0.0f;
	this->tsd = 0.0f;
	this->player->setPos(cs237::vec3f(0.0f, 0.0f, 0.0f));

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
	switch(this->state)
	{
		case TITLE:
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

//player pressed the pause button while runnning the game
int World::handleEventPAUSEBUTTON()
{
	switch(this->state)
	{
		case RUNNING:
			this->state = PAUSED;
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
			break;
		case RUNNING:
			//check if we have any collisions
			if(!this->checkForCollisions()){
				this->tod = t;
				this->handleEventCOLLISION();
				break;
			} //if here, no collisions
			this->renderWorld(); //renders ground, player, cubes
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
		default:
			fprintf(stderr, "CRITICAL ERROR: INVALID STATE %d\n", this->state);
			exit(-1);
	}
	return 0;
}


//========================= PER FRAME FUNCTIONS =========================//

void World::renderWorld()
{
	this->view->Render(); //draw the heighfield
	this->levels[curLevel]->RenderAllCubes(); //draw every cube in the current level
	this->player->Render(); //draw the player
}

void World::updateScore(float dt)
{
	float mult = this->levels[curLevel]->getScoreMult(); //get the score multiplier from the current level
	this->addToScore(mult * dt); //multiply it by dt and add it to the current score
}

void World::updatePlayerPosition(float dt)
{
	float vel = this->levels[curLevel]->getVelocity(); //get the velocity from the current level
	this->player->addToZPos(dt * vel); //multiply it by dt and add it to the current z position of the player
}



//========================= COLLISION CHECKING =========================//

int World::checkForCollisions()
{
	//check if the player's x coord is out of bounds
	if(this->player->getPos()[0] > std::abs(this->xEdge)){
		return 2;
	}
	
	//check if the player is touching any cube
	if(this->levels[curLevel]->intersectsAnyCube(this->player->getAABB())){
		return 1;
	}

	//we good
	return 0;
}

void World::killPlayer()
{
	//we might want this function eventually
	//could handle some kind of death animation?
	//though we probably want to just handle that a a lerp
	//between normal -> final death state
	//based on time since death
	//in the per frame renderer
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







