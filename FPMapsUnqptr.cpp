#include "Terrain.h"
#include "Decoder.h"

int main(void)
{
	FsOpenWindow(800, 100, WINWID, WINHEI, 1);

	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent, key = 0;
	int drawCount = 0;
	CameraObject camera, camera2;
	Terrain worldGrid(100, 2);
	materialBlock matBlock;
	worldGrid.texId=decodePng();

	//Generate Materials
	//Sample code to just generate 1 material block
	matBlock.setImage(5, 0);
	matBlock.setParam(1, 1);
	matBlock.setColor(255, 0, 0);
	matBlock.setXYZ(17, 16, 15);
	worldGrid.GenerateMaterial(matBlock);



	camera.playerBlock.roomSize = worldGrid.roomSize;
	camera2.playerBlock.roomSize = worldGrid.roomSize;

	camera.pos[2] = 100.0;
	camera2.pos[0] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera2.pos[1] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera2.pos[2] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera.Update(key);
	camera2.Update(key);
	camera2.stationary = TRUE;

	bool reductionMode = false;
	bool switchCamera = false;
	bool texturesOn = false;
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // vertices of any object's front face (aka outside face) should always be specified in CLOCKWISE order
	glCullFace(GL_BACK); // back 3 sides of each block are automatically removed by this openGL culling function

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
			break;
		case FSKEY_C:
			switchCamera = !switchCamera;
			break;
		case FSKEY_3:
			camera.gravityOn = !camera.gravityOn;
			break;
		case FSKEY_T:
			texturesOn = !texturesOn;
			if (texturesOn)
			{
				glEnable(GL_TEXTURE_2D);		// Turn on the texture mapping 
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}
			break;
		case FSKEY_TAB:
			camera.cursorLock = !camera.cursorLock;
			camera2.cursorLock = camera.cursorLock;
			break;
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

		if (switchCamera)
		{
			worldGrid.DrawTerrain(camera2, reductionMode, key, texturesOn);
		}
		else
		{
			worldGrid.DrawTerrain(camera, reductionMode, key, texturesOn);
		}

		switch (FsGetMouseEvent(lb, mb, rb, mx, my))
		{
			int xGrid, yGrid, zGrid;
		case FSMOUSEEVENT_LBUTTONDOWN:
			printf("IN LEFT BUTTON\n");
			if (worldGrid.FindBlock(camera, xGrid, yGrid, zGrid, ADD))
			{
				if (xGrid != camera.xGrid() || (yGrid != camera.yGrid() && yGrid != camera.yGrid() + 1) || zGrid != camera.zGrid())
				{
					printf("Found one at %d %d %d\n", xGrid, yGrid, zGrid);
					worldGrid.AddBlock(xGrid, yGrid, zGrid,0,0,0,0,0);		//right now default add dirt blocks
				}
			}
			else
			{
				printf("Did Not Find");
			}
			break;
		case FSMOUSEEVENT_RBUTTONDOWN:
			printf("IN RIGHT BUTTON\n");
			if (worldGrid.FindBlock(camera, xGrid, yGrid, zGrid, REMOVE))
			{
				if (xGrid != camera.xGrid() || (yGrid != camera.yGrid() && yGrid != camera.yGrid() + 1) || zGrid != camera.zGrid())
				{
					printf("Found one at %d %d %d\n", xGrid, yGrid, zGrid);
					worldGrid.RemoveBlock(xGrid, yGrid, zGrid);
				}
			}
			else
			{
				printf("Did Not Find");
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
		{ // draw crosshairs
			glLineWidth(1);
			glBegin(GL_LINES);
			glLineWidth(5);
			glColor3ub(255, 255, 255);
			glVertex2i(wid / 2 - 10, hei / 2);
			glVertex2i(wid / 2 + 10, hei / 2);
			glVertex2i(wid / 2, hei / 2 - 10);
			glVertex2i(wid / 2, hei / 2 + 10);
			glEnd();

			glLineWidth(5);
			glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			glVertex2i(wid / 2 - 12, hei / 2);
			glVertex2i(wid / 2 + 12, hei / 2);
			glVertex2i(wid / 2, hei / 2 - 12);
			glVertex2i(wid / 2, hei / 2 + 12);
			glEnd();
			glLineWidth(1);
		}

		glDisable(GL_DEPTH_TEST);

		// 2D drawing from here
		FsSwapBuffers();
		FsSleep(5);
	}

	return 0;
}