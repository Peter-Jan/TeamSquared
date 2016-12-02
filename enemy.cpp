#include "enemy.h"

enemy::enemy()
{
	roomSize=100;
	initialize(roomSize, 0.0,0.0,0.0);
}

enemy::~enemy()
{
	
}

enemy::enemy(int roomSize)
{
	initialize(roomSize, 0.0,0.0,0.0);
}

enemy::enemy(int roomSize,double startX,double startY,double startZ)
{
	initialize(roomSize,startX,startY,startZ);
}

double enemy::x(void)
{
	return pos[0];
}

double enemy::y(void)
{
	return pos[1];
}

double enemy::z(void)
{
	return pos[2];
}

double enemy::xGrid(void)
{
	return gridLocation[0];
}

double enemy::yGrid(void)
{
	return gridLocation[1];
}

double enemy::zGrid(void)
{
	return gridLocation[2];
}

void enemy::initialize(int roomSizeIn, double startX, double startY, double startZ)
{
	roomSize = roomSizeIn;
	SetVec(pos, startX, startY, startZ);
	//printf("xG: %d, yG: %d, zG: %d\n", gridLocation[0], gridLocation[1], gridLocation[2]);
	printf("xG: %lf, yG: %lf, zG: %lf\n", startX, startY, startZ);

	SetGridLocation();
	frenemy.setCoordinate(gridLocation[0], gridLocation[1], gridLocation[2]);		//takes grid coordinates
	dxMove = 0; dyMove = 0; dzMove = 0;
	blockSize = 8;
	printf("FRENEMY x: %lf, y: %lf, z: %lf\n", frenemy.pos[0], frenemy.pos[1], frenemy.pos[2]);


}

void inline enemy::SetGridLocation(void)
{
	gridLocation[0] = pos[0] / blockSize;
	gridLocation[1] = pos[1] / blockSize;
	gridLocation[2] = pos[2] / blockSize;

}

void enemy::drawEnemy(void)
{
	//printf("DRAWING\n");
	glBegin(GL_QUADS);
	frenemy.DrawSolid();
	glEnd();
	glBegin(GL_QUADS);
	frenemy.DrawEdges();
	glEnd();
}

void enemy::chase(CameraObject player)
{
	double chaseVec[3];
	double magnitude=0;

	magnitude = sqrt(pow(player.x() - pos[0], 2) + pow(player.y() - pos[1], 2) + pow(player.z() - pos[2], 2));
	chaseVec[0] = (player.x() - pos[0])/magnitude;
	chaseVec[1] = (player.y() - pos[1])/magnitude;
	chaseVec[2] = (player.z() - pos[2])/magnitude;
	pos[0] += chaseVec[0];
	pos[1] += chaseVec[1];
	pos[2] += chaseVec[2];

	frenemy.setPosition(pos[0], pos[1], pos[2]);


	printf("ENEMY: x: %lf,y: %lf,z: %lf \n", pos[0], pos[1], pos[2]);
	printf("PLAYER: x: %lf,y: %lf,z: %lf \n", player.x(), player.y(),player.z());

}