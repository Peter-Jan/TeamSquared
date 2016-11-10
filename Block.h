#include <vector>
#ifndef BLOCK_H
#define BLOCK_H


const int roomSize = 100;
const int blockCount = roomSize * roomSize;

class Block
{
public:
	Block();
	Block::Block(int x, int y, int z);
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
	double blockSize = 8;
	int index = 0;
	int sideVisible[6] = { 1,1,1,1,1,1 };
	std::vector<double> pos = { 0,0,0 };
};

#endif