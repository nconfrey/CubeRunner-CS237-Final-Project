#ifndef _PLAYER_HXX_
#define _PLAYER_HXX_

#include "mesh.hxx"

//stores:
// 	info to render the player to the screen
//	the current position of the player

class Player {
	public:

		Player();
		~Player();

		void Render(); //draw the player

		cs237::vec3f getPos();
		void setPos(cs237::vec3f v);
		cs237::vec3f addToZPos(float z); //also returns the new pos

		Mesh* getMeshInUse();
		void setMeshInUse(int n);

		cs237::color3f getColorInUse();
		void setColorInUse(int n);

		cs237::AABBd getAABB(); //note, we might want to eschew all this and have the AABB be a property of the mesh

	private:
		//players can choose between availible meshes and colors 

		//player mesh
		int curMesh; //current mesh used by the player
		int nMeshes; //number of meshes in the array
		Mesh ** playerMeshes; //array of meshes that by which the player might be represented

		//player color
		int curColor;
		int nColors;
		cs237::color3f * playerColors;

		//position
		cs237::vec3f pos;
		cs237::AABBd bb;
		void updateAABB();
};



#endif // !_PLAYER_HXX_