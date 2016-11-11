#include <stdio.h>
#include <math.h>
#include <random>
#include <vector>
#include <memory>
#include <iostream>
#include "fssimplewindow.h"
#if defined(_WIN32_WINNT) // Windows mouse movement routine
#else
	//#include <CoreGraphics/Coregraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#endif
#define NULLINT -1

const double YsPi = 3.1415927;
const int WINWID = 800, WINHEI = 600;
const double GRAV = -.09;

std::vector<double> dist = { 0,0,0 }, orthVec = { 0,0,0 }, fVec = { 0,0,0 };
double coneDist, coneRad, orthDist;

template <class NumType>
void VecPlus(std::vector<NumType> &first, std::vector<NumType> &second) // add second vec to first
{
	for (int i = 0; i < 3; i++)
	{
		first[i] += second[i];
	}
}

template <class NumType>
void VecPlus(std::vector<NumType> &first, NumType i1, NumType i2, NumType i3) // add second vec to first
{
	first[0] += i1;
	first[1] += i2;
	first[2] += i3;
}

template <class NumType>
void VecMinus(std::vector<NumType> &first, std::vector<NumType> &second) // subtract second vec from first
{
	for (int i = 0; i < 3; i++)
	{
		first[i] -= second[i];
	}
}

template <class NumType>
void VecMinus(std::vector<NumType> &first, NumType i1, NumType i2, NumType i3) // add second vec to first
{
	first[0] -= i1;
	first[1] -= i2;
	first[2] -= i3;
}

template <class NumType>
void SetVec(std::vector<NumType> &first, std::vector<NumType> &second)
{
	for (int i = 0; i < 3; i++)
	{
		first[i] = second[i];
	}
}

template <class NumType>
void SetVec(std::vector<NumType> &first, NumType i1, NumType i2, NumType i3) // add second vec to first
{
	first[0] = i1;
	first[1] = i2;
	first[2] = i3;
}

class Block
{
public:
	int roomSize = 10;
	Block();
	Block(int roomSize);
	Block(int roomSize, int x, int y, int z);
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
	double blockSize = 8.0;
	int index = 0;
	int sideVisible[6] = { 1,1,1,1,1,1 };
	std::vector<double> pos = { 0,0,0 };
};

