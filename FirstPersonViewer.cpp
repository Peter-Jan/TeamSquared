#include <stdio.h>
#include <math.h>
#include <random>
#include <vector>
#include <memory>
#include "fssimplewindow.h"

#define NULLINT -1

const double YsPi = 3.1415927;
const int WINWID = 800, WINHEI = 600;
const double GRAV = -.09;

std::vector<double> dist = { 0,0,0 }, orthVec = { 0,0,0 }, fVec = { 0,0,0 };
double coneDist, coneRad, orthDist;
const int roomSize = 100;
const int blockCount = roomSize * roomSize;

void DrawGroundLattice(void)
{
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);

	for (int i = -512; i <= 512; i += 8)
	{
		glVertex3i(i, 0, -512);
		glVertex3i(i, 0, +512);

		glVertex3i(-512, 0, i);
		glVertex3i(+512, 0, i);
	}
	glColor3ub(255, 0, 0);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 100, 0);

	glEnd();
}

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
	pos[0] = (x - roomSize/2)*blockSize;
	pos[1] = (y - roomSize/2)*blockSize;
	pos[2] = (z - roomSize/2)*blockSize;
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

class CameraObject
{
public:
	double x, y, z;
	double h, p, b;
	double px, py, pz, pvy;
	double vx, vy, vz;
	double camHeight;
	double zoom;
	std::vector<double> forwardVector = {0,0,0};
	int cursorLock = 0;
	int lb, mb, rb, mx, my, mouseEvent, key, wid, hei, winx0, winy0;

	double fov, nearZ, farZ;
	double sensitivity;
	CameraObject();
	CameraObject(double xLoc, double yLoc, double zLoc);

	void Initialize(void);
	void SetUpCameraProjection(void);
	void SetUpCameraTransformation(void);
	void DrawCamera(void);
	Block playerBlock;

	std::vector<double> GetForwardVector(double &vx, double &vy, double &vz);
	void Update(int key);
};

CameraObject::CameraObject()
{
	Initialize();
}

CameraObject::CameraObject(double xLoc, double yLoc, double zLoc)
{
	x = 0;
	y = 16;
	z = 0;
	h = 0;
	p = 0;
	b = 0;
	camHeight = 16;
	py = camHeight;
	pvy = 0;

	vx = 0;
	vy = 0;
	vz = 0;

	playerBlock.setPosition(x, y, z);

	fov = YsPi / 6.0;  // 30 degree
	nearZ = 0.5;
	farZ = 1000.0;
	zoom = 0;

	sensitivity = 1.0 / 5.0;
}

void CameraObject::Initialize(void)
{
	x = 0;
	y = 16;
	z = 0;
	h = 0;
	p = 0;
	b = 0;
	camHeight = 16;
	py = camHeight;
	pvy = 0;

	vx = 0;
	vy = 0;
	vz = 0;

	//playerBlock.setPosition(x-playerBlock.blockSize/2, 0, (z + roomSize - 2)*playerBlock.blockSize);
	playerBlock.setCoordinate(x, 0, z);

	fov = YsPi / 6.0;  // 30 degree
	nearZ = 0.5;
	farZ = 1000.0;
	zoom = 0;

	sensitivity = 1.0 / 5.0;
}

void CameraObject::SetUpCameraProjection(void)
{
	int wid, hei;
	double aspect;

	FsGetWindowSize(wid, hei);
	aspect = (double)wid / (double)hei;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov*180.0 / YsPi, aspect, nearZ, farZ);
}

void CameraObject::SetUpCameraTransformation(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(-b*180.0 / YsPi, 0.0, 0.0, 1.0);  // Rotation about Z axis
	glRotated(-p*180.0 / YsPi, 1.0, 0.0, 0.0);  // Rotation about X axis
	glRotated(-h*180.0 / YsPi, 0.0, 1.0, 0.0);  // Rotation about Y axis
	glTranslated(-x, -y, -z);
	glTranslated(vx*zoom, vy*zoom, vz*zoom);
}

