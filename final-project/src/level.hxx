#ifndef _LEVEL_HXX_
#define _LEVEL_HXX_

#include "cs237.hxx"
#include "cube.hxx"
#include "camera.hxx"

enum locationInPallette {
	GROUND=0,
	SKY,
	BOXCOLORSTART
}; //NOTE: the pallette may have any number colors.
  //These can be accessed using BOXCOLORSTART + {index of box color} 

class Level {

	public:
		Level(int difficulty, int levelNum, float zstart, float zend, float scoreMult, float velocity,
			  cs237::color4f * palletteColors, int nColors, cs237::vec3d playerPos, float width, Sunlight sun);
		~Level();

		//render a cube at every positoon in the cube positoon list
		void RenderAllCubes(Camera c);

		//check if the given AABB intersects any cubes in the cube list
		//note, this needs an intersection function it can call
		bool intersectsAnyCube(cs237::AABBd bb);

		//member access
		//note, members of the level should never be modified, except for the cube list as we
		//dynamically generate cubes, if we are doing that
		int getLevelNum() const {return this->levelNum;} 
		float getZStart() const {return this->zstart;} 
		float getZEnd() const {return this->zend;} 
		float getScoreMult() const {return this->scoreMult;}
		float getVelocity() const {return this->velocity;}

		float getNCubes() const {return this->nCubes;}
		cs237::vec3f **getCubes() const {return this->cubePositions;}

		cs237::color4f getColorAt(locationInPallette l);

	private:
		//level details
		int difficulty; //affects number of cubes generated
		int levelNum; //who am I???
		float zstart; //the start of the level
		float zend;   //the end of the level
		float scoreMult;
		float velocity;
		int nColors;
		int width;
		cs237::color4f * palletteColors; //colors of ground, fog, and cubes

		//cube position list
		//note: since every cube has the exact same mesh
		//we don't need to store and array of cubes objects or anything like that
		//we can render a master cube at each position
		//and similaraily we can do colission detection by having a single AABB defined for the master cube
		//that we check against the player's AABB
		int nCubes;
		cs237::vec3f **cubePositions;
		Cube * masterCube;


		//cube generation
		//divides the zrange into a number of slices and populates each slice with a number of cubes
		//determined by the difficulty
		cs237::vec3f ** generateCubePositions(int nCubes, float zstart, float zend, float width);

		//populates a given zrange wtih cubes based on the difficulty
		void generateCubeSlice(int nCubes, float zstart, float zend, float width, int arrayStart, cs237::vec3f **cubes);

};


#endif // !_LEVEL_HXX_