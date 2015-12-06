#include "level.hxx"

#define yOffset 15
#define SLICE_DENSITY 10 //the number of slices per 1000 z values

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//
Level::Level(int difficulty, int levelNum, float zstart, float zend, float scoreMult, float velocity,
			  cs237::color4f * palletteColors, int nColors, cs237::vec3d playerPos, float width, Sunlight sun)
{
	this->difficulty = difficulty;
	this->levelNum = levelNum;
	this->zstart = zstart;
	this->zend = zend;
	this->width = width;
	this->scoreMult = scoreMult;
	this->velocity = velocity;
	this->nColors = nColors;
	this->masterCube = new Cube(sun); //nick: Should this be here or in view? we can pass it in (only need to make 1 per game)
	this->palletteColors = new cs237::color4f[nColors];
	for(int i = 0; i<nColors; i++){
		this->palletteColors[i] = palletteColors[i];
	}

	//Randomly generate cubes for this level
	this->nCubes = difficulty * 10;
	//this->cubePositions = new cs237::vec3f *[this->nCubes];
	this->cubePositions = this->generateCubePositions(this->nCubes, zstart, zend, width);
}

Level::~Level()
{}


//========================= PROCEDURAL CUBE GENERATION =========================//

//divides the zrange into a number of slices and populates each slice with a number of cubes
//determined by the difficulty
cs237::vec3f ** Level::generateCubePositions(int nCubes, float zstart, float zend, float width)
{

	cs237::vec3f ** cubePositions = new cs237::vec3f *[nCubes];
	//calculate the number of slices and density of those slices
	int nSlices = (SLICE_DENSITY * (zend - zstart)) / 1000;
	int sliceWidth = (zend - zstart) / nSlices;
	int cubesPerSlice = nCubes / nSlices;
	int nCubesAfterSlicing = 0;

	//note this leaves some remainder without any cubes at the end of each level

	//printf("nslices: %d\n", nSlices);
	//printf("slicewidth: %d\n", sliceWidth);
	//printf("cubesperslice : %d\n", cubesPerSlice);

	for(int i = 0; i<nSlices-1; i++)
	{
		//printf("indexing into %d to %d\n", i*cubesPerSlice, (i+1)*cubesPerSlice-1);
		this->generateCubeSlice(cubesPerSlice, i*sliceWidth + zstart, (i+1)*sliceWidth + zstart - 1, width, i*cubesPerSlice, cubePositions);
		nCubesAfterSlicing += cubesPerSlice;
	}

	//handle remainder
	int cubesLeft = nCubes - nCubesAfterSlicing;
	this->generateCubeSlice(cubesLeft, sliceWidth*(nSlices-1) + zstart, zend, width, (nSlices-1)*cubesPerSlice, cubePositions);

	return cubePositions;
}

//populates a given zrange wtih cubes based on the difficulty
void Level::generateCubeSlice(int nCubes, float zstart, float zend, float width, int arrayStart, cs237::vec3f **cubes)
{
	for(int i=arrayStart; i<nCubes+arrayStart; i++)
	{
		//new cubes are placed on the x/z plane
 		float x = rand() % (int)(2.0f * width);
 		x -= width;
      	float z = rand() % (int)(zend - zstart);
      	z += zstart;
      	printf("%d  ",i);
 		cubes[i] = new cs237::vec3f(x, 15, z);
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

void Level::RenderAllCubes(Camera c)
{
	//Check the cubes for any that are behind the camera
	//If they are in front of the player, render
	//Otherwise, generate a new cube at a position ahead of the player
	//std::cout << "RenderAllCubes: " << c.projTransform() << "\n";
	//glDisable(GL_DEPTH_TEST);
	float x,z,r,g,b;
	for(int i = 0; i < this->nCubes; i++){
		if(c.position().z > this->cubePositions[i]->z)
		{
			//printf("RECYCLE CUBE #%d\n", i);
			//randomize the xvalue
			x = rand() % (int)(2.0f * this->width) - this->width;
			//take the z value, wiggle it a little
			//increment it by the length of the field (zend - zstart)
			//translate the z value by that amount
      		z = (((rand() % 100) - 100) / 2) + (zend-zstart);
 			cubePositions[i] = new cs237::vec3f(x, 15, cubePositions[i]->z+z);
		}
		
		masterCube->Render(*(this->cubePositions[i]), this->getColorAt(BOXCOLORSTART), c.projTransform(), c.ModelViewMatrix());
		//printf("rendered cube at %f %f %f\n", this->cubePositions[i]->x, this->cubePositions[i]->y, this->cubePositions[i]->z);
		//masterCube->Render(cs237::vec3f(x,500,z), cs237::color4f(r, g, b, 1.0), c.projTransform(), c.ModelViewMatrix());
	}

}

//========================= COLLISION DETECTION =========================//

bool intersectionTest(cs237::AABBd bb, cs237::AABBd bb2)
{
	std::cout << "Cube pos\n";
	std::cout << bb.center();
	std::cout << "Player pos\n";
	std::cout << bb2.center();
	return bb.includesPt(bb2.center()) || bb2.includesPt(bb.center());
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