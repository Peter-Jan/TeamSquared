#include "Block.h"

Block::Block()
{
	quantity = 0;
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
	quantity = fromBlock.quantity;
	health = fromBlock.health;
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
	printf("Location = %d %d %d\n", x, y, z);
	Initialize(roomSizeIn, x, y, z);
}

void Block::Initialize(int roomSizeIn, int x, int y, int z)
{
	roomSize = roomSizeIn;
	blockSize = 8;
	setCoordinate(x, y, z);
	r = 255; g = 255; b = 255;
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

int Block::getR() const
{
	return r;
}
int Block::getG() const
{
	return g;
}
int Block::getB() const
{
	return b;
}

void Block::setCoordinate(int xin, int yin, int zin)
{
	x = xin; y = yin; z = zin;
 	pos[0] = (x)*blockSize; pos[1] = (y)*blockSize; pos[2] = (z)*blockSize;
 	xM = pos[0] + blockSize / 2; yM = pos[1] + blockSize / 2; zM = pos[2] + blockSize / 2;
 	x1 = pos[0] + blockSize;     y1 = pos[1] + blockSize;     z1 = pos[2] + blockSize;
 	centerPos[0] = xM; centerPos[1] = yM; centerPos[2] = zM;
	index = x + roomSize*z + pow(roomSize, 2)*y;
}

void Block::setTexture(int x,int y)
{
	textMapX = x;
	textMapY = y;
}
void Block::setPosition(double xLoc, double yLoc, double zLoc)
{
	pos[0] = xLoc;
	pos[1] = yLoc;
	pos[2] = zLoc;
	x = (int)(xLoc / blockSize);
	y = (int)(yLoc / blockSize);
	z = (int)(zLoc / blockSize);
	xM = pos[0] + blockSize / 2; yM = pos[1] + blockSize / 2; zM = pos[2] + blockSize / 2;
 	x1 = pos[0] + blockSize;     y1 = pos[1] + blockSize;     z1 = pos[2] + blockSize;
 	centerPos[0] = xM; centerPos[1] = yM; centerPos[2] = zM;
	index = x + roomSize*z + pow(roomSize, 2)*y;
}

void Block::setColor(const int rin, const int gin, const int bin)
{
	r = rin;
	g = gin;
	b = bin;
}

double Block::BlockDist(const Block &otherBlock)
{
	return abs(pos[0] - otherBlock.pos[0]) + abs(pos[1] - otherBlock.pos[1]) + abs(pos[2] - otherBlock.pos[2]);
}

void Block::DrawSolid(void)
{
	// Left Side
	if (sideVisible[0])
	{
		glColor3ub(255, 0, 0);
		glVertex3d(pos[0], pos[1], pos[2]);
		glVertex3d(pos[0], y1, pos[2]);
		glVertex3d(pos[0], y1, z1);
		glVertex3d(pos[0], pos[1], z1);
	}
	// Right Side
	if (sideVisible[1])
	{
		glColor3ub(100, 0, 0);
		glVertex3d(x1, pos[1], z1);
		glVertex3d(x1, y1, z1);
		glVertex3d(x1, y1, pos[2]);
		glVertex3d(x1, pos[1], pos[2]);
	}
	// Back Side
	if (sideVisible[2])
	{
		glColor3ub(0, 255, 0);
		glVertex3d(x1, pos[1], pos[2]);
		glVertex3d(x1, y1, pos[2]);
		glVertex3d(pos[0], y1, pos[2]);
		glVertex3d(pos[0], pos[1], pos[2]);
	}
	// Front Side
	if (sideVisible[3])
	{
		glColor3ub(0, 100, 0);
		glVertex3d(pos[0], pos[1], z1);
		glVertex3d(pos[0], y1, z1);
		glVertex3d(x1, y1, z1);
		glVertex3d(x1, pos[1], z1);
	}

	// Bottom Side
	if (sideVisible[4])
	{
		glColor3ub(0, 0, 255);
		glVertex3d(pos[0], pos[1], pos[2]);
		glVertex3d(pos[0], pos[1], z1);
		glVertex3d(x1, pos[1], z1);
		glVertex3d(x1, pos[1], pos[2]);
	}

	// Top Side
	if (sideVisible[5])
	{
		glColor3ub(0, 0, 100);
		glVertex3d(pos[0], y1, z1);
		glVertex3d(pos[0], y1, pos[2]);
		glVertex3d(x1, y1, pos[2]);
		glVertex3d(x1, y1, z1);
	}
}

void Block::DrawTexture(GLuint texId,double imageX,double imageY)
{

	glColor3d(health / (double)healthParam, health/(double)healthParam, health/ (double)healthParam);           	// Current color is solid white

	//Left side

	if (sideVisible[0])
	{
		glTexCoord2d((imageX)/6, (imageY+1)/texRows);
		glVertex3d(pos[0], pos[1], pos[2]);
		glTexCoord2d((imageX)/6, (imageY ) / texRows);
		glVertex3d(pos[0], y1, pos[2]);
		glTexCoord2d((imageX+1)/6, (imageY ) / texRows);
		glVertex3d(pos[0], y1, z1);
		glTexCoord2d((imageX+1)/6, (imageY + 1)/texRows);
		glVertex3d(pos[0], pos[1], z1);
	}

	// Right Side
	if (sideVisible[1])   
	{
		glTexCoord2d((imageX+1)/6, (imageY+1) / texRows);
		glVertex3d(x1, pos[1], z1);
		glTexCoord2d((imageX+1)/6, (imageY) / texRows);
		glVertex3d(x1, y1, z1);
		glTexCoord2d(imageX/6, (imageY) / texRows);
		glVertex3d(x1, y1, pos[2]);
		glTexCoord2d(imageX/6, (imageY+1) / texRows);
		glVertex3d(x1, pos[1], pos[2]);
	}
	// Back Side
	if (sideVisible[2])
	{
		glTexCoord2d(imageX/6, (imageY+1) / texRows);
		glVertex3d(x1, pos[1], pos[2]);
		glTexCoord2d(imageX/6, (imageY) / texRows);
		glVertex3d(x1, y1, pos[2]);
		glTexCoord2d((imageX+1)/6, (imageY) / texRows);
		glVertex3d(pos[0], y1, pos[2]);
		glTexCoord2d((imageX+1)/6, (imageY+1) / texRows);
		glVertex3d(pos[0], pos[1], pos[2]);

	}
	// Front Side
	if (sideVisible[3])
	{
		glTexCoord2d(imageX/6, (imageY+1) / texRows);
		glVertex3d(pos[0], pos[1], z1);
		glTexCoord2d(imageX/6, (imageY) / texRows);
		glVertex3d(pos[0], y1, z1);
		glTexCoord2d((imageX+1)/6, (imageY) / texRows);
		glVertex3d(x1, y1, z1);
		glTexCoord2d((imageX+1)/6, (imageY+1) / texRows);
		glVertex3d(x1, pos[1], z1);
	}
	// Bottom Side
	if (sideVisible[4])
	{
		glTexCoord2d(imageX/6, (imageY+1) / texRows);
		glVertex3d(pos[0], pos[1], pos[2]);
		glTexCoord2d(imageX/6, (imageY) / texRows);
		glVertex3d(pos[0], pos[1], z1);
		glTexCoord2d((imageX+1)/6, (imageY) / texRows);
		glVertex3d(x1, pos[1], z1);
		glTexCoord2d((imageX+1)/6, (imageY+1) / texRows);
		glVertex3d(x1, pos[1], pos[2]);
	}

	// Top Side
	if (sideVisible[5])
	{
		if (imageX == 0 && imageY == 0)
		{
			glTexCoord2d(imageX / 6, 0.666);
			glVertex3d(pos[0], y1, z1);
			glTexCoord2d(imageX / 6, 0.333);
			glVertex3d(pos[0], y1, pos[2]);
			glTexCoord2d((imageX + 1) / 6, 0.333);
			glVertex3d(x1, y1, pos[2]);
			glTexCoord2d((imageX + 1) / 6, 0.666);
			glVertex3d(x1, y1, z1);
		}
		else
		{
			glTexCoord2d(imageX/6, (imageY+1) / texRows);
			glVertex3d(pos[0], y1, z1);
			glTexCoord2d(imageX/6, (imageY) / texRows);
			glVertex3d(pos[0], y1, pos[2]);
			glTexCoord2d((imageX+1)/6, (imageY) / texRows);
			glVertex3d(x1, y1, pos[2]);
			glTexCoord2d((imageX+1)/6, (imageY+1) / texRows);
			glVertex3d(x1, y1, z1);

		}


	}
	//glEnd();
}

void Block::DrawEdges(void)
{
	//Draw edges
	// Left Side
	glColor3ub(0, 0, 0);
	if (sideVisible[0])
	{
		glVertex3d(pos[0], pos[1], pos[2]);
		glVertex3d(pos[0], pos[1], z1);

		glVertex3d(pos[0], pos[1], z1);
		glVertex3d(pos[0], y1, z1);

		glVertex3d(pos[0], y1, z1);
		glVertex3d(pos[0], y1, pos[2]);

		glVertex3d(pos[0], y1, pos[2]);
		glVertex3d(pos[0], pos[1], pos[2]);
	}
	// Right Side
	if (sideVisible[1])
	{
		glVertex3d(x1, pos[1], pos[2]);
		glVertex3d(x1, pos[1], z1);

		glVertex3d(x1, pos[1], z1);
		glVertex3d(x1, y1, z1);

		glVertex3d(x1, y1, z1);
		glVertex3d(x1, y1, pos[2]);

		glVertex3d(x1, y1, pos[2]);
		glVertex3d(x1, pos[1], pos[2]);
	}
	// Back Side
	if (sideVisible[2])
	{
		glVertex3d(pos[0], y1, pos[2]);
		glVertex3d(x1, y1, pos[2]);

		glVertex3d(x1, y1, pos[2]);
		glVertex3d(x1, pos[1], pos[2]);

		glVertex3d(x1, pos[1], pos[2]);
		glVertex3d(pos[0], pos[1], pos[2]);

		glVertex3d(pos[0], pos[1], pos[2]);
		glVertex3d(pos[0], y1, pos[2]);
	}
	// Front Side
	if (sideVisible[3])
	{
		glVertex3d(pos[0], y1, z1);
		glVertex3d(x1, y1, z1);

		glVertex3d(x1, y1, z1);
		glVertex3d(x1, pos[1], z1);

		glVertex3d(x1, pos[1], z1);
		glVertex3d(pos[0], pos[1], z1);

		glVertex3d(pos[0], pos[1], z1);
		glVertex3d(pos[0], y1, z1);
	}

	// Bottom Side
	if (sideVisible[4])
	{
		glVertex3d(pos[0], pos[1], pos[2]);
		glVertex3d(x1, pos[1], pos[2]);

		glVertex3d(x1, pos[1], pos[2]);
		glVertex3d(x1, pos[1], z1);

		glVertex3d(x1, pos[1], z1);
		glVertex3d(pos[0], pos[1], z1);

		glVertex3d(pos[0], pos[1], z1);
		glVertex3d(pos[0], pos[1], pos[2]);
	}
	// Top Side
	if (sideVisible[5])
	{
		glVertex3d(pos[0], y1, pos[2]);
		glVertex3d(x1, y1, pos[2]);

		glVertex3d(x1, y1, pos[2]);
		glVertex3d(x1, y1, z1);

		glVertex3d(x1, y1, z1);
		glVertex3d(pos[0], y1, z1);

		glVertex3d(pos[0], y1, z1);
		glVertex3d(pos[0], y1, pos[2]);
	}
}
