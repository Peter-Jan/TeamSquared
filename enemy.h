#ifndef ENEMY
#define ENEMY
#include "MiscFunctions.h"
#include "CameraObject.h"

class enemy
{
public:

	int roomSize = 0;
	int index = 0;
	double dxMove, dyMove, dzMove;
	int blockSize;
	std::vector<double> pos = { 0,0,0 };
	std::vector<double> posGrid = { 0,0,0 };
	std::vector<int> gridLocation = { 0,0,0 };
	Block frenemy;

	enemy();
	enemy(int roomSize);
	enemy(int roomSize, double xLoc, double yLoc, double zLoc);
	~enemy();

	void initialize(int roomSize, double startX, double startY, double startZ);
	void drawEnemy(void);
	void inline SetGridLocation(void);

	double x(void);
	double y(void);
	double z(void);
	double xGrid(void);
	double yGrid(void);
	double zGrid(void);

	void chase(CameraObject player);

};










#endif