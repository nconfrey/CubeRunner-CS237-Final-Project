#include "player.hxx"

#define NMESHES 1
#define NCOLORS 1

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//

Player::Player()
{
	//start at the origin
	this->pos = cs237::vec3f(0.0f, 0.0f, 0.0f);

	//use the 0th mesh and color
	curMesh = 0;
	curColor = 0;

	//set how many meshes and colors we have
	nMeshes = NMESHES;
	nColors = NCOLORS;

	//initalize the mesh array
	playerMeshes = new Mesh *[NMESHES];
	for(int i = 0; i<NMESHES; i++){
		//some kind of intialization code for the meshes
	}

	//initalize the colors
	playerColors = new cs237::color3f[NCOLORS];
	playerColors[0] = cs237::color3f(0.2f, 0.6f, 1.0f); 

	//init the aabb
	this->bb = cs237::AABBd(cs237::toDouble(this->pos));
}

Player::~Player()
{
	//delete the meshes?
}


//========================= MEMBER ACCESS AND SETTING FUNCTIONS =========================//
cs237::vec3f Player::getPos()
{
	return this->pos;
}

void Player::setPos(cs237::vec3f v)
{
	this->pos = pos;
	this->updateAABB();
}

cs237::vec3f Player::addToZPos(float z)
{
	this->pos = cs237::vec3f(this->pos[0], this->pos[1], this->pos[2] + z);
	this->updateAABB();
	return this->getPos();
}

Mesh* Player::getMeshInUse()
{
	return this->playerMeshes[this->curMesh];
}

void Player::setMeshInUse(int n)
{
	if(n < NMESHES){
		this->curMesh = n;
	}
}

cs237::color3f Player::getColorInUse()
{
	return this->playerColors[this->curColor];
}

void Player::setColorInUse(int n)
{
	if(n < NCOLORS){
		this->curColor = n;
	}
}

cs237::AABBd Player::getAABB()
{
	return this->bb;
}


//========================= UPDATE BOUNDING BOX =========================//

void Player::updateAABB()
{
	//update the AABB based on the position
	printf("player position: %f, %f, %f\n", this->pos.x, this->pos.y, this->pos.z);
	this->bb = cs237::AABBd(cs237::toDouble(this->pos - cs237::vec3f(1.0, 1.0, 1.0)), cs237::toDouble(this->pos + cs237::vec3f(1.0, 1.0, 1.0)));
}


//========================= RENDERING =========================//

void Player::Render()
{
	//not gonna do this rn
}