std::vector<double> CameraObject::GetForwardVector(double &vx, double &vy, double &vz)
{
	vx = -cos(p)*sin(h);
	vy = sin(p);
	vz = -cos(p)*cos(h);
	forwardVector[0] = vx;
	forwardVector[1] = vy;
	forwardVector[2] = vz;
	return forwardVector;
}

void CameraObject::DrawCamera(void)
{
	// Draw Forward Vector
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(playerBlock.pos[0] + playerBlock.blockSize / 2, playerBlock.pos[1] + playerBlock.blockSize / 2, playerBlock.pos[2] + playerBlock.blockSize / 2);
	glVertex3d(playerBlock.pos[0] + vx * 100 + playerBlock.blockSize / 2, playerBlock.pos[1] + vy * 100 + playerBlock.blockSize / 2,
		playerBlock.pos[2] + vz * 100 + playerBlock.blockSize / 2);
	glEnd();

	if (zoom > 5)
	{
		// Draw Camera Cube
		glBegin(GL_QUADS);
		// Left Side
		playerBlock.DrawSolid();
		glEnd();
	}
}

void CameraObject::Update(int key)
{
	FsPollDevice();
	
	mouseEvent = FsGetMouseEvent(lb, mb, rb, mx, my);
	FsGetWindowPosition(winx0, winy0);
	FsGetWindowSize(wid, hei);
	GetForwardVector(vx, vy, vz);

	//printf("key = %d", key);

	if (key == FSKEY_TAB)
	{
		cursorLock = (cursorLock++) % 2;
		if (cursorLock == 1)
		{
			ShowCursor(FALSE);
			SetCursorPos(winx0 + wid / 2, winy0 + hei / 2);
		}
		else
		{
			ShowCursor(TRUE);
		}
	}

	if (mouseEvent == FSMOUSEEVENT_MOVE) // mouseView-control
	{
		if (cursorLock == 1)
		{
			double dx, dy, dh, dp;
			int wid, hei;
			FsGetWindowSize(wid, hei);
			dx = (double)(wid / 2 - mx) - 9;
			dy = (double)(hei / 2 - my) - 38;
			//printf("dx = %lf, dy = %lf\n", dx, dy);
			dh = dx / 240 * sensitivity;
			dp = dy / 240 * sensitivity;
			//printf("dh = %lf, dp = %lf\n", dh, dp);
			h += dh;
			p += dp;
			//printf("cameraH = %lf, cameraP = %lf\n", h, p);
			SetCursorPos(winx0 + wid / 2, winy0 + hei / 2);
		}
		else
		{
			//printf("mx = %d, my = %d\n", mx, my);
		}
	}
	//key = FsInkey();
	if (key != 0)
	{
		printf("%d\n", key);
	}
	if (key == FSKEY_WHEELDOWN)
	{
		printf("Zoom in");
		zoom += 5;
		farZ += 5;
	}
	else if (key == FSKEY_WHEELUP)
	{
		printf("Zoom out");
		zoom = max(zoom - 5, 0);
		farZ = min(farZ-5,1000.0);
	}

	if (cursorLock == 0) // arrowView-control
	{
		if (0 != FsGetKeyState(FSKEY_LEFT))
		{
			h += YsPi / 240.0;
		}
		if (0 != FsGetKeyState(FSKEY_RIGHT))
		{
			h -= YsPi / 240.0;
		}
		if (0 != FsGetKeyState(FSKEY_UP))
		{
			p += YsPi / 240.0;
		}
		if (0 != FsGetKeyState(FSKEY_DOWN))
		{
			p -= YsPi / 240.0;
		}
	}
	
	if (0 != FsGetKeyState(FSKEY_W))
	{
		x += vx / 1;
		z += vz / 1;
		playerBlock.setPosition(playerBlock.pos[0] + vx / 1, playerBlock.pos[1], playerBlock.pos[2] + vz / 1);
	}
	if (0 != FsGetKeyState(FSKEY_S))
	{
		x -= vx / 2.0;
		z -= vz / 2.0;
		playerBlock.setPosition(playerBlock.pos[0] - vx / 2, playerBlock.pos[1], playerBlock.pos[2] - vz / 2);
	}
	if (0 != FsGetKeyState(FSKEY_D))
	{
		x -= vz / 1;
		z += vx / 1;
		playerBlock.setPosition(playerBlock.pos[0] - vz / 1, playerBlock.pos[1], playerBlock.pos[2] + vx / 1);
	}
	if (0 != FsGetKeyState(FSKEY_A))
	{
		x += vz / 1;
		z -= vx / 1;
		playerBlock.setPosition(playerBlock.pos[0] + vz / 1, playerBlock.pos[1], playerBlock.pos[2] - vx / 1);
	}

	//{
	//	if (pvy != 0)
	//	{
	//		py = py + pvy;
	//		pvy += GRAV;
	//	}
	//	if (py == 16)
	//	{
	//		if (0 != FsGetKeyState(FSKEY_SPACE))
	//		{
	//			pvy = 2;
	//		}
	//	}
	//} //GRAVITY ON

	{
		if (0 != FsGetKeyState(FSKEY_1))
		{
			y += 2;
			py = y;
			playerBlock.setPosition(playerBlock.pos[0], playerBlock.pos[1] + 2, playerBlock.pos[2]);
		}
		if (0 != FsGetKeyState(FSKEY_2) && y >= 0)
		{
			y -= 2;
			py = y;
			playerBlock.setPosition(playerBlock.pos[0], playerBlock.pos[1] - 2, playerBlock.pos[2]);
		}
	} // GRAVITY OFF

	if (py<camHeight)
	{
		py = camHeight;
		y = py;
		pvy = 0;
	}

	if (0 != FsGetKeyState(FSKEY_CTRL))
	{
		y = py - 12;
	}
	else
	{
		y = py;
	}
	playerBlock.setPosition(x - playerBlock.blockSize / 2, y - camHeight, z - playerBlock.blockSize / 2);
	//printf("%lf %lf %lf\n", playerBlock.pos[0], playerBlock.pos[1], playerBlock.pos[2]);
	DrawCamera();
}

