#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"

const double YsPi = 3.1415927;
const int WINWID = 800, WINHEI = 600;
const double GRAV = -.06;

void DrawGroundLattice(void)
{
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);

	for (int i = -1000; i <= 1000; i += 20)
	{
		glVertex3i(i, 0, -1000);
		glVertex3i(i, 0, +1000);

		glVertex3i(-1000, 0, i);
		glVertex3i(+1000, 0, i);
	}

	glEnd();
}

class CameraObject
{
public:
	double x, y, z;
	double h, p, b;
	double py, pvy;
	double vx, vy, vz;
	int cursorLock = 0;
	int lb, mb, rb, mx, my, mouseEvent, key, wid, hei, winx0, winy0;

	double fov, nearZ, farZ;
	double sensitivity;
	CameraObject();
	void Initialize(void);
	void SetUpCameraProjection(void);
	void SetUpCameraTransformation(void);

	void GetForwardVector(double &vx, double &vy, double &vz);
	void Update(int key);
};

CameraObject::CameraObject()
{
	Initialize();
}

void CameraObject::Initialize(void)
{
	x = 0;
	y = 0;
	z = 0;
	h = 0;
	p = 0;
	b = 0;

	py = 30;
	pvy = 0;

	vx = 0;
	vy = 0;
	vz = 0;

	fov = YsPi / 6.0;  // 30 degree
	nearZ = 1.0;
	farZ = 1000.0;

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
}

void CameraObject::GetForwardVector(double &vx, double &vy, double &vz)
{
	vx = -cos(p)*sin(h);
	vy = sin(p);
	vz = -cos(p)*cos(h);
}

void CameraObject::Update(int key)
{
	FsPollDevice();
	
	mouseEvent = FsGetMouseEvent(lb, mb, rb, mx, my);
	FsGetWindowPosition(winx0, winy0);
	FsGetWindowSize(wid, hei);
	GetForwardVector(vx, vy, vz);

	printf("key = %d", key);

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
			printf("dx = %lf, dy = %lf\n", dx, dy);
			dh = dx / 240 * sensitivity;
			dp = dy / 240 * sensitivity;
			printf("dh = %lf, dp = %lf\n", dh, dp);
			h += dh;
			p += dp;
			printf("cameraH = %lf, cameraP = %lf\n", h, p);
			SetCursorPos(winx0 + wid / 2, winy0 + hei / 2);
		}
		else
		{
			printf("mx = %d, my = %d\n", mx, my);
		}
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

	if (py == 30)
	{
		if (0 != FsGetKeyState(FSKEY_SPACE))
		{
			pvy = 2;
		}
	}
	if (0 != FsGetKeyState(FSKEY_W))
	{
		x += vx / 1;
		z += vz / 1;
	}
	if (0 != FsGetKeyState(FSKEY_S))
	{
		x -= vx / 2.0;
		z -= vz / 2.0;
	}
	if (0 != FsGetKeyState(FSKEY_D))
	{
		x -= vz / 1;
		z += vx / 1;
	}
	if (0 != FsGetKeyState(FSKEY_A))
	{
		x += vz / 1;
		z -= vx / 1;
	}
	if (pvy != 0)
	{
		py = py + pvy;
		pvy += GRAV;
	}
	if (py<30)
	{
		py = 30;
		pvy = 0;
	}
	y = py;
}

int main(void)
{
	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent;

	CameraObject camera;

	FsOpenWindow(16, 16, WINWID, WINHEI, 1);

	while (0 == terminate)
	{
		FsPollDevice();

		int wid, hei;
		FsGetWindowSize(wid, hei);

		int key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			terminate = 1;
			break;
		}

		camera.Update(key);

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

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		// 2D drawing from here

		FsSwapBuffers();
		FsSleep(10);
	}

	return 0;
}
