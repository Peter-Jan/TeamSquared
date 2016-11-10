#include <vector>
#include "Block.h"
#include "fssimplewindow.h"

const double YsPi = 3.1415927;



class CameraObject
{
public:
	double x, y, z;
	double h, p, b;
	double px, py, pz, pvy;
	double vx, vy, vz;
	double camHeight;
	double zoom;
	std::vector<double> forwardVector = { 0,0,0 };
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