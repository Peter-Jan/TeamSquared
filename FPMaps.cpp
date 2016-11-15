#include <stdio.h>
#include <math.h>
#include <random>
#include <vector>
#include <memory>
#include <iostream>
#include <time.h>
#include <map>
#include "fssimplewindow.h"

#if defined(_WIN32_WINNT) // Windows mouse movement routine
#else
	//#include <CoreGraphics/Coregraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#endif
#define NULLINT -1
#define REMOVEDBLOCK -2
const double YsPi = 3.1415927;
#define DEGTORAD YsPi/180
#define VISIBLE 0
const int WINWID = 800, WINHEI = 600;
const double GRAV = -.09;

std::vector<double> dist = { 0,0,0 }, orthVec = { 0,0,0 }, fVec = { 0,0,0 };
double coneDist, coneRad, orthDist;

template <class NumType>
NumType Pow(NumType base, int power)
{
	NumType value = 1;
	int i = power;
	while (i != 0)
	{
		if (power > 0)
		{
			value *= base;
			i--;
		}
		else
		{
			value /= base;
			i++;
		}
	}
	return value;
}

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
	double blockSize = 8.0;
	int index = 0;
	int sideVisible[6] = { 1,1,1,1,1,1 };
	int renderable = TRUE;
	std::vector<double> pos = { 0,0,0 };
};

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
	index = xRand + roomSize*zRand + Pow(roomSize,2)*yRand;
}

