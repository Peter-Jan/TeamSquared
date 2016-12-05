#ifndef ENEMY
#define ENEMY
#include "MiscFunctions.h"
#include "CameraObject.h"
#include <time.h>


class enemy
{
public:
    double scale = 2;
	int health = 0;
	int damage = 0;
	double vertVel = 0;
	double xVel = 0;
	double yVel = 0;
	int roomSize = 0;
	int index = 0;
    int planJump =0;
    int jumps = 0;
	double jumpVel = sqrt(3.0 * (double)blockSize*-GRAV);
	int hitPlayer = 0;
	double dxMove, dyMove, dzMove;
	int blockSize;
	std::vector<double> pos = { 0,0,0 };
	std::vector<double> posM = {((double) blockSize) / 2,((double)blockSize) / 2,((double)blockSize) / 2 };
	std::vector<double> posGrid = { 0,0,0 };
	std::vector<int> gridLocation = { 0,0,0 };
	std::vector<double> chaseVec = { 0,0,0 };
	Block frenemy;

	enemy();
	enemy(int roomSize, double xLoc, double yLoc, double zLoc, int healthIn, int damageIn, int texX, int texY, double speedScaler);
	enemy(int roomSize);
	~enemy();

	void initialize(int roomSize, double startX, double startY, double startZ);
	void drawEnemy(void);
	void inline SetGridLocation(void);
	void setPos(void);
	void setPosM(void);

	double x(void);
	double y(void);
	double z(void);
	int xGrid(void);
	int yGrid(void);
	int zGrid(void);

	int chase(CameraObject player, std::map<int, std::unique_ptr<Block>> &blockMap);
	void hitCheck(CameraObject player,std::map<int, std::unique_ptr<Block>> &blockMap, std::vector<double> curFV);

};










#endif
