#include "Block.h"



void Block::DrawTexture(GLuint texId)
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   // Texture value should be multiplied to the current color
	glColor4d(1.0, 1.0, 1.0, 1.0);           	// Current color is solid white

	glEnable(GL_TEXTURE_2D);		// Turn on the texture mapping 
	glBindTexture(GL_TEXTURE_2D, texId);	// Select the current texture.

	glBegin(GL_QUADS);

	if (sideVisible[0])
	{
		glTexCoord2d(0.0, 0.0);
		glVertex3i(pos[0], pos[1], pos[2]);
		glTexCoord2d(1.0, 0.0);
		glVertex3i(pos[0], pos[1], pos[2] + l);
		glTexCoord2d(1.0, 1.0);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
		glTexCoord2d(0.0, 1.0);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
	}
	// Right Side
	if (sideVisible[1])
	{
		glTexCoord2d(0.0, 0.0);
		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glTexCoord2d(1.0, 0.0);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glTexCoord2d(1.0, 1.0);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glTexCoord2d(0.0, 1.0);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
	}
	// Back Side
	if (sideVisible[2])
	{
		glTexCoord2d(0.0, 0.0);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
		glTexCoord2d(1.0, 0.0);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
		glTexCoord2d(1.0, 1.0);
		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glTexCoord2d(0.0, 1.0);
		glVertex3i(pos[0], pos[1], pos[2]);
	}
	// Front Side
	if (sideVisible[3])
	{
		glTexCoord2d(0.0, 0.0);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
		glTexCoord2d(1.0, 0.0);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glTexCoord2d(1.0, 1.0);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glTexCoord2d(0.0, 1.0);
		glVertex3i(pos[0], pos[1], pos[2] + l);
	}

	// Bottom Side
	if (sideVisible[4])
	{
		glTexCoord2d(0.0, 0.0);
		glVertex3i(pos[0], pos[1], pos[2]);
		glTexCoord2d(1.0, 0.0);
		glVertex3i(pos[0] + w, pos[1], pos[2]);
		glTexCoord2d(1.0, 1.0);
		glVertex3i(pos[0] + w, pos[1], pos[2] + l);
		glTexCoord2d(0.0, 1.0);
		glVertex3i(pos[0], pos[1], pos[2] + l);
	}

	// Top Side
	if (sideVisible[5])
	{
		glTexCoord2d(0.0, 0.0);
		glVertex3i(pos[0], pos[1] + h, pos[2]);
		glTexCoord2d(1.0, 0.0);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2]);
		glTexCoord2d(1.0, 1.0);
		glVertex3i(pos[0] + w, pos[1] + h, pos[2] + l);
		glTexCoord2d(0.0, 1.0);
		glVertex3i(pos[0], pos[1] + h, pos[2] + l);
	}

	glEnd();
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

Block::Block()
{
	roomSize = 10;
	blockSize = 8;
	int xRand = rand() % roomSize;
	int yRand = rand() % roomSize;
	int zRand = rand() % roomSize;
	setCoordinate(xRand, yRand, zRand);
	setColor(255, 0, 0);
	setDimension(blockSize, blockSize, blockSize);
	pos[0] = (double)getX()*blockSize; pos[1] = (double)getY()*blockSize; pos[2] = (double)getZ()*blockSize;
	index = xRand + roomSize*zRand + pow(roomSize, 2)*yRand;
}

void Block::copyFrom(const Block &fromBlock)
{
	roomSize = fromBlock.roomSize;
	blockSize = 8;
	setCoordinate(fromBlock.x, fromBlock.y, fromBlock.z);
	setColor(255, 0, 0);
	setDimension(blockSize, blockSize, blockSize);
	pos[0] = (double)getX()*blockSize; pos[1] = (double)getY()*blockSize; pos[2] = (double)getZ()*blockSize;
	index = getX() + roomSize*getZ() + pow(roomSize, 2)*getY();
}

Block::Block(const std::unique_ptr<Block> &fromPtr)
{
	copyFrom(*fromPtr);
}

Block &Block::operator=(const std::unique_ptr<Block> &fromPtr)
{
	copyFrom(*fromPtr);
	return *this;
}

Block::Block(int roomSizeIn)
{
	roomSize = roomSizeIn;
	blockSize = 8;
	int xRand = rand() % roomSize;
	int yRand = rand() % roomSize;
	int zRand = rand() % roomSize;
	setCoordinate(xRand, yRand, zRand);
	setColor(255, 0, 0);
	setDimension(blockSize, blockSize, blockSize);
	pos[0] = (double)getX()*blockSize; pos[1] = (double)getY()*blockSize; pos[2] = (double)getZ()*blockSize;
	index = xRand + roomSize*zRand + pow(roomSize, 2)*yRand;
}

Block::Block(int roomSizeIn, int x, int y, int z)
{
	Initialize(roomSizeIn, x, y, z);
}

void Block::Initialize(int roomSizeIn, int x, int y, int z)
{

	roomSize = roomSizeIn;
	blockSize = 8;
	setCoordinate(x, y, z);
	setColor(255, 0, 0);
	setDimension(blockSize, blockSize, blockSize);
	index = x + roomSize*z + pow(roomSize, 2)*y;
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
	pos[0] = (x)*blockSize;
	pos[1] = (y)*blockSize;
	pos[2] = (z)*blockSize;
	index = x + roomSize*z + pow(roomSize, 2)*y;
}

void Block::setPosition(double xLoc, double yLoc, double zLoc)
{
	pos[0] = xLoc;
	pos[1] = yLoc;
	pos[2] = zLoc;
	x = (int)(xLoc / blockSize);
	y = (int)(yLoc / blockSize);
	z = (int)(zLoc / blockSize);
	index = x + roomSize*z + pow(roomSize, 2)*y;
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
