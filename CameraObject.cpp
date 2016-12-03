#include "CameraObject.h"
#include "fssimplewindow.h"
#if defined(_WIN32_WINNT) // Windows mouse movement routine
#else
#include <ApplicationServices/ApplicationServices.h>
#endif

CameraObject::CameraObject()
{
	roomSize = 100;
	Initialize(roomSize, 0.0, 0.0, 0.0);
}

CameraObject::CameraObject(int roomSize)
{
	Initialize(roomSize, 0.0, 0.0, 0.0);
}

CameraObject::CameraObject(int roomSize, double startX, double startY, double startZ)
{
	Initialize(roomSize, startX, startY, startZ);
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

void CameraObject::Initialize(int roomSizeIn, double startX, double startY, double startZ)
{
	roomSize = roomSizeIn;
	SetVec(pos, startX, startY, startZ);
	dxMove = 0; dyMove = 0; dzMove = 0;
	h = 0;
	p = 0;
	b = 0;
	camHeight = 16.0;
	vertVel = 0.0;
	jumps = 5;
	blockSize = 8;

	jumpVel = sqrt(3.0 * (double)blockSize*-GRAV);
	printf("JumpVel = %lf\n", jumpVel);

	playerBlock.setCoordinate(startX, 0, startY);
	playerBlock.blockSize = 8;

	fov = YsPi / 6.0;  // 30 degree
	nearZ = 1;
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
#if defined(_WIN32_WINNT) // Windows mouse movement routine
	FsGetWindowPosition(winx0, winy0, winx1, winy1);
#else
    FsGetWindowPosition(winx0, winy0);
#endif
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
	}
#else
	CGDirectDisplayID ID = CGMainDisplayID();
	CGRect CurRect;
	CurRect = CGDisplayBounds(ID);
	CGFloat monitorheight = CurRect.size.height;
	CGPoint mouseCenter;

	mouseCenter.x = winx0 + wid / 2;
	mouseCenter.y = monitorheight - winy0 - hei / 2;
//	if (key == FSKEY_TAB)
//	{
//		cursorLock = !cursorLock;
		if (cursorLock)
		{
            if (cursorHidden == 0)
            {
                CGDisplayHideCursor(ID);
                CGDisplayMoveCursorToPoint(ID, mouseCenter);
                cursorHidden = 1;
            }
		}
		else
		{
            if (cursorHidden == 1)
            {
                CGDisplayShowCursor(ID);
                cursorHidden = 0;
            }
		}
//	}

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

		CGWarpMouseCursorPosition(mouseCenter);
		CGAssociateMouseAndMouseCursorPosition(true);
    }
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
		std::vector<double> curFV = { 0,0,0 };
		SetVec(curFV, forwardVector);
		if (gravityOn)
		{
			curFV[1] = 0;
		}
		if (FsGetKeyState(FSKEY_W))
		{
			dxMove += curFV[0] / 1.0;
			dyMove += curFV[1] / 1.0;
			dzMove += curFV[2] / 1.0;
			//VecPlus(pos, curFV);
		}
		if (FsGetKeyState(FSKEY_S))
		{
			dxMove -= curFV[0] / 1.0;
			dyMove -= curFV[1] / 1.0;
			dzMove -= curFV[2] / 1.0;
			//VecMinus(pos, curFV);
		}
		if (FsGetKeyState(FSKEY_D))
		{
			dxMove -= curFV[2] / 1.0;
			dzMove += curFV[0] / 1.0;
		}
		if (FsGetKeyState(FSKEY_A))
		{
			dxMove += curFV[2] / 1.0;
			dzMove -= curFV[0] / 1.0;
		}

		if (gravityOn)
		{
			//printf("Gravity On");

			if (key == FSKEY_SPACE && jumps != 0)
			{
				printf("JUMPING");
				vertVel = jumpVel;
				jumps--;
			}
			else
			{
				vertVel += GRAV;
			}

			dyMove = vertVel;
		}
		else // Gravity Off
		{
			if (FsGetKeyState(FSKEY_1))
			{
				dyMove += 1.0;
			}
			if (FsGetKeyState(FSKEY_2) && pos[1] >= 0)
			{
				dyMove -= 1.0;
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
		hitCheck(blockMap, curFV);
		playerBlock.setPosition(pos[0] - playerBlock.blockSize / 2.0, pos[1], pos[2] - playerBlock.blockSize / 2.0);
		SetGridLocation();
		dxMove = 0; dyMove = 0; dzMove = 0;
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

	DrawCamera();
	SetFrustrumCriteria();
}

void CameraObject::hitCheck(std::map<int, std::unique_ptr<Block>> &blockMap, std::vector<double> &curFV)
{
	double px0, px1, pz0, pz1, py0, py1;
	int cx0, cy0, cz0, checkX, checkY, checkZ;

	px1 = pos[0] + dxMove; // centerblock after move
	py1 = pos[1] + dyMove;
	pz1 = pos[2] + dzMove;

	cx0 = ((int)pos[0] % blockSize) / (blockSize / 4);
	cy0 = pos[1] / blockSize;
	cz0 = ((int)pos[2] % blockSize) / (blockSize / 4);

	checkX = ((int)px1%blockSize) / (blockSize / 4);
	checkY = py1 / blockSize;
	checkZ = ((int)pz1%blockSize) / (blockSize / 4);

	if (dxMove != 0 && cx0 != checkX && (checkX == 0 || checkX == 3))
	{
		int xIdx = xGrid() + (dxMove < 0 ? -1 : 1);
        printf("xindx to check = %d\n",xIdx);
		index = xIdx + zGrid()*roomSize + yGrid()*pow(roomSize, 2);
		if (xIdx >= 0 && xIdx < roomSize && blockMap.find(index) == blockMap.end()) // crouch check needs to be added
		{
			if (camHeight == (playerBlock.blockSize / 2.0) || blockMap.find(index + pow(roomSize, 2)) == blockMap.end()) // if crouching
			{
				pos[0] += dxMove;
			}
			else
			{
				pos[0] = xGrid()*blockSize + (dxMove < 0 ? +blockSize / 4.0 : blockSize*3.0 / 4.0);
			}
		}
		else
		{
			pos[0] = xGrid()*blockSize + (dxMove < 0 ? +blockSize / 4.0 : blockSize*3.0 / 4.0);
		}
	}
	else
	{
		pos[0] += dxMove;
	}

	if (dzMove != 0 && cz0 != checkZ && (checkZ == 0 || checkZ == 3)) // check Z direction
	{
		int zIdx = zGrid() + (dzMove < 0 ? -1 : 1);
		index = xGrid() + zIdx*roomSize + yGrid()*pow(roomSize, 2);
		if (zIdx >= 0 && zIdx < roomSize && blockMap.find(index) == blockMap.end())
		{
			if (camHeight == (playerBlock.blockSize / 2.0) || blockMap.find(index + pow(roomSize, 2)) == blockMap.end()) // crouch check needs to be added
			{
				pos[2] += dzMove;
			}
			else
			{
				pos[2] = zGrid()*blockSize + (dzMove < 0 ? +blockSize / 4.0 : blockSize*3.0 / 4.0);
			}
		}
		else
		{
			pos[2] = zGrid()*blockSize + (dzMove < 0 ? +blockSize / 4.0 : blockSize*3.0 / 4.0);
		}
	}
	else
	{
		pos[2] += dzMove;
	}

	if (dyMove != 0 && cy0 != checkY) // check Y direction, only for falling
	{
		int yIdx = yGrid() + (dyMove < 0 ? -1 : 2);
		index = xGrid() + zGrid()*roomSize + yIdx*pow(roomSize, 2);
		if (dyMove <= 0) // falling or standing
		{
			if (blockMap.find(index) != blockMap.end()) // hit floor
			{
				jumps = 5;
				vertVel = 0;
				pos[1] = yGrid()*blockSize;
			}
			else
			{
				pos[1] += dyMove;
			}
			if (dyMove == GRAV && blockMap.find(index + 2 * pow(roomSize, 2)) != blockMap.end()) // stay crouched if there's a block right above you
			{
				camHeight = blockSize / 2.0;
			}
		}
		else // jumping
		{
			if (camHeight == blockSize / 2.0) // crouching
			{
				if (blockMap.find(index) != blockMap.end()) // hit ceiling crouched
				{
					vertVel = 0;
					pos[1] = (yGrid()+1)*blockSize;
				}
				else
				{
					pos[1] += dyMove;
				}
			}
			else
			{
				if (blockMap.find(index) != blockMap.end() || blockMap.find(index + pow(roomSize, 2)) != blockMap.end()) // hit ceiling un-crouched
				{
					vertVel = 0;
					pos[1] = yGrid()*blockSize;
				}
				else
				{
					pos[1] += dyMove;
				}
			}
		}
	}
	else // didn't cross boundary
	{
		if (dyMove > 0 && camHeight != blockSize/2.0 && blockMap.find(xGrid() + zGrid()*roomSize + (yGrid() + 2)*pow(roomSize, 2)) != blockMap.end())
		{
			vertVel = 0;
			pos[1] = yGrid()*blockSize;
		}
		else
		{
			pos[1] += dyMove;
		}
	}
}
