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

int enemy::xGrid(void)
{
	return gridLocation[0];
}

int enemy::yGrid(void)
{
	return gridLocation[1];
}

int enemy::zGrid(void)
{
	return gridLocation[2];
}

void enemy::initialize(int roomSizeIn, double startX, double startY, double startZ)
{
	roomSize = roomSizeIn;
	SetVec(pos, startX, startY, startZ);
	//printf("xG: %d, yG: %d, zG: %d\n", gridLocation[0], gridLocation[1], gridLocation[2]);
	//printf("xG: %lf, yG: %lf, zG: %lf\n", startX, startY, startZ);

	SetGridLocation();
	frenemy.setCoordinate(gridLocation[0], gridLocation[1], gridLocation[2]);		//takes grid coordinates
	dxMove = 0; dyMove = 0; dzMove = 0;
	blockSize = 8;
	//printf("FRENEMY x: %lf, y: %lf, z: %lf\n", frenemy.pos[0], frenemy.pos[1], frenemy.pos[2]);


}

void inline enemy::SetGridLocation(void)
{
	gridLocation[0] = posM[0] / blockSize;
	gridLocation[1] = posM[1] / blockSize;
	gridLocation[2] = posM[2] / blockSize;

}

void enemy::setPosM(void)
{	
	posM[0] = pos[0] + blockSize / 2;
	posM[1] = pos[1];
	posM[2] = pos[2] + blockSize / 2;

}
void enemy::setPos()
{
	pos[0] = posM[0] - blockSize / 2;
	pos[2] = posM[2] - blockSize / 2;
	pos[1] = posM[1];
}


void enemy::drawEnemy(void)
{
	//printf("DRAWING\n");
	frenemy.setColor(255, 255, 255);
	glBegin(GL_QUADS);
	frenemy.DrawTexture((GLuint)1,2.0,1.0);
	glEnd();
	//glBegin(GL_QUADS);
	//frenemy.DrawEdges();
	//glEnd();
}

int enemy::chase(CameraObject player, std::map<int, std::unique_ptr<Block>> &blockMap)
{
	double magnitude=0;
	double scale = 1.0;

	magnitude = sqrt(pow(player.x() - pos[0], 2) + pow(player.y() - pos[1], 2) + pow(player.z() - pos[2], 2));
	chaseVec[0] = (player.x() - (pos[0]+blockSize/2))/(scale*magnitude);
	chaseVec[1] = (player.y() - (pos[1]+(blockSize / 2)))/ (scale*magnitude);
	chaseVec[2] = (player.z() - (pos[2]+(blockSize/2)))/ (scale*magnitude);

	setPosM();
	SetGridLocation();
	//printf("CHASEVEC: x: %lf,y: %lf,z: %lf \n", chaseVec[0],chaseVec[1],chaseVec[2]);
	//printf("ENEMY: x: %lf,y: %lf,z: %lf \n", posM[0], posM[1], posM[2]);
	//printf("ENEMY GRID: x: %d,y: %d,z: %d \n\n", xGrid(), yGrid(), zGrid());
	//printf("PLAYER: x: %lf,y: %lf,z: %lf \n", player.x(), player.y(), player.z());
	//printf("PLAYER GRID: x: %d,y: %d,z: %d \n\n", player.xGrid(), player.yGrid(), player.zGrid());
	frenemy.setPosition(pos[0], pos[1], pos[2]);
	hitCheck(player,blockMap, chaseVec);
	if (player.xGrid() == xGrid() && player.yGrid() == yGrid() && player.zGrid() == zGrid())
	{
		return 1;
	}

	return 0;
}


void enemy::hitCheck(CameraObject player,std::map<int, std::unique_ptr<Block>> &blockMap, std::vector<double> chaseVec)
{
	double px0, px1, pz0, pz1, py0, py1;
	int cx0, cy0, cz0, checkX, checkY, checkZ;

	setPos();
	dxMove = chaseVec[0];
	dzMove = chaseVec[2];

	px1 = posM[0] + chaseVec[0]; // centerblock after move
	pz1 = posM[2] + chaseVec[2];

	cx0 = ((int)posM[0] % blockSize) / (blockSize / 4);
	cz0 = ((int)posM[2] % blockSize) / (blockSize / 4);

	checkX = ((int)px1%blockSize) / (blockSize / 4);
	checkZ = ((int)pz1%blockSize) / (blockSize / 4);

	if (dxMove != 0 && cx0 != checkX && checkX == 0 || checkX == 3)
	{
		int xIdx = xGrid() + (dxMove < 0 ? -1 : 1);
		index = xIdx + zGrid()*roomSize + yGrid()*pow(roomSize, 2);
		if (xIdx >= 0 && xIdx < roomSize && blockMap.find(index) == blockMap.end()) // crouch check needs to be added
		{
			posM[0] += dxMove;
		}
		else
		{
            vertVel= .3;//player.jumpVel;
			if (dxMove > 0)
			{
				posM[0] = xGrid()*blockSize + blockSize*3/4;
			}
			else
			{

				posM[0] = (xGrid()+1)*blockSize-blockSize*3/4;

			}
		}
	}
	else
	{
		posM[0] += dxMove;
	}

	if (dzMove != 0 && cz0 != checkZ && checkZ == 0 || checkZ == 3) // check Z direction
	{
		int zIdx = zGrid() + (dzMove < 0 ? -1 : 1);
		index = xGrid() + zIdx*roomSize + yGrid()*pow(roomSize, 2);
		if (zIdx >= 0 && zIdx < roomSize && blockMap.find(index) == blockMap.end())
		{
			posM[2] += dzMove;
		}
		else
		{
            vertVel= .3; //player.jumpVel;
			if (dzMove > 0)
			{
				posM[2] = zGrid()*blockSize + blockSize*3/4;

			}
			else

			{
				posM[2] = (zGrid()+1)*blockSize-blockSize*3/4;
			}
		}
	}
	else
	{
		posM[2] += dzMove;
	}
	setPos();
	SetGridLocation();
    vertVel += GRAV;
    
    //falling down to ground
    pos[1] += vertVel;
    int yIdx = yGrid() + (vertVel < 0 ? -1 : 1);
    
	index = xGrid() + zGrid()*roomSize + yIdx*pow(roomSize, 2);

    
       	if (blockMap.find(index) != blockMap.end()) // hit floor
        {
            vertVel = 0;
            pos[1] = yGrid()*blockSize;
        }

}

