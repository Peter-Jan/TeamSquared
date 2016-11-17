#ifndef BLOCK_CLASS_INCLUDED
#define BLOCK_CLASS_INCLUDED

#include <memory>
#include <vector>
#include <stdlib.h>
#include "fssimplewindow.h"
#include <math.h>

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

class Block
{
public:
	int roomSize = 10;
	Block();
	Block(int roomSize);
	Block(int roomSize, int x, int y, int z);
	Block(const std::unique_ptr<Block> &fromPtr);
	Block &operator=(const std::unique_ptr<Block> &fromPtr);
	void Initialize(int roomSize, int x, int y, int z);
	void copyFrom(const Block &fromBlock);

	~Block();
	int getX() const;
	int getY() const;
	int getZ() const;
	void setCoordinate(int xin, int yin, int zin);
	void setPosition(double xLoc, double yLoc, double zLoc);
	void setDimension(const int win, const int hin, const int lin);
	void setColor(const int rin, const int gin, const int bin);
	void DrawSolid(void);
	void DrawEdges(void);

private:
	int x, y, z;
	int r, g, b;
	int w, h, l;
public:
	double x1, xM, y1, yM, z1, zM;
	double blockSize = 8.0;
	int index = 0;
	int sideVisible[6] = { 1,1,1,1,1,1 };
	int renderable = TRUE;
	std::vector<double> pos = { 0,0,0 };
	std::vector<double> centerPos = { 0,0,0 };
};

#endif // !BLOCK_CLASS_INCLUDED
