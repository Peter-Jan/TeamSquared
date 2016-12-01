#include "CameraObject.h"


CameraObject::CameraObject()
{
	Initialize(0.0, 0.0, 0.0);
}

CameraObject::CameraObject(double startX, double startY, double startZ)
{
	Initialize(startX, startY, startZ);
}

double CameraObject::x(void)
{
	return pos[0];
}

double CameraObject::y(void)
{
	return pos[1];
}

double CameraObject::z(void)
{
	return pos[2];
}

double CameraObject::xF(void) const
{
	return forwardVector[0];
}

double CameraObject::yF(void) const
{
	return forwardVector[1];
}

double CameraObject::zF(void) const
{
	return forwardVector[2];
}

int CameraObject::xGrid(void) const
{
	return gridLocation[0];
}

int CameraObject::yGrid(void) const
{
	return gridLocation[1];
}

int CameraObject::zGrid(void) const
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
	nearZ = 2;
	farZ = 1000.0;
	viewRadius = 1000;
	zoom = 0.0;

	sensitivity = 1.0 / 5.0;
	SetFrustrumCriteria();
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
	glTranslated(-pos[0], -pos[1] - camHeight, -pos[2]);
	glTranslated(forwardVector[0] * zoom, forwardVector[1] * zoom, forwardVector[2] * zoom);
}

void CameraObject::GetForwardVector(void)
{
	forwardVector[0] = -cos(p)*sin(h);
	forwardVector[1] = sin(p);
	forwardVector[2] = -cos(p)*cos(h);
	//printf("%lf %lf %lf", forwardVector[0], forwardVector[1], forwardVector[2]);
}

void inline CameraObject::SetGridLocation(void)
{
	gridLocation[0] = pos[0] / blockSize;
	gridLocation[1] = pos[1] / blockSize;
	gridLocation[2] = pos[2] / blockSize;
	playerBlock.y1 = pos[1] + camHeight + blockSize / 2;
}

void CameraObject::DrawCamera(void)
{
	// Draw Forward Vector
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
	glVertex3d(playerBlock.xM, playerBlock.yM, playerBlock.zM);
	glVertex3d(playerBlock.xM + forwardVector[0] * 100.0,
			   playerBlock.yM + forwardVector[1] * 100.0,
			   playerBlock.zM + forwardVector[2] * 100.0);
	glEnd();

	if (zoom > 5)
	{
		// Draw Camera Cube
		glBegin(GL_QUADS);
		playerBlock.DrawSolid();
		glEnd();
		glBegin(GL_QUADS);
		playerBlock.DrawEdges();
		glEnd();
	}
}
void CameraObject::SetFrustrumCriteria(void)
 {
	double fVLen = fabs(farZ - zoom);
	coneAngle = fVLen / sqrt(pow(viewRadius, 2) + pow(fVLen, 2));
	}

