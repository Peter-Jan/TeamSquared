#include "materialBlock.h"

materialBlock::materialBlock()
{
	initialize();
}

materialBlock::materialBlock(int roomsize, int x, int y, int z)
{
	Block::Block(roomsize, x, y, z);
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