Block::Block()
{
	roomSize = 10;
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

Block::Block(int roomSizeIn)
{
	roomSize = roomSizeIn;
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

Block::Block(int roomSizeIn, int x, int y, int z)
{
	roomSize = roomSizeIn;
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

class CameraObject
{
public:
	//double x, y, z;r
	double h, p, b;
	double px, py, pz, vertVel;
	double vx, vy, vz;
	double camHeight;
	double zoom;
	double viewRadius;
	std::vector<double> forwardVector = { 0,0,0 };
	std::vector<double> pos = { 0,0,0 };
	std::vector<int> gridLocation = { 0,0,0 };
	bool cursorLock = FALSE;
	bool stationary = FALSE;
	bool gravityOn = FALSE;
	int jumps = 5;
	int lb, mb, rb, mx, my, mouseEvent, key, wid, hei, winx0, winy0;

	double fov, nearZ, farZ;
	double sensitivity;
	CameraObject();
	CameraObject(double xLoc, double yLoc, double zLoc);

	void Initialize(double startX, double startY, double startZ);
	void SetUpCameraProjection(void);
	void SetUpCameraTransformation(void);
	void DrawCamera(void);
	Block playerBlock;

	void GetForwardVector(void);
	void Update(int &key);
};

CameraObject::CameraObject()
{
	Initialize(0.0,0.0,0.0);
}

CameraObject::CameraObject(double startX, double startY, double startZ)
{
	Initialize(startX, startY, startZ);
}

void CameraObject::Initialize(double startX, double startY, double startZ)
{
	SetVec(pos, startX, startY, startZ);
	h = 0;
	p = 0;
	b = 0;
	camHeight = 16.0;
	vertVel = 0.0;
	jumps = 5;

	vx = 0;
	vy = 0;
	vz = 0;

	playerBlock.setCoordinate(startX, 0, startY);
	playerBlock.blockSize = 8;

	fov = YsPi / 6.0;  // 30 degree
	nearZ = 0.5;
	farZ = 1000.0;
	viewRadius = 1000;
	zoom = 0.0;

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
	glTranslated(-pos[0], -pos[1]-camHeight, -pos[2]);
	glTranslated(forwardVector[0]*zoom, forwardVector[1]*zoom, forwardVector[2]*zoom);
}

void CameraObject::GetForwardVector(void)
{
	forwardVector[0] = -cos(p)*sin(h);
	forwardVector[1] =	sin(p);
	forwardVector[2] = -cos(p)*cos(h);
	//printf("%lf %lf %lf", forwardVector[0], forwardVector[1], forwardVector[2]);
}

void CameraObject::DrawCamera(void)
{
	// Draw Forward Vector
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
	glVertex3d(playerBlock.pos[0] + playerBlock.blockSize / 2, playerBlock.pos[1] + playerBlock.blockSize / 2, playerBlock.pos[2] + playerBlock.blockSize / 2);
	glVertex3d(playerBlock.pos[0] + forwardVector[0] * 100.0 + playerBlock.blockSize / 2, playerBlock.pos[1] + forwardVector[1] * 100.0 + playerBlock.blockSize / 2,
		playerBlock.pos[2] + forwardVector[2] * 100.0 + playerBlock.blockSize / 2);
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

void CameraObject::Update(int &key)
{
	FsPollDevice();
	mouseEvent = FsGetMouseEvent(lb, mb, rb, mx, my);
	FsGetWindowPosition(winx0, winy0);
	FsGetWindowSize(wid, hei);
	GetForwardVector();

	if (key != 0)
	{
		//printf("%d\n", key);
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
		farZ = max(farZ - 5, 1000.0);
	}

#if defined(_WIN32_WINNT) // Windows mouse movement routine
	if (key == FSKEY_TAB)
	{
		cursorLock = !cursorLock;
		if (cursorLock)
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
#else
	CGDirectDisplayID ID = CGMainDisplayID();
	CGRect CurRect;
	CurRect = CGDisplayBounds(ID);
	CGFloat monitorheight = CurRect.size.height;
	CGPoint mouse;

	mouse.x = winx0 + wid / 2;
	mouse.y = monitorheight - winy0 - hei / 2;
	if (key == FSKEY_TAB)
	{
		cursorLock = !cursorLock;
		if (cursorLock)
		{
			CGDisplayHideCursor(ID);
			CGDisplayMoveCursorToPoint(ID, mouse);
		}
		else
		{
			CGDisplayShowCursor(ID);
		}
	}

	if (cursorLock == 1)
	{
		FsPollDevice();
		FsGetMouseState(lb, mb, rb, mx, my);
		double dx, dy, dh, dp;
		int wid, hei;
		FsGetWindowSize(wid, hei);
		dx = wid/2 - mx;
		dy = hei/2 - my -1;
		dh = dx / 240.0 * sensitivity;
		dp = dy / 240.0 * sensitivity;
		h += dh;
		p += dp;

		//printf("%d, %d, %d, %d\n", mx, my, pastmx, pastmy);

		CGDisplayMoveCursorToPoint(ID, mouse);
		CGWarpMouseCursorPosition(mouse);
		CGAssociateMouseAndMouseCursorPosition(true);
		FsGetMouseState(lb, mb, rb, pastmx, pastmy);
#endif

	if (cursorLock == 0) // arrowView-control
	{
		if (FsGetKeyState(FSKEY_LEFT))
		{
			h += YsPi / 240.0;
		}
		if (FsGetKeyState(FSKEY_RIGHT))
		{
			h -= YsPi / 240.0;
		}
		if (FsGetKeyState(FSKEY_UP))
		{
			p += YsPi / 240.0;
		}
		if (FsGetKeyState(FSKEY_DOWN))
		{
			p -= YsPi / 240.0;
		}
	}
	if (!stationary)
	{
		if (FsGetKeyState(FSKEY_W))
		{
			VecPlus(pos, forwardVector[0], 0.0, forwardVector[2]);
		}
		if (FsGetKeyState(FSKEY_S))
		{
			VecMinus(pos, forwardVector[0] / 2.0, 0.0, forwardVector[2] / 2.0);
		}
		if (FsGetKeyState(FSKEY_D))
		{
			pos[0] -= forwardVector[2];
			pos[2] += forwardVector[0];
		}
		if (FsGetKeyState(FSKEY_A))
		{
			pos[0] += forwardVector[2];
			pos[2] -= forwardVector[0];
		}

		if (gravityOn)
		{
			//printf("Gravity On");
			if (pos[1] > 0.0)
			{
				pos[1] += vertVel;
				vertVel += GRAV;
			}
			else
			{
				if (pos[1] < 0.0)
				{
					pos[1] = 0.0;
					vertVel = 0.0;
					jumps = 5;
				}
			}
			if (jumps != 0)
			{
				if (key == FSKEY_SPACE)
				{
					if (vertVel == 0.0)
					{
						pos[1] = .01;
					}

					vertVel += 4.0;
					jumps--;
				}
			}
		} 
		else // Gravity Off
		{
			if (FsGetKeyState(FSKEY_1))
			{
				pos[1] += 2.0;
			}
			if (FsGetKeyState(FSKEY_2) && pos[1] >= 0)
			{
				pos[1] -= 2.0;
			}
		} 

		if (FsGetKeyState(FSKEY_CTRL))
		{
			camHeight = playerBlock.blockSize / 2.0;
		}
		else
		{
			camHeight = 16.0;
		}
		//printf("%lf\n\n", camHeight);
		playerBlock.setPosition(pos[0] - playerBlock.blockSize / 2.0, pos[1], pos[2] - playerBlock.blockSize / 2.0);
	}

	if (FsGetKeyState(FSKEY_PLUS))
	{
		if (viewRadius <= 1000)
		{
			viewRadius += 100;
		}
	}
	if (FsGetKeyState(FSKEY_MINUS))
	{
		if (viewRadius > 0)
		{
			viewRadius -= 100;
		}
	}
	//printf("%lf %lf %lf\n", playerBlock.pos[0], playerBlock.pos[1], playerBlock.pos[2]);
	DrawCamera();
}

class Terrain
{
public:
	int roomSize, blockNum, totalSpace, drawCount;
	int indexCheck[6];
	std::vector<Block> blockVec;
	std::vector<int> indexVec;

	Terrain();
	Terrain(int size);
	Terrain(int size, int randomVsOrdered);
	~Terrain();
	void Initialize(int randomVsOrdered);

	void inline SizeVectors(void);
	void inline GenerateRandom(void);
	void inline GenerateOrdered(void);

	void SortBlocks(void);
	void MergeVecs(int index1, int end1, int index2, int end2);
	void HideSides(void);
	void CheckBlockSides(int i);
	int BinBlockSearch(int targetIndex, int &mostRecentIndex);

	void AddBlock(int x, int y, int z);

	void DrawTerrain(CameraObject &cameraView, bool reductionMode, int &key);
	void DrawOffsetMode(int &drawCount, CameraObject &camera);
};

Terrain::Terrain()
{
	roomSize = 10; // default
	Initialize(0);
}

Terrain::Terrain(int size)
{
	roomSize = size;
	Initialize(0);
}

Terrain::Terrain(int size, int randomVsOrdered) // 0 = random, 1 = linear
{
	roomSize = size;
	Initialize(randomVsOrdered);
}

void Terrain::Initialize(int randomVsOrdered) // 0 = random, 1 = linear
{
	blockNum = roomSize*roomSize;
	totalSpace = roomSize*roomSize*roomSize;
	indexCheck[0] = -1; indexCheck[1] = 1; indexCheck[2] = -roomSize; indexCheck[3] = roomSize;
	indexCheck[4] = -roomSize*roomSize; indexCheck[5] = roomSize*roomSize; // left, right, back, front, bottom, top
	SizeVectors();
	if (randomVsOrdered)
	{
		GenerateRandom();
	}
	else
	{
		GenerateOrdered();
	}
	SortBlocks();
	HideSides();
}

Terrain::~Terrain()
{
}

void inline Terrain::SizeVectors(void)
{
	for (int i = 0; i < blockNum; i++)
	{
		indexVec.push_back(i);
	}
}

void inline Terrain::GenerateRandom(void)
{
	for (int i = 0; i < blockNum; i++) // random blocks
	{
		blockVec.push_back(Block(roomSize));
	}
}

void inline Terrain::GenerateOrdered(void) // ordered blocks (starts at "y=0" plane, actually y = roomHeight/2)
{
	//int y = roomSize/2;
	for (int y = roomSize / 2; y < roomSize / 2 + 1; y++)
	{
		for (int z = 0; z < roomSize; z++)
		{
			for (int x = 0; x < roomSize; x++)
			{
				blockVec.push_back(Block(roomSize, x, y, z));
			}
		}
	}
}

void Terrain::MergeVecs(int index1, int end1, int index2, int end2)
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

void Terrain::SortBlocks()
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
			end2 = fmin(start2 + vecSize, blockNum);
			//printf("Iteration %d \n", j);
			//printf("start = %d\n", start1);
			//printf("end = %d\n", end2);
			MergeVecs(start1, end1, start2, end2);
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

void Terrain::HideSides(void)
{
	for (int i = 0; i < blockNum; i++)
	{
		CheckBlockSides(i);
	}
}

int Terrain::BinBlockSearch(int targetValue, int &mostRecentIndex)
{
	int L = 0, R = blockNum-1;
	int i = (R + L) / 2;
	for (;;)
	{
		//printf("targetIndex = %d | blockIndex = %d | blockVal = %d | size = %d\n", targetIndex, i, blockVec[indexVec[i]].index, (R+L)/2);
		if (targetValue == blockVec[indexVec[i]].index)
		{
			//printf("FOUND MATCH");
			//getchar();
			return i;
		}
		else if (L > R)
		{
			mostRecentIndex = i;
			return NULLINT;
		}
		else if (blockVec[indexVec[i]].index < targetValue)
		{
			L = i + 1;
			i = (R + L) / 2;
			mostRecentIndex = L-1;
		}
		else
		{
			R = i - 1;
			i = (R + L) / 2;
			mostRecentIndex = i;
			//mostRecentIndex = R+1;
		}
		//getchar();
	}
}

void Terrain::CheckBlockSides(int i) // for block i in the block-Vector, check the 6 surrounding sides and hide any shared faces
{
	int idx;
	auto &b = blockVec[i];
	int mostRecentIndex;
	int eReverse;
	for (int e = 0; e < 6; e++)
	{
		eReverse = e + (e % 2 == 0 ? 1 : -1);
		//printf("%d vs %d\n", e, eReverse);
		idx = b.index + indexCheck[e];
		//printf("  Searching for %d \n", idx);
		//printf("\nBlock Index = %d | BlockCheck Index = %d\n", b.index, idx);
		// binSearch for conjecture block, if it exists then hide
		idx = BinBlockSearch(idx, mostRecentIndex); // new index in sorted vector
		if (idx != NULLINT)
		{
			auto &b1 = blockVec[indexVec[idx]];
			//printf("Original Loc = %d\n", b.index);
			//printf("Found Loc = %d\n", b1.index);
			if (b1.sideVisible[eReverse] == 0 || abs(b.getX() - b1.getX()) + abs(b.getY() - b1.getY()) + abs(b.getZ() - b1.getZ()) == 1)
			{
				b.sideVisible[e] = 0;
				b1.sideVisible[eReverse] = 0;
			}
		}
		//getchar();
	}
}

void Terrain::AddBlock(int x, int y, int z)
{
	int newLocation = x + z*roomSize + y*roomSize*roomSize;
	//printf("newLocation = %d\n", newLocation);
	int mostRecentIndex = 0;
	if (BinBlockSearch(newLocation, mostRecentIndex) == NULLINT) // free spot
	{
		//printf("Size = %d\n", blockVec.size());
		blockVec.push_back(Block(roomSize, x, y, z));
		//printf("New location before = %d\n", blockVec[blockNum-1].index);
		//blockVec[blockNum].index = newLocation;
		//printf("New location after = %d\n", blockVec[blockNum].index);
		//printf("Most Recent Index = %d\n\n", mostRecentIndex);

		//for (auto &b : blockVec)
		//{
		//	printf("%d\n", b.index);
		//}

		if (newLocation > blockVec[indexVec[mostRecentIndex]].index)
		{
			while (newLocation > blockVec[indexVec[mostRecentIndex]].index && mostRecentIndex < blockNum-1 )
			{
				//printf("Most Recent Index UP = %d\n\n", mostRecentIndex);
				mostRecentIndex++;
				//printf("Most Recent Location = %d\n\n", blockVec[indexVec[mostRecentIndex]].index);
			}
		}
		else
		{
			while (newLocation < blockVec[indexVec[mostRecentIndex]].index && mostRecentIndex < 0)
			{
				//printf("Most Recent Index DOWN = %d\n\n", mostRecentIndex);
				mostRecentIndex--;
			}
		}

		blockNum++;
		//printf("BlockNum = %d\n\n", blockNum);
		//printf("Most Recent Index = %d\n\n", mostRecentIndex);

		//blockVec[blockNum].index = newIndex;
		if (mostRecentIndex == blockNum - 1)
		{
			//printf("pushed\n");
			indexVec.push_back(blockNum-1);
		}
		//else if (mostRecentIndex == 0)
		//{
		//	printf("front item");
		//	indexVec.insert(indexVec.begin(), blockNum - 1);
		//}
		else
		{
			//printf("inserted at %d\n", mostRecentIndex);
			indexVec.insert(indexVec.begin() + mostRecentIndex, blockNum - 1);
		}
		CheckBlockSides(blockNum-1);
	}
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

void Terrain::DrawOffsetMode(int &drawCount, CameraObject &camera)
{
	glBegin(GL_QUADS);
	drawCount = 0;

	//for (int i = 0; i < 3; i++)
	//{
	//	printf("%lf | ", camera.pos[i]);
	//}
	//printf("\n");

	for (int i = 0; i < blockNum; i++)
	{
		auto b = blockVec[i];
		for (int i = 0; i < 3; i++)
		{
			dist[i] = b.pos[i] - camera.pos[i];// camera.forwardVector[i];
											   //printf("%lf",b.pos[i]);
		}
		//printf("\n");
		coneDist = Dot(dist, camera.forwardVector);
		//printf("coneDist = %lf\n", coneDist);
		if (VecLen(dist) <= 50)
		{
			b.DrawSolid();
		}
		else if (coneDist <= camera.farZ)
		{
			//coneRad = coneDist / abs(camera.farZ - camera.nearZ)*abs(camera.farZ);
			coneRad = coneDist / abs(camera.farZ - camera.nearZ) * camera.viewRadius;
			orthVec[0] = dist[0] - coneDist*camera.forwardVector[0];
			orthVec[1] = dist[1] - coneDist*camera.forwardVector[1];
			orthVec[2] = dist[2] - coneDist*camera.forwardVector[2];
			orthDist = VecLen(orthVec);
			//printf("OrthDist = %lf\n coneRad = %lf\n",orthDist, coneRad);
			if (orthDist <= coneRad)
			{
				b.DrawSolid();
			}
		}
	}
	//printf("Draw Count = %d\n", drawCount);
	glEnd();
	glBegin(GL_LINES);
	for (int i = 0; i < blockNum; i++)
	{
		auto b = blockVec[i];
		for (int i = 0; i < 3; i++)
		{
			dist[i] = b.pos[i] - camera.pos[i];
		}
		//printf("\n");
		coneDist = Dot(dist, camera.forwardVector);
		//printf("coneDist = %lf\n", coneDist);
		if (VecLen(dist) <= 50)
		{
				b.DrawEdges();
		}
		else if(coneDist <= camera.farZ)
		{
			//coneRad = coneDist / abs(camera.farZ - camera.nearZ)*abs(camera.farZ);
			coneRad = coneDist / abs(camera.farZ - camera.nearZ) * camera.viewRadius;
			orthVec[0] = dist[0] - coneDist*camera.forwardVector[0];
			orthVec[1] = dist[1] - coneDist*camera.forwardVector[1];
			orthVec[2] = dist[2] - coneDist*camera.forwardVector[2];
			orthDist = VecLen(orthVec);
			//printf("OrthDist = %lf\n coneRad = %lf\n",orthDist, coneRad);
			if (orthDist <= coneRad)
			{
				b.DrawEdges();
				drawCount++;
			}
		}
	}
	glEnd();
}

void Terrain::DrawTerrain(CameraObject &cameraView, bool reductionMode, int &key)
{
	drawCount = 0;
	// Draw ground lattice
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

	if (reductionMode)
	{
		cameraView.Update(key);
		DrawOffsetMode(drawCount, cameraView);
	}
	else
	{
		cameraView.Update(key);
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
}

int main(void)
{
	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent, key = 0;
	int drawCount = 0;
	CameraObject camera, camera2;

	Terrain worldGrid(100, 1);

	camera.playerBlock.roomSize = worldGrid.roomSize;
	camera2.playerBlock.roomSize = worldGrid.roomSize;

	FsOpenWindow(800, 100, WINWID, WINHEI, 1);

	camera.Update(key);
	camera2.Update(key);
	camera2.stationary = TRUE;
	camera.pos[2] = 100.0;
	bool reductionMode = false;
	bool switchCamera = false;

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
		case FSKEY_C:
			switchCamera = !switchCamera;
		case FSKEY_3:
			camera.gravityOn = !camera.gravityOn;
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
		//DrawGroundLattice();
		drawCount = 0;
		if (FsGetKeyState(FSKEY_C))
		{
			if (!camera.cursorLock)
			{
				camera.cursorLock != camera.cursorLock;
			}
			camera2.cursorLock = camera.cursorLock; // set both camera's to mouse mode
		}

		if (switchCamera)
		{
			worldGrid.DrawTerrain(camera2, reductionMode, key);
		}
		else
		{
			worldGrid.DrawTerrain(camera, reductionMode, key);
		}

		if (FsGetMouseEvent(lb, mb, rb, mx, my) == FSMOUSEEVENT_LBUTTONDOWN)
		{
			//printf("%d %d %d Tried to add\n", x,y,z);
			for (int i = 0; i < 10; i++)
			{
				int x = rand() % worldGrid.roomSize;
				int y = rand() % worldGrid.roomSize;
				int z = rand() % worldGrid.roomSize;

				worldGrid.AddBlock(x, y, z);
			}
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