#include "materialBlock.h"

materialBlock::materialBlock()
{
	initialize();
}

materialBlock::materialBlock(int roomsize, int xIn, int yIn, int zIn, std::vector<int> matVec) : Block(roomsize, xIn, yIn, zIn)
{
	itemCode = matVec[0];
	textMapX = matVec[1];
	textMapY = matVec[2];
	setColor(matVec[3], matVec[4], matVec[5]);
	strength = matVec[6];
	health = matVec[7];
	setParam(matVec[7], matVec[8]);
}

materialBlock::~materialBlock()
{
}

int materialBlock::getImx() const
{
	return imageX;
}

int materialBlock::getImy() const
{
	return imageY;
}

void materialBlock::initialize()
{
	itemCode = 0;
	setImage(0, 0);
	health = 0;
	healthParam = 0;
	quantParam = 0;
	name = "unassigned";
	//printf("X: %d, Y: %d ", textMapX, textMapY);
}

std::string materialBlock::getName() const
{
	return name;
}

void materialBlock::setImage(int inX,int inY)
{
	Block::textMapX = inX;
	Block::textMapY = inY;
}

void materialBlock::setColor(int inR, int inG, int inB)
{
	Block::setColor(inR, inG, inB);
}

void materialBlock::setParam(int inHeal, int inQuant)
{
	healthParam = inHeal;
	quantParam = inQuant;
}

void materialBlock::setName(std::string inStr)
{
	name = inStr;
}

void materialBlock::setXYZ(double x, double y, double z)
{
	Block::setCoordinate(x, y, z);
}

int materialBlock::TakeDamage(int wepStr, int wepDmg)
{
	if (wepStr >= strength)
	{
		health -= wepDmg;
		return health;
	}
}