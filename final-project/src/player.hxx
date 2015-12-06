#ifndef _PLAYER_HXX_
#define _PLAYER_HXX_

#include "cs237.hxx"
#include "mesh.hxx"
#include "camera.hxx"
#include "cube.hxx"

//stores:
// 	info to render the player to the screen
//	the current position of the player

class Player {
	public:

		Player(Sunlight sun);
		~Player();

		void Render(Camera c); //draw the player

		cs237::vec3f getPos();
		void setPos(cs237::vec3f v);
		cs237::vec3f addToZPos(float z); //also returns the new pos
		cs237::vec3f updateXPos(Camera c);

		Cube* getMeshInUse();
		void setMeshInUse(int n);

		cs237::color4f getColorInUse();
		void setColorInUse(int n);

		cs237::AABBd getAABB(); //note, we might want to eschew all this and have the AABB be a property of the mesh

		void reset();

	private:
		//players can choose between availible meshes and colors 

		//player mesh
		int curMesh; //current mesh used by the player
		int nMeshes; //number of meshes in the array
		Cube ** playerMeshes; //array of meshes that by which the player might be represented
								//have to hijack this and make it cube only until we have some kind of drawables struct

		//player color
		int curColor;
		int nColors;
		cs237::color4f * playerColors;

		//position
		cs237::vec3f pos;
		cs237::AABBd bb;
		void updateAABB();
};



#endif // !_PLAYER_HXX_