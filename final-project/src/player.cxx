#include "player.hxx"

#define NMESHES 1
#define NCOLORS 2

#define SIZE 0.3
#define STARTINGPOS cs237::vec3f(0.0f, 15.0f, 0.0f)

//========================= CONSTRUCTOR AND DESTRUCTOR =========================//

Player::Player(Sunlight sun)
{
	//start at the origin
	this->pos = STARTINGPOS;

	//use the 0th mesh and color
	curMesh = 0;
	curColor = 0;

	//set how many meshes and colors we have
	nMeshes = NMESHES;
	nColors = NCOLORS;

	//initalize the mesh array
	playerMeshes = new Cube *[NMESHES];
	for(int i = 0; i<NMESHES; i++){
		playerMeshes[i] = new Cube(sun, SIZE, SIZE);
	}

	//initalize the colors
	playerColors = new cs237::color4f[NCOLORS];
	playerColors[0] = cs237::color4f(0.2f, 1.0f, 0.3f, 1.0f); 
	playerColors[1] = cs237::color4f(0.9f, 0.3f, 0.3f, 1.0f);

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
	this->pos = v;
	this->updateAABB();
}

cs237::vec3f Player::addToZPos(float z)
{
	this->pos = cs237::vec3f(this->pos[0], this->pos[1], this->pos[2] + z);
	this->updateAABB();
	return this->getPos();
}

cs237::vec3f Player::updateXPos(Camera c)
{
	this->pos = cs237::vec3f(c.position()[0], this->pos[1], this->pos[2]);
	this->updateAABB();
	return this->getPos();
}

Cube* Player::getMeshInUse()
{
	return this->playerMeshes[this->curMesh];
}

void Player::setMeshInUse(int n)
{
	if(n < NMESHES){
		this->curMesh = n;
	}
}

cs237::color4f Player::getColorInUse()
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

void Player::reset()
{
	this->setPos(STARTINGPOS);
	this->setColorInUse(0);
}


//========================= UPDATE BOUNDING BOX =========================//

void Player::updateAABB()
{
	//update the AABB based on the position
	//printf("player position: %f, %f, %f\n", this->pos.x, this->pos.y, this->pos.z);
	this->bb = cs237::AABBd(cs237::toDouble(this->pos - cs237::vec3f(SIZE, SIZE, SIZE)), 
		cs237::toDouble(this->pos + cs237::vec3f(SIZE, SIZE, SIZE)));
}


//========================= RENDERING =========================//

void Player::Render(Camera c)
{
	Cube *curcube = this->getMeshInUse();

	curcube->Render(this->getPos(), this->getColorInUse(),c.projTransform(), c.ModelViewMatrix(), 
		false, cs237::color3f(0.0, 0.0, 0.0), 0.0);
}