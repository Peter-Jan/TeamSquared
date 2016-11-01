#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fssimplewindow.h"

const int WINWID = 800, WINHEI = 600;
#define NULLINT -1

class Grid
{
public:
	int *cell;
	int activeCell = -1;
	int tempColor = 0;
	int xLeft, yTop, xRight, yBottom, rows, cols, border;
	double cellWidth, cellHeight, xBorder, yBorder;
	Grid();
	~Grid();
	Grid(int x0, int y0, int x1, int y1, int numObjects);
	//Grid(int xLeft, int yTop, int xRight, int yBottom, int rows, int cols, int borderSize);
	void CleanUp(void);
	void Draw(void);
	int CheckClick(int &mx, int &my);
};

Grid::Grid()
{
	if (cell != nullptr)
	{
		delete[] cell;
		cell = nullptr;
	}
}

Grid::Grid(int x0, int y0, int x1, int y1, int numObjects)
{
	double gridWidth = abs(x1 - x0);
	double gridHeight = abs(y1 - y0);
	//printf("%d, %d\n", gridWidth, gridHeight);
	double crRatio = gridWidth / gridHeight;
	border = 5;
	xLeft = x0; yTop = y0; xRight = x1; yBottom = y1;
	//printf("ColRow Ratio = %lf\n", crRatio);
	rows = round(sqrt((double)numObjects/crRatio));
	cols = floor((double)numObjects / (double)rows);
	int cellX, cellY;
	printf("%d, %d\n", rows, cols);
	for (;;)
	{
		//printf("%d, %d\n", rows, cols);
		cellX = (double)(gridWidth - (cols + 1)*border) / (double) cols;
		cellY = (double)(gridHeight - (rows + 1)*border) / (double) rows;
		//printf("cellWidth = %lf, cellHeight = %lf\n", cellX, cellY);

		cellWidth = min(cellX, cellY);
		cellHeight = cellWidth;

		xBorder = (gridWidth - cellWidth*cols - border*(cols - 1)) / 2;
		yBorder = (gridHeight - cellHeight*rows - border*(rows - 1)) / 2;
		printf("xBorder = %lf, yBorder = %lf\n", xBorder, yBorder);
		if (rows*cols < numObjects)
		{
			if (xBorder > yBorder)
			{
				cols++;
			}
			else
			{
				rows++;
			}
		}
		else
		{
			break;
		}
	}

	int numCells = rows*cols;
	cell = new int[numCells];
	for (int i = 0; i < rows*cols; i++)
	{
		if (i < numObjects)
		{
			cell[i] = i*5;
		}
		else
		{
			cell[i] = 0;
		}
	}
}

void Grid::Draw(void)
{
	int x0, y0, x1, y1;
	glColor3ub(255, 0, 0);
	glBegin(GL_QUADS);

	glVertex2i(xLeft, yTop);
	glVertex2i(xRight, yTop);
	glVertex2i(xRight, yBottom);
	glVertex2i(xLeft, yBottom);
	glEnd();
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			x0 = (int)(xLeft + xBorder + i*(cellWidth + border));
			y0 = (int)(yTop + yBorder + j*(cellHeight + border));
			x1 = x0 + cellWidth;
			y1 = y0 + cellHeight;
			//printf("%d, %d, %d, %d\n", x0, y0, x1, y1);
			glBegin(GL_QUADS);
			glColor3ub(0,0,cell[j*cols + i]);
			glVertex2i(x0, y0);
			glVertex2i(x1, y0);
			glVertex2i(x1, y1);
			glVertex2i(x0, y1);
			glEnd();
		}
	}
}

Grid::~Grid()
{
	CleanUp();
}

void Grid::CleanUp(void)
{
	if (cell != nullptr)
	{
		delete[] cell;
		cell = nullptr;
	}
}

int Grid::CheckClick(int &mx, int &my)
{
	printf("%d, %d\n", mx, my);
	if (xLeft < mx && xRight > mx && yTop < my && yBottom > my)
	{
		int xLoc, yLoc;
		xLoc = mx - xLeft - xBorder;
		yLoc = my - yTop - yBorder;

		xLoc = (int)(xLoc / (cellWidth + border));
		yLoc = (int)(yLoc / (cellHeight + border));

		printf("Cell clicked = (%d, %d)\n", xLoc, yLoc);
		if (activeCell == NULLINT)
		{
			activeCell = (yLoc*cols + xLoc);
			tempColor = cell[yLoc*cols + xLoc];
			cell[yLoc*cols + xLoc] = 255;
		}
		else
		{
			cell[activeCell] = cell[yLoc*cols + xLoc];
			cell[yLoc*cols + xLoc] = tempColor;
			printf("Swap cell %d with cell %d", activeCell, (yLoc*cols + xLoc));
			activeCell = NULLINT;
		}
		return 1;
	}
	printf("Active Cell = %d", activeCell);
	if (activeCell != NULLINT)
	{
		printf("IN HERE ALLLAASFDAHGF");
		cell[activeCell] = tempColor;
		tempColor = 0;
		activeCell = NULLINT;
	}
	return 0;
}

int main(void)
{
	FsOpenWindow(800, 100, WINWID, WINHEI, 1);

	Grid rectGrid(200,100,700,400,40);
	int lb, mb, rb, mx, my, eventType;
	while (FsInkey() != FSKEY_ESC)
	{
		FsPollDevice();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		eventType = FsGetMouseEvent(lb, mb, rb, mx, my);
		if (eventType == FSMOUSEEVENT_LBUTTONDOWN)
		{
			rectGrid.CheckClick(mx, my);
		}

		rectGrid.Draw();

		FsSwapBuffers();
		FsSleep(50);
	}
	FsCloseWindow();
	getchar();
	return 0;
}