void Block::copyFrom(const Block &fromBlock)
{
	roomSize = fromBlock.roomSize;
	blockSize = 8;
	setCoordinate(fromBlock.x, fromBlock.y, fromBlock.z);
	setColor(255, 0, 0);
	setDimension(blockSize, blockSize, blockSize);
	pos[0] = (double)getX()*blockSize; pos[1] = (double)getY()*blockSize; pos[2] = (double)getZ()*blockSize;
	index = getX() + roomSize*getZ() + Pow(roomSize, 2)*getY();
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

int BlockDist(const Block &block1, const Block &block2)
{
	return abs(block1.getX() - block2.getX()) + abs(block1.getY() - block2.getY()) + abs(block1.getZ() - block2.getZ());
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
	index = xRand + roomSize*zRand + Pow(roomSize, 2)*yRand;
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
	index = x + roomSize*z + Pow(roomSize, 2)*y;
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
	index = x + roomSize*z + Pow(roomSize, 2)*y;
}

void Block::setPosition(double xLoc, double yLoc, double zLoc)
{
	pos[0] = xLoc;
	pos[1] = yLoc;
	pos[2] = zLoc;
	x = (int)(xLoc / blockSize);
	y = (int)(yLoc / blockSize);
	z = (int)(zLoc / blockSize);
	index = x + roomSize*z + Pow(roomSize, 2)*y;
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
	double vertVel, camHeight, zoom, viewRadius, sensitivity, fov, nearZ, farZ;
	std::vector<double> forwardVector = { 0,0,0 };
	std::vector<double> pos = { 0,0,0 };
	std::vector<int> gridLocation = { 0,0,0 };
	int blockSize, jumps = 5;
	int lb, mb, rb, mx, my, mouseEvent, key, wid, hei, winx0, winy0;
	bool cursorLock = FALSE;
	bool stationary = FALSE;
	bool gravityOn = FALSE;

	double inline x(void); // pos vector items
	double inline y(void);
	double inline z(void);
	double inline xF(void) const; // forward vector items
	double inline yF(void) const;
	double inline zF(void) const;
	int inline xGrid(void) const;
	int inline yGrid(void) const;
	int inline zGrid(void) const;

	CameraObject();
	CameraObject(double xLoc, double yLoc, double zLoc);

	void Initialize(double startX, double startY, double startZ);
	void SetUpCameraProjection(void);
	void SetUpCameraTransformation(void);
	void DrawCamera(void);
	Block playerBlock;

	void GetForwardVector(void);
	void inline SetGridLocation(void);
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

double inline CameraObject::x(void)
{
	return pos[0];
}

double inline CameraObject::y(void)
{
	return pos[1];
}

double inline CameraObject::z(void)
{
	return pos[2];
}

double inline CameraObject::xF(void) const
{
	return forwardVector[0];
}

double inline CameraObject::yF(void) const
{
	return forwardVector[1];
}

double inline CameraObject::zF(void) const
{
	return forwardVector[2];
}

int inline CameraObject::xGrid(void) const
{
	return gridLocation[0];
}

int inline CameraObject::yGrid(void) const
{
	return gridLocation[1];
}

int inline CameraObject::zGrid(void) const
{
	return gridLocation[2];
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
	blockSize = 8;

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

void inline CameraObject::SetGridLocation(void)
{
	gridLocation[0] = pos[0] / blockSize;
	gridLocation[1] = pos[1] / blockSize;
	gridLocation[2] = pos[2] / blockSize;
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
		//printf("Zoom in");
		zoom += 5;
		farZ += 5;
	}
	else if (key == FSKEY_WHEELUP)
	{
		//printf("Zoom out");
		zoom = fmax(zoom - 5, 0);
		farZ = fmax(farZ - 5, 1000.0);
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
		SetGridLocation();
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
	//std::map<int, Block> blockMap;
	std::map<int, std::unique_ptr<Block>> blockMap;
	std::map<int, int> renderMap;

	Terrain();
	Terrain(int size);
	Terrain(int size, int randomVsOrdered);
	~Terrain();
	void Initialize(int randomVsOrdered);

	void inline GenerateRandom(void);
	void inline GenerateOrdered(void);
	void GenerateFunctionTerrain(void);

	void HideSides(void);
	void HideSingleBlockSides(int i);
	void ShowSingleBlockSides(int i);
	int BinBlockSearch(int targetIndex, int &mostRecentIndex);
	int VisibleObjectSearch(int blockVecIndex, int &mostRecentIndex);
	void AddBlock(int x, int y, int z);
	void RemoveBlock(int x, int y, int z);

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

Terrain::Terrain(int size, int randomVsOrdered) // 0 = random, 1 = linear, 2 = Terrain generator
{
	roomSize = size;
	Initialize(randomVsOrdered);
}

void Terrain::Initialize(int randomVsOrdered) // 0 = random, 1 = linear
{
	indexCheck[0] = -1; indexCheck[1] = 1; indexCheck[2] = -roomSize; indexCheck[3] = roomSize;
	indexCheck[4] = -Pow(roomSize, 2); indexCheck[5] = Pow(roomSize, 2); // left, right, back, front, bottom, top
	if (randomVsOrdered == 1)
	{
		blockNum = Pow(roomSize, 2);
		GenerateRandom();
	}
	else if (randomVsOrdered == 0)
	{
		blockNum = Pow(roomSize, 2);
		GenerateOrdered();
	}
	else
	{
		GenerateFunctionTerrain();
	}
	HideSides();
}

void Terrain::GenerateFunctionTerrain(void)
{
	int y = 0;

	blockNum = 0;

	int stepSize = roomSize / 10;
	//srand((int)time(nullptr));

	int xScalar = rand() % (roomSize / 5 + 1) + 1;
	int zScalar = rand() % (roomSize / 5 + 1) + 1;
	double xFreq = rand() % 10 + 1;
	double zFreq = rand() % 10 + 1;

	int index;
	for (int x = 0; x < roomSize; x++)
	{
		for (int z = 0; z < roomSize; z++)
		{
			y = (int)(sin(x*xFreq*DEGTORAD + z*zFreq*DEGTORAD)*cos(x*DEGTORAD)*xScalar/2.0 + sin(x*zFreq*DEGTORAD*z*xFreq*DEGTORAD/2.0)*cos(z*DEGTORAD)*zScalar/2.0) + roomSize/4;

			while (y >= 0)
			{
				index = x + roomSize*z + Pow(roomSize, 2)*y;
				std::unique_ptr<Block> newPtr;
				newPtr.reset(new Block(roomSize, x, y, z));
				blockMap[index] = std::move(newPtr);
				y--;
				blockNum++;
			}
		}
	}

	printf("World Initialized\n");
}

Terrain::~Terrain()
{
}

void inline Terrain::GenerateRandom(void)
{
	Block tempBlock;
	for (int i = 0; i < blockNum; i++) // random blocks
	{
		//tempBlock = Block(roomSize);
		std::unique_ptr<Block> newPtr;
		newPtr.reset(new Block(roomSize));
		blockMap[newPtr->index] = std::move(newPtr);
	}
}

void inline Terrain::GenerateOrdered(void) // ordered blocks (starts at "y=0" plane, actually y = roomHeight/2)
{
	//int y = roomSize/2;
	Block tempBlock;
	blockNum = 0;
	for (int y = 0; y < 3; y++)
	{
		for (int z = 0; z < roomSize; z++)
		{
			for (int x = 0; x < roomSize; x++)
			{
				tempBlock = Block(roomSize, x, y, z);
				std::unique_ptr<Block> newPtr;
				newPtr.reset(new Block(roomSize, x, y, z));
				blockMap[tempBlock.index] = std::move(newPtr);
				printf("Index = %d, BlockLocationIndex = %d\n", x + roomSize*z + roomSize*roomSize*y, blockMap[x + roomSize*z + roomSize*roomSize*y]->index);
				blockNum++;
			}
		}
	}
}

void Terrain::HideSides(void)
{
	int visCount = 0;
	int location;
	for (auto &keyValPair : blockMap)
	{
		location = keyValPair.first;
		HideSingleBlockSides(location);
	}

	for (auto &keyValPair : blockMap)
	{
		location = keyValPair.first;
		if (renderMap.find(keyValPair.first) == renderMap.end() && keyValPair.second->renderable == TRUE) // visible object not in the map
		{
			renderMap[location] = location; // add renderable object to the renderMap
		}
		else if (renderMap.find(keyValPair.first) != renderMap.end() && keyValPair.second->renderable == FALSE)
		{
			renderMap.erase(location);
		}
	}
	printf("Total Blocks = %d, Total Visible Blocks = %d\n", blockNum, (int)renderMap.size());
}

void Terrain::HideSingleBlockSides(int i) // for block i in the block-Vector, check the 6 surrounding sides and hide any shared faces
{
	//printf("Hide Blocks around index = %d", i);
	int idx;
	auto &b = blockMap[i];
	b->renderable = FALSE;
	int mostRecentIndex;
	int eReverse;
	for (int e = 0; e < 6; e++)
	{
		eReverse = e + (e % 2 == 0 ? 1 : -1);
		//printf("%d vs %d\n", e, eReverse);
		idx = b->index + indexCheck[e];

		if (blockMap.find(idx) != blockMap.end())
		{
			auto &b1 = blockMap[idx];
			if (BlockDist(b, b1) == 1)
			{
				b->sideVisible[e] = 0;
				b1->sideVisible[eReverse] = 0;
			}
			else
			{
				if (renderMap.find(idx) != renderMap.end())
				{
					printf("Add block %d\n", idx);
					renderMap[idx] = idx;
				}
				b->renderable = TRUE;
			}
		}
		else
		{
			b->renderable = TRUE;
		}
	}
}

void Terrain::ShowSingleBlockSides(int i) // for block i in the block-Vector, check the 6 surrounding sides and show any unshared faces
{
	printf("Show Blocks around index = %d\n", i);
	int idx;
	auto &&b = blockMap[i];
	b->renderable = FALSE;
	int eReverse;
	for (int e = 0; e < 6; e++)
	{
		eReverse = e + (e % 2 == 0 ? 1 : -1);
		printf("eRev = %d\n", eReverse);
		idx = b->index + indexCheck[e];
		
		if (blockMap.find(idx) != blockMap.end())
		{
			auto &&b1 = blockMap[idx];
			if (BlockDist(b, b1) == 1)
			{
				b->sideVisible[e] = 1;
				b1->sideVisible[eReverse] = 1;
				b1->renderable = TRUE;
				b->renderable = TRUE;
				if (renderMap.find(idx) == renderMap.end()) // block not current present
				{
					printf("Add block %d\n", idx);
					renderMap[idx] = idx; // add block
				}
			}
		}
		else
		{
			b->sideVisible[e] = 1;
			b->renderable = TRUE;
		}
	}
}

void Terrain::AddBlock(int x, int y, int z)
{
	int newLocation = x + z*roomSize + y*roomSize*roomSize;
	
	if (blockMap.find(newLocation) == blockMap.end()) // free spot
	{
		std::unique_ptr<Block> newPtr;
		//printf("BEFORE x = %d, y = %d, z = %d\n", newPtr->getX(), newPtr->getY(), newPtr->getZ());
		newPtr.reset(new Block(roomSize, x, y, z));
		printf("AFTER x = %d, y = %d, z = %d\n", newPtr->getX(), newPtr->getY(), newPtr->getZ());
		blockMap[newLocation] = std::move(newPtr);
		printf("AFTER2 x = %d, y = %d, z = %d\n", blockMap[newLocation]->getX(), blockMap[newLocation]->getY(), blockMap[newLocation]->getZ());
		printf("Index = %d, BlockLocationIndex = %d\n", newLocation, blockMap[newLocation]->index);
		blockNum++;

		HideSingleBlockSides(newLocation);

		if (blockMap[newLocation]->renderable == TRUE)
		{
			renderMap[newLocation] = newLocation;
		}
	}
}

void Terrain::RemoveBlock(int x, int y, int z)
{
	int newLocation = x + z*roomSize + y*roomSize*roomSize;
	printf("newLocation = %d\n", newLocation);
	printf("BEFORE renderMap size = %d, blockMap size = %d\n", renderMap.size(), blockMap.size());
	if (blockMap.find(newLocation) != blockMap.end()) // if the removed block is in the list of existing blocks
	{
		printf("In here\n");
		ShowSingleBlockSides(newLocation);
		blockNum--;
		if (renderMap.find(newLocation) != renderMap.end())
		{
			renderMap.erase(newLocation);
		}
		blockMap.erase(newLocation);
		//printf("renderMap size = %d, blockMap size = %d\n", renderMap.size(), blockMap.size());
	}
	else
	{
		//blockMap.erase(blockMap[newLocation].index);
		printf("Out of Bounds \n");
	}
	printf("AFTER renderMap size = %d, blockMap size = %d\n", renderMap.size(), blockMap.size());
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
	if (FsGetKeyState(FSKEY_P) != 1)
	{
		glBegin(GL_QUADS);
		drawCount = 0;
		for (auto &keyVal : renderMap)
		{
			auto &b = blockMap[keyVal.second];
			for (int i = 0; i < 3; i++)
			{
				dist[i] = b->pos[i] - camera.pos[i]; // camera.forwardVector[i];
			}
			//printf("\n");
			coneDist = Dot(dist, camera.forwardVector);
			//printf("coneDist = %lf\n", coneDist);
			if (VecLen(dist) <= 50)
			{
				b->DrawSolid();
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
					b->DrawSolid();
				}
			}
		}
		glEnd();
		glBegin(GL_LINES);
		for (auto &keyVal : renderMap)
		{
			auto &b = blockMap[keyVal.second];
			for (int i = 0; i < 3; i++)
			{
				dist[i] = b->pos[i] - camera.pos[i];
			}
			//printf("\n");
			coneDist = Dot(dist, camera.forwardVector);
			//printf("coneDist = %lf\n", coneDist);
			if (VecLen(dist) <= 50)
			{
				b->DrawEdges();
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
					b->DrawEdges();
					drawCount++;
				}
			}
		}
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		drawCount = 0;
		for (auto &keyVal : blockMap)
		{
			auto &b = keyVal.second;
			for (int i = 0; i < 3; i++)
			{
				dist[i] = b->pos[i] - camera.pos[i];// camera.forwardVector[i];
			}
			//printf("\n");
			coneDist = Dot(dist, camera.forwardVector);
			//printf("coneDist = %lf\n", coneDist);
			if (VecLen(dist) <= 50)
			{
				b->DrawSolid();
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
					b->DrawSolid();
				}
			}
		}
		glEnd();

		glBegin(GL_LINES);
		for (auto &keyVal : blockMap)
		{
			auto &b = keyVal.second;
			for (int i = 0; i < 3; i++)
			{
				dist[i] = b->pos[i] - camera.pos[i];
			}
			coneDist = Dot(dist, camera.forwardVector);
			//printf("coneDist = %lf\n", coneDist);
			if (VecLen(dist) <= 50)
			{
				b->DrawEdges();
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
					b->DrawEdges();
					drawCount++;
				}
			}
		}
		glEnd();
	}
}

