#include "fssimplewindow.h"
#include "Block.h"



Block::Block()
{
	blockSize = 8;
	int xRand = rand() % roomSize;// -roomSize / 2;
	int yRand = rand() % roomSize;// -roomSize / 2;
	int zRand = rand() % roomSize;// -roomSize / 2;
	setCoordinate(xRand - roomSize / 2, yRand - roomSize / 2, zRand - roomSize / 2);
	setColor(255, 0, 0);
	setDimension(blockSize, blockSize, blockSize);
	pos[0] = (double)getX()*blockSize; pos[1] = (double)getY()*blockSize; pos[2] = (double)getZ()*blockSize;
	index = xRand + roomSize*zRand + roomSize*roomSize*yRand;
}

Block::Block(int x, int y, int z)
{
	blockSize = 8;
	setCoordinate(x, y, z);
	setColor(255, 0, 0);
	setDimension(blockSize, blockSize, blockSize);
	index = x + roomSize*z + roomSize*roomSize*y;
}

Block::~Block()
{
}

void Block::setDimension(const int win, const int hin, const int lin)
{
	w = win;
	h = hin;
	l = lin;
}

int Block::getX() const
{
	return x;
}

int Block::getY() const
{
	return y;
}

int Block::getZ() const
{
	return z;
}

void Block::setCoordinate(int xin, int yin, int zin)
{
	x = xin;
	y = yin;
	z = zin;
	pos[0] = (x - roomSize / 2)*blockSize;
	pos[1] = (y - roomSize / 2)*blockSize;
	pos[2] = (z - roomSize / 2)*blockSize;
}

void Block::setPosition(double xLoc, double yLoc, double zLoc)
{
	pos[0] = xLoc;
	pos[1] = yLoc;
	pos[2] = zLoc;
	x = (int)(xLoc / blockSize) + roomSize;
	y = (int)(yLoc / blockSize) + roomSize;
	z = (int)(zLoc / blockSize) + roomSize;
}

void Block::setColor(const int rin, const int gin, const int bin)
{
	r = rin;
	g = gin;
	b = bin;
}

void Block::DrawSolid(void)
{
	// Draw Cube

	glColor3ub(r, g, b);

	// Left Side
	if (sideVisible[0])
	{
		glColor3ub(255, 0, 0);
		glVertex3i(pos[0], pos[1], pos[2]);
		glVertex3i(pos[0], pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
	}
	// Right Side
	if (sideVisible[1])
	{
		glColor3ub(100, 0, 0);
		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
	}
	// Back Side
	if (sideVisible[2])
	{
		glColor3ub(0, 255, 0);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glVertex3i(pos[0], pos[1], pos[2]);
	}
	// Front Side
	if (sideVisible[3])
	{
		glColor3ub(0, 100, 0);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1], pos[2] + l);
	}

	// Bottom Side
	if (sideVisible[4])
	{
		glColor3ub(0, 0, 255);
		glVertex3i(pos[0], pos[1], pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1], pos[2] + l);
	}
	// Top Side
	if (sideVisible[5])
	{
		glColor3ub(0, 0, 100);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
	}
}

void Block::DrawEdges(void)
{
	//Draw edges
	// Left Side
	glColor3ub(0, 0, 0);
	if (sideVisible[0])
	{
		glVertex3i(pos[0], pos[1], pos[2]);
		glVertex3i(pos[0], pos[1], pos[2] + l);

		glVertex3i(pos[0], pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);

		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2]);

		glVertex3i(pos[0], pos[1] + h, pos[2]);
		glVertex3i(pos[0], pos[1], pos[2]);
	}
	// Right Side
	if (sideVisible[1])
	{
		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);

		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);

		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);

		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2]);
	}
	// Back Side
	if (sideVisible[2])
	{
		glVertex3i(pos[0], pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);

		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2]);

		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glVertex3i(pos[0], pos[1], pos[2]);

		glVertex3i(pos[0], pos[1], pos[2]);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
	}
	// Front Side
	if (sideVisible[3])
	{
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);

		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);

		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1], pos[2] + l);

		glVertex3i(pos[0], pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
	}

	// Bottom Side
	if (sideVisible[4])
	{
		glVertex3i(pos[0], pos[1], pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2]);

		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);

		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1], pos[2] + l);

		glVertex3i(pos[0], pos[1], pos[2] + l);
		glVertex3i(pos[0], pos[1], pos[2]);
	}
	// Top Side
	if (sideVisible[5])
	{
		glVertex3i(pos[0], pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);

		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);

		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);

		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
	}
}