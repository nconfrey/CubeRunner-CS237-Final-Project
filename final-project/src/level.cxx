#include "level.hxx"

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//
Level::Level(int difficulty, int levelNum, float zstart, float zend, float scoreMult, float velocity,
			  cs237::color3f * palletteColors, int nColors)
{
	this->difficulty = difficulty;
	this->levelNum = levelNum;
	this->zstart = zstart;
	this->zend = zend;
	this->scoreMult = scoreMult;
	this->velocity = velocity;
	this->nColors = nColors;
	this->palletteColors = new cs237::color3f[nColors];
	for(int i = 0; i<nColors; i++){
		this->palletteColors[i] = palletteColors[i];
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

void Level::RenderAllCubes()
{
	for(int i = 0; i<this->nCubes; i++){
		//masterCube->Render(this->cubePositions[i], this->getColorAt(BOXCOLORSTART));
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