void Terrain::DrawTerrain(CameraObject &cameraView, bool reductionMode, int &key)
{
	drawCount = 0;
	// Draw ground lattice
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);

	for (int i = 0; i <= 1024; i += 8)
	{
		glVertex3i(i, 0, 0);
		glVertex3i(i, 0, 1024);

		glVertex3i(0, 0, i);
		glVertex3i(1025, 0, i);
	}
	glColor3ub(255, 0, 0);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 100, 0);

	glEnd();
	cameraView.Update(key);

	if (reductionMode)
	{
		DrawOffsetMode(drawCount, cameraView);
	}
	else
	{
		if (FsGetKeyState(FSKEY_P) == 0) // only render visible objects
		{
			glBegin(GL_QUADS);
			for (auto &keyVal : renderMap)
			{
				blockMap[keyVal.first]->DrawSolid();
				drawCount++;
			}
			glEnd();

			glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			for (auto &keyVal : renderMap)
			{
				blockMap[keyVal.first]->DrawEdges();
			}
			glEnd();
		}
		else
		{
			cameraView.Update(key);
			glBegin(GL_QUADS);
			for (auto &keyVal : blockMap)
			{
				keyVal.second->DrawSolid();
			}
			glEnd();

			glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			for (auto &keyVal : blockMap)
			{
				keyVal.second->DrawEdges();
			}
			glEnd();
		}
	}
}