double VecLen(std::vector<double> const &vec)
{
	double sum = 0;
	for (double e : vec)
	{
		sum += e*e;
	}
	return sqrt(sum);
}

double Dot(std::vector<double> const &vec1, std::vector<double> const &vec2)
{
	double len = VecLen(vec2);
	return (vec1[0] * (vec2[0] / len) + vec1[1] * (vec2[1] / len) + vec1[2] * (vec2[2] / len));
}

void DrawOffsetMode(int &drawCount, std::vector<Block> &blockVec, CameraObject &camera2, double len)
{
	glBegin(GL_QUADS);
	drawCount = 0;
	for (int i = 0; i < blockCount; i++)
	{
		auto b = blockVec[i];
		for (int i = 0; i < 3; i++)
		{
			dist[i] = b.pos[i] - camera2.forwardVector[i] / len;
			//printf("%lf",b.pos[i]);
		}
		//printf("\n");
		coneDist = Dot(dist, camera2.forwardVector);
		//printf("coneDist = %lf\n", coneDist);
		if (coneDist >= camera2.nearZ && coneDist <= camera2.farZ)
		{
			//coneRad = coneDist / abs(camera2.farZ - camera2.nearZ)*abs(camera2.farZ);
			coneRad = coneDist / abs(camera2.farZ - camera2.nearZ) * 400;
			orthVec[0] = dist[0] - coneDist*camera2.forwardVector[0];
			orthVec[1] = dist[1] - coneDist*camera2.forwardVector[1];
			orthVec[2] = dist[2] - coneDist*camera2.forwardVector[2];
			orthDist = VecLen(orthVec);
			//printf("OrthDist = %lf\n coneRad = %lf\n",orthDist, coneRad);
			if (orthDist <= coneRad)
			{
				b.DrawSolid();
				drawCount++;
			}
		}
		//printf("Drawn");
	}
	//printf("Draw Count = %d\n", drawCount);
	glEnd();
}

