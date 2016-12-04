#ifndef CAMERA_CLASS_INCLUDED
#define CAMERA_CLASS_INCLUDED

#include "MiscFunctions.h"

#if defined(_WIN32_WINNT) // Windows mouse movement routine
#else
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULLINT
#define NULLINT -1
#endif

class CameraObject
{
public:
	double grav = 0;
	int roomSize = 0;
	int index = 0;
	double h, p, b;
	double dxMove, dyMove, dzMove;
	double jumpVel;
	double vertVel, camHeight, zoom, viewRadius, sensitivity, fov, nearZ, farZ, coneAngle;
	std::vector<double> forwardVector = { 0,0,0 };
	std::vector<double> pos = { 0,0,0 };
	std::vector<int> gridLocation = { 0,0,0 };
	std::vector<double> curFV = { 0,0,0 };

	int blockSize, jumps = 5;
	int health, maxHealth;
	int lb, mb, rb, mx, my, mouseEvent, key, wid, hei, winx0, winy0, winx1, winy1;
	int mouseOffsetX = 0, mouseOffsetY = 0;
	int activeTool = NULLINT;
	bool cursorLock = FALSE;
    bool cursorHidden = FALSE;
	bool stationary = FALSE;
	bool gravityOn = FALSE;

	double x(void); // pos vector items
	double y(void);
	double z(void);
	double xF(void) const; // forward vector items
	double yF(void) const;
	double zF(void) const;
	int xGrid(void) const;
	int yGrid(void) const;
	int zGrid(void) const;

	CameraObject();
	CameraObject(int roomSize);
	CameraObject(int roomSize, double xLoc, double yLoc, double zLoc);

	void Initialize(int roomSize, double startX, double startY, double startZ);
	void SetUpCameraProjection(void);
	void SetUpCameraTransformation(void);
	void DrawCamera(void);
	int findEnemy(int range, const Block &enemy);
	void hitCheck(std::map<int, std::unique_ptr<Block>> &blockMap, std::vector<double> &curFV);
	Block playerBlock;

	void GetForwardVector(void);
	void inline SetGridLocation(void);
	void inline SetFrustrumCriteria(void);
	void Update(int &key,std::map<int, std::unique_ptr<Block>> &blockMap);
};

#endif // !CAMERA_CLASS_INCLUDED
