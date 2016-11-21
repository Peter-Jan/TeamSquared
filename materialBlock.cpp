#include "materialBlock.h"

materialBlock::materialBlock()
{
	initialize();
}

materialBlock::~materialBlock()
{

}

void materialBlock::initialize()
{
	setImage(4, 0);
	health = 0;
	healthParam = 0;
	quantParam = 0;
	name = "unassigned";
	//printf("X: %d, Y: %d ", textMapX, textMapY);
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