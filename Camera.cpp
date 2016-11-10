#include "Camera.h"


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
		farZ = min(farZ - 5, 1000.0);
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

