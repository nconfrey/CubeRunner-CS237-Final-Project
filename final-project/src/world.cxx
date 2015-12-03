#include "world.hxx"

#define OFFSET 1;

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//

//for now we will hard code levels, eventually we would like to load these dynamically
World::World()
{
	//create level array
	this->numLevels = 5;
	//this->levels = new Level()*[5];
	//for each level, add it to the array

	//set the x edges
	this->xEdge = 10.0f;

	//create the player
	//this->player = new Player();

	//make sure all values are intialized to their correct values
	this->restart();

	//set the state to title screen
	this->state = TITLE;
}

World::~World()
{
	//destroy each level
	for(int i = 0; i<this->numLevels; i++){
		//this->levels[i]->~Level();
	}

	//destroy player
	//this->player->~Player();
}

void World::restart()
{	
	//reset world member variables
	this->score = 0.0f;
	this->curLevel = 0;
	this->tod = 0.0f;
	this->tsd = 0.0f;
	//reset player position

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
				//oops, collision
				this->tod = t;
				this->handleEventCOLLISION();
				break;
			}
			this->renderWorld();
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
	//this->levels[curLevel]->Render(); //draw every cube in the current level
	//this->player->Render; //draw the player
}

void World::updateScore(float dt)
{
	//get the score multiplier from the current level
	//multiply it by dt
	//add it to the current score
}

void World::updatePlayerPosition(float dt)
{
	//get the velocity from the current level
	//multiply it by dt
	//add it to the current z position of the player
}



//========================= COLLISION CHECKING =========================//

int World::checkForCollisions()
{
	//check if the player's x coord is > abs(xEdge)

	//get the cubelist of the current level
	//for every cube in that cubelist,
	// 	check if we intersect
	return 1; //delete this
}

void World::killPlayer()
{
	//we might want this function eventually
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