int main(void)
{
	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent, key = 0;
	int drawCount = 0;
	CameraObject camera, camera2;

	Terrain worldGrid(10, 1);

	camera.playerBlock.roomSize = worldGrid.roomSize;
	camera2.playerBlock.roomSize = worldGrid.roomSize;
	FsOpenWindow(800, 100, WINWID, WINHEI, 1);

	camera.pos[2] = 100.0;
	camera2.pos[0] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera2.pos[1] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera2.pos[2] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera.Update(key);
	camera2.Update(key);
	camera2.stationary = TRUE;

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

		if (!camera.cursorLock)
		{
			camera.cursorLock != camera.cursorLock;
		}
		camera2.cursorLock = camera.cursorLock; // set both camera's to mouse mode

		if (switchCamera)
		{
			worldGrid.DrawTerrain(camera2, reductionMode, key);
		}
		else
		{
			worldGrid.DrawTerrain(camera, reductionMode, key);
		}

		switch (FsGetMouseEvent(lb, mb, rb, mx, my))
		{
		case FSMOUSEEVENT_LBUTTONDOWN:
			printf("IN LEFT BUTTON\n");
			for (int i = 0; i < 5; i++)
			{
				int x = rand() % worldGrid.roomSize;
				int y = rand() % worldGrid.roomSize;
				int z = rand() % worldGrid.roomSize;

				worldGrid.AddBlock(x, y, z);
			}
			break;
		case FSMOUSEEVENT_RBUTTONDOWN:
			printf("IN RIGHT BUTTON\n");
			int randBlockIndex = rand() % worldGrid.blockNum;
			printf("Removing Block %d\n", randBlockIndex);
			if (worldGrid.blockMap.find(randBlockIndex) != worldGrid.blockMap.end())
			{
				auto &b = worldGrid.blockMap[randBlockIndex];
				int x = b->getX(), y = b->getY(), z = b->getZ();
				printf("%d %d %d Trying to remove\n", x, y, z);
				worldGrid.RemoveBlock(x, y, z);
			}
			else
			{
			}
			break;
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