void MergeVecs(std::vector<Block> &blockVec, std::vector<int> &indexVec, int index1, int end1, int index2, int end2)
{
	int startIndex = index1;
	int size = end2 - startIndex;
	std::vector<int> vec1;
	while (index1 < end1 && index2 < end2) // compare vecs, populate temp vec
	{
		if (blockVec[indexVec[index1]].index < blockVec[indexVec[index2]].index)
		{
			vec1.push_back(indexVec[index1]);
			index1++;
		}
		else
		{
			vec1.push_back(indexVec[index2]);
			index2++;
		}
	} 

	while (index1 < end1) // add rest of 1
	{
		vec1.push_back(indexVec[index1]);
		index1++;
	}

	while (index2 < end2) // add rest of 2
	{
		vec1.push_back(indexVec[index2]);
		index2++;
	}
	//printf("size = %d\n", size);

	for (int i = 0; i < size; i++) // modify orig vec
	{
		indexVec[startIndex + i] = vec1[i];
		//printf("%d | ", indexVec[startIndex + i]);
	}
	//printf("\n");
	//getchar();
	//for (int i = index1; i < end2; i++)
	//{
	//	printf("VecIndex = %d | BlockIndex = %d\n", indexVec[i], blockVec[indexVec[i]].index);
	//}
	//printf("\n");
	//getchar();
}

void SortBlocks(std::vector<Block> &blockVec, std::vector<int> &indexVec, int blockNum)
{
	int vecSize = 1;
	int start1, end1, start2, end2;
	//for (int i = 0; i < blockNum; i++)
	//{
	//	printf("VecIndex = %d | BlockIndex = %d\n", indexVec[i], blockVec[i].index);
	//}
	while (vecSize < blockNum)
	{
		//printf("vecSize = %d\n", vecSize);
		int j = 0;
		for (int i = 0; i < blockNum; i += (2 * vecSize))
		{
			j++;
			start1 = i;
			end1 = start1 + vecSize;
			start2 = end1;
			end2 = min(start2 + vecSize, blockNum);
			//printf("Iteration %d \n", j);
			//printf("start = %d\n", start1);
			//printf("end = %d\n", end2);
			MergeVecs(blockVec, indexVec, start1, end1, start2, end2);
		}

		vecSize *= 2;
	}
	//printf("Sorted");
	//for (int i = 0; i < blockNum; i++)
	//{
	//	printf("VecIndex = %d | BlockIndex = %d\n", indexVec[i], blockVec[indexVec[i]].index);
	//}
	//getchar();
}

int BinBlockSearch(std::vector<Block> &blockVec, std::vector<int> &indexVec, int idx)
{
	//int length = blockCount;
	int L = 0, R = blockCount;
	int i = (R+L)/2;
	for(;;)
	{
		//printf("idx = %d | blockIndex = %d | blockVal = %d | size = %d\n", idx, i, blockVec[indexVec[i]].index, size);
		if (idx == blockVec[indexVec[i]].index)
		{
			//printf("FOUND MATCH");
			//getchar();
			return i;
		}
		else if (L > R)
		{
			return NULLINT;
		}
		else if (blockVec[indexVec[i]].index < idx)
		{
			L = i + 1;
			i = (R+L)/2;
		}
		else
		{
			R = i - 1;
			i = (R+L)/2;
		}
		//getchar();
	}
}

void CheckSide(std::vector<Block> &blockVec, std::vector<int> &indexVec, int rows, int cols, int height, int blockNum)
{
	int indexCheck[6] = { -1, 1, -cols, cols, -rows*cols, rows*cols }; // left, right, back, front, bottom, top
	int idx;
	int length = sizeof(blockVec) / sizeof(blockVec[0]);
	auto &b = blockVec[blockNum];
	for (int e = 0; e < 6; e++)
	{
		idx = b.index + indexCheck[e];
		//printf("\nBlock Index = %d | BlockCheck Index = %d\n", b.index, idx);
		// binSearch for conjecture block, if it exists then hide
		idx = BinBlockSearch(blockVec, indexVec, idx); // new index in sorted vector
		if (idx != NULLINT)
		{
			auto &b1 = blockVec[indexVec[idx]];
			if (abs(b.getX() - b1.getX()) + abs(b.getY() - b1.getY()) + abs(b.getZ() - b1.getZ()) == 1)
			{
				b.sideVisible[e] = 0;
			}
		}
		//getchar();
	}
}

