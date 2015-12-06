#include "level.hxx"

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//
Level::Level(int difficulty, int levelNum, float zstart, float zend, float scoreMult, float velocity,
			  cs237::color3f * palletteColors, int nColors, cs237::vec3d playerPos)
{
	this->difficulty = difficulty;
	this->levelNum = levelNum;
	this->zstart = zstart;
	this->zend = zend;
	this->scoreMult = scoreMult;
	this->velocity = velocity;
	this->nColors = nColors;
	this->masterCube = new Cube(); //nick: Should this be here or in view? we can pass it in (only need to make 1 per game)
	this->palletteColors = new cs237::color3f[nColors];
	for(int i = 0; i<nColors; i++){
		this->palletteColors[i] = palletteColors[i];
	}

	//Randomly generate cubes for this level
	this->cubePositions = new cs237::vec3f *[50];
	this->nCubes = 50;
 	for(int i = 0; i < 50; i++)
 	{
 		//Assuming for now player is traveling down z axis
 		//new cubes are placed on the x/z plane
 		float x = rand() % 5000;
      	float z = rand() % 2000;
 		cubePositions[i] = new cs237::vec3f(x, 500, z);
 		printf("cube %d at %f, %f, %f\n", i, cubePositions[i]->x, cubePositions[i]->y, cubePositions[i]->z);
 	}
}

Level::~Level()
{}



//========================= MEMBER ACCESS =========================//
//most of these are defined in the header file
cs237::color3f Level::getColorAt(locationInPallette l)
{
	return this->palletteColors[l];
}


//========================= CUBE RENDERING =========================//

void Level::RenderAllCubes(Camera c)
{
	//Check the cubes for any that are behind the camera
	//If they are in front of the player, render
	//Otherwise, generate a new cube at a position ahead of the player
	std::cout << "RenderAllCubes: " << c.projTransform() << "\n";
	//glDisable(GL_DEPTH_TEST);
	float x;
	float z;
	for(int i = 0; i < this->nCubes; i++){
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if(true)//c.position().z > this->cubePositions[i]->z)
		{
			printf("RECYCLE CUBE #%d\n", i);
			x = rand() % 5000;
      		z = rand() % 2000; //+ c.position().z;
      		//memory leak here
 			cubePositions[i] = new cs237::vec3f(x, 500, z);
		}
		
		//masterCube->Render(*(this->cubePositions[i]), cs237::color4f(r,g,b,1.0f), c.projTransform(), c.ModelViewMatrix());
		masterCube->Render(cs237::vec3f(x,500,z), cs237::color4f(r, g, b, 1.0), c.projTransform(), c.ModelViewMatrix());
	}

}

//========================= COLLISION DETECTION =========================//

bool Level::intersectsAnyCube(cs237::AABBd bb)
{
	for(int i = 0; i<this->nCubes; i++){
		//if(intersectionTest(this->cubeAABB(this->cubePositions[i]), bb) {
		//	return true;
		//}
	}
	return false;
}

cs237::AABBd cubeAABB(cs237::vec3f)
{
	//not gonna do this right now
}