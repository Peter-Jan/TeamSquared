#ifndef CAMERA_CLASS_INCLUDED
#define CAMERA_CLASS_INCLUDED

#include "MiscFunctions.h"

#if defined(_WIN32_WINNT) // Windows mouse movement routine
#else
#include <ApplicationServices/ApplicationServices.h>
#endif

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


#endif // !CAMERA_CLASS_INCLUDED