void HideSides(std::vector<Block> &blockVec, std::vector<int> &indexVec, int rows, int cols, int height)
{
	for (int i = 0; i < rows*cols*height; i++)
	{
		CheckSide(blockVec, indexVec, rows, cols, height, i);
	}
}

int main(void)
{
	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent, key = 0;
	int drawCount = 0;
	CameraObject camera, camera2;
	std::vector<Block> blockVec;
	std::vector<int> indexVec;

	for (int i = 0; i < blockCount; i++)
	{
		indexVec.push_back(i);
	}
	for (int i = 0; i < blockCount; i++) // random blocks
	{
		blockVec.push_back(Block());
	}

	//int y = roomSize/2;
	//for (int z = 0; z < roomSize; z++) // organized blocks
	//{
	//	for (int x = 0; x < roomSize; x++)
	//	{
	//		blockVec.push_back(Block(x, y, z));
	//	}
	//}
	SortBlocks(blockVec, indexVec, blockCount);
	HideSides(blockVec, indexVec, roomSize, roomSize, 1);
	FsOpenWindow(800, 100, WINWID, WINHEI, 1);
	camera.Update(key);
	camera.z = 500;
	bool reductionMode = FALSE;
	double len = VecLen(camera.forwardVector);
	while (0 == terminate)
	{
		FsPollDevice();

		int wid, hei;
		FsGetWindowSize(wid, hei);

		key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			terminate = 1;
			break;
		case FSKEY_SHIFT:
			reductionMode = !reductionMode;
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glViewport(0, 0, wid, hei);

		// Set up 3D drawing
		camera.SetUpCameraProjection();
		camera.SetUpCameraTransformation();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);

		// 3D drawing from here
		DrawGroundLattice();
		drawCount = 0;

		if (reductionMode)
		{
			camera2.Update(key);
			DrawOffsetMode(drawCount, blockVec, camera2, len);
		}
		else
		{
			camera.Update(key);
			glBegin(GL_QUADS);
			for (auto &b : blockVec)
			{
				b.DrawSolid();
				drawCount++;
			}
			glEnd();

			glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			for (auto &b : blockVec)
			{
				b.DrawEdges();
			}
			glEnd();
		}

		//printf("Draw Count = %d\n", drawCount);

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		// 2D drawing from here

		FsSwapBuffers();
		FsSleep(5);
	}

	return 0;
}


//void HideSides(Block(&blockGrid)[blockCount], int rows, int cols, int height)
//{
//	for (int i = 0; i < rows*cols*height; i++)
//	{
//		CheckSide(blockGrid, rows, cols, height, i);
//	}
//}

//void Swap(Block &block1, Block &block2)
//{
//	decltype(block1) &temp = block1;
//	block1 = block2;
//	block2 = temp;
//}

//void CheckSide(Block(&blockGrid)[blockCount], int rows, int cols, int height, int blockNum)
//{
//	int indexCheck[6] = { -1, 1, -cols, cols, -rows*cols, rows*cols }; // left, right, back, front, bottom, top
//	int idx;
//	int length = sizeof(blockGrid) / sizeof(blockGrid[0]);
//	auto &b = blockGrid[blockNum];
//	for (int e = 0; e < 6; e++)
//	{
//		idx = blockNum + indexCheck[e];
//		if (0 <= (idx) && idx <= length)
//		{
//			auto &b1 = blockGrid[idx];
//			if (abs(b.getX() - b1.getX()) + abs(b.getY() - b1.getY()) + abs(b.getZ() - b1.getZ()) == 1)
//			{
//				b.sideVisible[e] = 0;
//			}
//		}
//	}
//}