void CameraObject::Update(int &key, std::map<int, std::unique_ptr<Block>> &blockMap)
{
	FsPollDevice();
	mouseEvent = FsGetMouseEvent(lb, mb, rb, mx, my);
	FsGetWindowPosition(winx0, winy0, winx1, winy1);
	FsGetWindowSize(wid, hei);
	mouseOffsetX = (winx1 - winx0 - wid) / 2;
	mouseOffsetY = (winy1 - winy0 - hei) - mouseOffsetX;
	//printf("MouseOffX = %d, MouseOffY = %d\n", mouseOffsetX, mouseOffsetY);
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
	if (cursorLock)
	{
		if (ShowCursor(TRUE) > -1)
		{
			ShowCursor(FALSE);
			ShowCursor(FALSE);
		}
		else
		{
			ShowCursor(FALSE);
		}

		SetCursorPos(winx0 + wid / 2, winy0 + hei / 2);
		//FsGetMouseEvent(lb, mb, rb, mx, my);
		//mouseOffsetX = (mx - wid / 2);
		//mouseOffsetY = (my - hei / 2);
	}
	else
	{
		if (ShowCursor(FALSE) < 1)
		{
			ShowCursor(TRUE);
			ShowCursor(TRUE);
		}
		else
		{
			ShowCursor(TRUE);
		}
	}

	if (mouseEvent == FSMOUSEEVENT_MOVE) // mouseView-control
	{
		if (cursorLock)
		{
			double dx, dy, dh, dp;
			int wid, hei, offX, offY;
			FsGetWindowSize(wid, hei);
			dx = (double)((wid / 2 - mouseOffsetX) - mx);
			dy = (double)((hei / 2 - mouseOffsetY) - my);
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
		dx = wid / 2 - mx;
		dy = hei / 2 - my - 1;
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
		std::vector<double> delta = { 0,0,0 };
		std::vector<double> curFV = { 0,0,0 };
		SetVec(curFV, forwardVector);
		if (gravityOn)
		{
			curFV[1] = 0;
		}
		if (FsGetKeyState(FSKEY_W))
		{
			dx += curFV[0];
			dy += curFV[1];
			dz += curFV[2];
			//VecPlus(pos, curFV);
		}
		if (FsGetKeyState(FSKEY_S))
		{
			dx -= curFV[0];
			dy -= curFV[1];
			dz -= curFV[2];
			//VecMinus(pos, curFV);
		}
		if (FsGetKeyState(FSKEY_D))
		{
			dx -= curFV[2];
			dz += curFV[0];
		}
		if (FsGetKeyState(FSKEY_A))
		{
			dz += curFV[2];
			dx -= curFV[0];
		}
		hitCheck(blockMap, curFV);

		if (gravityOn)
		{
			//printf("Gravity On");
			index = (xGrid()) + (zGrid()*roomSize) + ((yGrid() - 1)*pow(roomSize, 2));
			int index1 = (xGrid()) + (zGrid()*roomSize) + (yGrid()*pow(roomSize, 2));

			if (blockMap.find(index) != blockMap.end())
			{
				printf("GROUNDED");
				pos[1] = yGrid() * 8;
				vertVel = 0.0;
			}
			if (key == FSKEY_SPACE)
			{
				printf("JUMPING");
				vertVel += 4.0;
				pos[1] += vertVel;
				//jumps--;
			}

			if (blockMap.find(index) == blockMap.end())
			{

				printf("vertVel: %lf \n", vertVel);

				vertVel += GRAV;
				pos[1] += vertVel;

				printf("FALLING");
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
			camHeight = 12.0;
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
	SetFrustrumCriteria();
}

void CameraObject::hitCheck(std::map<int, std::unique_ptr<Block>> &blockMap, std::vector<double> &curFV)
{
	double xmid, zmid, x1mid, z1mid, ymid, y1mid;
	double px0, px1, pz0, pz1, py0, py1;
	int xleft = 0;
	int xright = 0;
	int yleft = 0;
	int yright = 0;
	int zleft = 0;
	int zright = 0;

	x1mid = pos[0] + curFV[0];
	z1mid = pos[2] + curFV[2];

	px0 = ((int)pos[0] % blockSize) / (blockSize / 4);
	pz0 = ((int)pos[2] % blockSize) / (blockSize / 4);
	px1 = ((int)x1mid%blockSize) / (blockSize / 4);
	pz1 = ((int)z1mid%blockSize) / (blockSize / 4);


	//printf("px0: %lf pz0: %lf, px1: %lf, pz1: %lf \n", px0,pz0,px1,pz1);
	//printf("x: %lf, z: %lf \n", x1mid,z1mid);
	if (px1 != px0 || pz1 != pz0)
	{
		if (px1 == 0)
		{
			index = (xGrid() - 1) + (zGrid()*roomSize) + (yGrid()*pow(roomSize, 2));
			if (blockMap.find(index) == blockMap.end())
			{
				pos[0] += dx;
			}
		}

		if (px1 == 3)
		{
			index = (xGrid() + 1) + (zGrid()*roomSize) + (yGrid()*pow(roomSize, 2));
			if (blockMap.find(index) == blockMap.end())
			{
				pos[0] += dx;
			}
		}

		if (pz1 == 0)
		{
			index = xGrid() + ((zGrid() - 1)*roomSize) + (yGrid()*pow(roomSize, 2));
			if (blockMap.find(index) == blockMap.end())
			{
				pos[2] += dz;
			}
		}

		if (pz1 == 3)
		{
			index = (xGrid()) + ((zGrid() + 1)*roomSize) + (yGrid()*pow(roomSize, 2));
			if (blockMap.find(index) == blockMap.end())
			{
				pos[2] += dz;
			}
		}


		//index = (xGrid()) + (zGrid() *roomSize) + ((yGrid()-1)*pow(roomSize, 2));
		//if (blockMap.find(index) == blockMap.end())
		//{
		//	gravityOn = TRUE;
		//}
	}
}