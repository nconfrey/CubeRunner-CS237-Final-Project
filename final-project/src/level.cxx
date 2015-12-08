#include "level.hxx"

#define yOffset 15
#define SLICE_DENSITY 10 //the number of slices per 1000 z values

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//
Level::Level(int difficulty, int levelNum, float zstart, float zend, float scoreMult, float velocity,
			  cs237::color4f * palletteColors, int nColors, cs237::vec3d playerPos, float width, Sunlight sun,
			  bool hasFog, float fogDensity, bool wireframe, cs237::vec3f up, float zstop)
{
	this->difficulty = difficulty;
	this->levelNum = levelNum;
	this->zstart = zstart;
	this->zend = zend;
	this->width = width;
	this->scoreMult = scoreMult;
	this->velocity = velocity;
	this->nColors = nColors;
	this->masterCube = new Cube(sun, 1, 1); //nick: Should this be here or in view? we can pass it in (only need to make 1 per game)
	this->palletteColors = new cs237::color4f[nColors];
	for(int i = 0; i<nColors; i++){
		this->palletteColors[i] = palletteColors[i];
	}

	this->hasFog = hasFog;
	this->fogColor = cs237::color3f(palletteColors[SKY][0], palletteColors[SKY][1], palletteColors[SKY][2]);
	this->fogDensity = fogDensity;

	this->wireframe = wireframe;
	this->up = up;

	//Randomly generate cubes for this level
	this->nCubes = difficulty * 10;
	//this->cubePositions = new cs237::vec3f *[this->nCubes];
	this->generateCubePositions(this->nCubes, zstart, zend, width);

	this->zstop = zstop;
}

Level::~Level()
{}


//========================= PROCEDURAL CUBE GENERATION =========================//

//divides the zrange into a number of slices and populates each slice with a number of cubes
//determined by the difficulty
void Level::generateCubePositions(int nCubes, float zstart, float zend, float width)
{

	this->cubePositions = new cs237::vec3f *[nCubes];
	this->cubeColors = new locationInPallette [nCubes];
	//calculate the number of slices and density of those slices
	int nSlices = (SLICE_DENSITY * (zend - zstart)) / 1000;
	int sliceWidth = (zend - zstart) / nSlices;
	int cubesPerSlice = nCubes / nSlices;
	int nCubesAfterSlicing = 0;

	//note this leaves some remainder without any cubes at the end of each level

	for(int i = 0; i<nSlices-1; i++)
	{
		//printf("indexing into %d to %d\n", i*cubesPerSlice, (i+1)*cubesPerSlice-1);
		this->generateCubeSlice(cubesPerSlice, i*sliceWidth + zstart, (i+1)*sliceWidth + zstart - 1, width, i*cubesPerSlice);
		nCubesAfterSlicing += cubesPerSlice;
	}

	//handle remainder
	int cubesLeft = nCubes - nCubesAfterSlicing;
	this->generateCubeSlice(cubesLeft, sliceWidth*(nSlices-1) + zstart, zend, width, (nSlices-1)*cubesPerSlice);

}

//populates a given zrange wtih cubes based on the difficulty
void Level::generateCubeSlice(int nCubes, float zstart, float zend, float width, int arrayStart)
{
	for(int i=arrayStart; i<nCubes+arrayStart; i++)
	{
		//generate a color for the cube from the pallette
		locationInPallette color = (locationInPallette)(rand() % (this->nColors - 2));
		//new cubes are placed on the x/z plane
 		float x = rand() % (int)(2.0f * width);
 		x -= width;
      	float z = rand() % (int)(zend - zstart);
      	z += zstart;
      	//printf("%d  ",i);
 		this->cubePositions[i] = new cs237::vec3f(x, 15, z);
 		this->cubeColors[i] = (locationInPallette)(BOXCOLORSTART+color);
 		//printf("%f %f %f\n", cubes[i]->x, cubes[i]->y, cubes[i]->z );
	}
}


//========================= MEMBER ACCESS =========================//
//most of these are defined in the header file
cs237::color4f Level::getColorAt(locationInPallette l)
{
	return this->palletteColors[l];
}


//========================= CUBE RENDERING =========================//

void Level::RenderAllCubes(Camera c, bool inAnimation)
{
	//Check the cubes for any that are behind the camera
	//If they are in front of the player, render
	//Otherwise, generate a new cube at a position ahead of the player
	//std::cout << "RenderAllCubes: " << c.projTransform() << "\n";
	//glDisable(GL_DEPTH_TEST);
	float x,z;
	//int color;
	for(int i = 0; i < this->nCubes; i++){
		if((c.position().z > this->cubePositions[i]->z) & !inAnimation)
		{
			//get a random color from the number of box colors
			//color = rand() % (this->nColors - 2);
			//randomize the xvalue
			x = rand() % (int)(2.0f * this->width) - this->width;
			//take the z value, wiggle it a little
			//increment it by the length of the field (zend - zstart)
			//translate the z value by that amount
      		z = (((rand() % 100) - 100) / 2) + (zend-zstart);
      		if(!(z > this->zstop)) //don't add cubes beyond the end of the level
 				cubePositions[i] = new cs237::vec3f(x, 15, cubePositions[i]->z+z);
		}
		
		if(this->wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		masterCube->Render(*(this->cubePositions[i]), this->getColorAt(this->cubeColors[i]), c.projTransform(), c.ModelViewMatrix(),
			this->hasFog, this->fogColor, this->fogDensity);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

//========================= COLLISION DETECTION =========================//

bool intersectionTest(cs237::AABBd bb, cs237::AABBd bb2)
{
	//std::cout << "Cube pos\n";
	//std::cout << bb.center();
	//std::cout << "Player pos\n";
	//std::cout << bb2.center();
	//return bb.includesPt(bb2.center()) || bb2.includesPt(bb.center());
	for (int i = 0; i < 8; ++i)
	{
		if(bb.distanceToPt(bb2.corner(i)) <= 0)
			return true;
	}
}

bool Level::intersectsAnyCube(cs237::AABBd bb)
{
	for(int i = 0; i<this->nCubes; i++){
		if(intersectionTest(this->masterCube->CubeAABB(this->cubePositions[i]), bb)) {
			return true;
		}
	}
	return false;
}