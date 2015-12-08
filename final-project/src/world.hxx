#ifndef _WORLD_HXX_
#define _WORLD_HXX_

#include "cs237.hxx"
#include "level.hxx"
#include "view.hxx"
#include "map.hxx"
#include "player.hxx"
#include "render.hxx"
#include "camera-animation.hxx"


enum WorldState { //world state
	TITLE = 0,
	RUNNING,
	DEAD,
	PAUSED,
	WIN
};

enum EventType { //things that can cause us to change state
	NEWGAME = 0,
	COLLISION,
	PAUSEBUTTON,
	BACKTOTITLE,
};

class World {
	public:
		World(View *v);
		~World();

		//a dispatch function for each event type
		int handleEvent(EventType t);
		int handleFrame(float t, float dt);
		void renderAnimations(float dt);

		//access member variables
		float getScore();				//returns the player score
		float addToScore(float add);	//adds the value to the current score and returns the new score
										//note, the score addition based on time elapsed is handled in handleFrame
										//this function might be used for something like level completion bonus or sth

		//draw everything in the world, including the heightfield, the player, and the cubes of the current level
		void renderWorld();

	private:
		WorldState state;

		//functions to handle specific events based on game state
		int handleEventNEWGAME();
		int handleEventCOLLISION();
		int handleEventPAUSEBUTTON();
		int handleEventBACKTOTITLE();

		//resets score to 0, places player at begining of level 1
		void restart();

		//check if our intrepid player is inside of any cubes or outside the defined edges
		float xEdge;
		int checkForCollisions(); //0 indicates no collisions
								  //1 indicates the player hit a cube
								  //2 indicates the player is off the x edges
		float tod; //time of death
		float tsd; //time since death;

		//cumulative player score
		float score;

		//per frame updates
		void updateScore(float dt);
		void updatePlayerPosition(float dt);
		bool playerPastLevel();

		//LEVELS
		void generateLevels(View *v);
		int numLevels;
		int curLevel;
		Level** levels;

		//PLAYER
		Player *player;
		void killPlayer();

		//VIEW
		View *view;

		//CAMERA ANIMATIONS DURING TITLE
		void generateAnimes(View *v);
		int nAnimations;
		int curAnimation;
		cameraAnimation **animes;
		bool inAnimation;




};


#endif // !_WORLD_HXX_