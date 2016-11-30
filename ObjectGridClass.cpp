#include "ObjectGridClass.h"

const int WINWID = 800, WINHEI = 600;
#define NULLINT -1

void glColor3ubArray(int colorArray[3])
{
	glColor3ub(colorArray[0], colorArray[1], colorArray[2]);
}

Item::Item()
{
}

Item::~Item()
{
}

void Item::Increase(void)
{
	quantity++;
}

void Item::Decrease(void)
{
	quantity--;
}

void Item::Increase(int quant)
{
	quantity += quant;
}

void Item::Decrease(int quant)
{
	quantity -= quant;
}

void Item::Draw(int x0, int y0, int x1, int y1)
{
	char quant[10];
	glColor3ub(255, 255, 255);
	glRasterPos2d((double)x0 + 10, (double)y0 + 14);
	YsGlDrawFontBitmap6x7(name);
	glColor3ub(255, 255, 255);
	glRasterPos2d((double)x1 - 10, (double)y1 - 10);
	YsGlDrawFontBitmap10x14(itoa(quantity, quant, 9));

	if (highlight)
	{
		glColor3ub(0, 255, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(x0, y0);
		glVertex2i(x1, y0);
		glVertex2i(x1, y1);
		glVertex2i(x0, y1);
		glEnd();
	}

	glColor3ubArray(color);
	glBegin(GL_QUADS);
	glVertex2i(x0, y0);
	glVertex2i(x1, y0);
	glVertex2i(x1, y1);
	glVertex2i(x0, y1);
	glEnd();


}

Material::Material()
{
	fgets(name, 255, stdin);
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = 1;
	stackable = TRUE;
}

Material::~Material()
{
	CleanUp();
}

Material::Material(char perm[],int q)
{
	strcpy(name, perm);
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = q;
	stackable = TRUE;
}

void Material::CleanUp(void)
{
}

Recipe::Recipe()
{

}

Axe::Axe()
{
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = 1;
	char nameStart[255] = "Axe";
	strcpy(name, nameStart);
	std::unique_ptr<Item> randomItem(new Material("Wood",2));
	materialList.push_back(nullptr);
	materialList[0].swap(randomItem);
	randomItem.reset(new Material("Stone", 2));
	materialList.push_back(nullptr);
	materialList[1].swap(randomItem);
	randomItem.release();
}

Axe::~Axe()
{
}

Hammer::Hammer()
{
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = 1;
	char nameStart[255] = "Hammer";
	strcpy(name, nameStart);
	std::unique_ptr<Item> randomItem(new Material("Wood", 10));
	materialList.push_back(nullptr);
	materialList[0].swap(randomItem);
	randomItem.reset(new Material("Stone", 3));
	materialList.push_back(nullptr);
	materialList[1].swap(randomItem);
	randomItem.release();
}

Hammer::~Hammer()
{
}

Grid::Grid()
{
}

Grid::~Grid()
{
	CleanUp();
}

void Grid::CleanUp(void)
{

	printf("Cleaning");
	gridVec.clear();
	

}

bool Grid::AddElement(std::unique_ptr<Item> &item)
{
	int i = -1;
	for (auto &elem : gridVec)
	{
		i++;
		if (elem == nullptr)
		{
			elem = std::move(item);
			printf("New item added to cell %d\n", i);
			return TRUE;
		}
	}
	printf("Inventory full\n");
	return FALSE;
}

bool Grid::AddElement(int elemIndex, std::unique_ptr<Item> &item)
{
	auto &c = gridVec[elemIndex];
	c = std::move(item);
	return TRUE;
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
	rows = round(sqrt((double)numObjects / crRatio));
	cols = floor((double)numObjects / (double)rows);
	int cellX, cellY;
	printf("%d, %d\n", rows, cols);
	for (;;)
	{
		//printf("%d, %d\n", rows, cols);
		cellX = (double)(gridWidth - (cols + 1)*border) / (double)cols;
		cellY = (double)(gridHeight - (rows + 1)*border) / (double)rows;
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

	for (int i = 0; i < numCells; i++)
	{
		gridVec.push_back(nullptr);
	}
}

void Grid::Draw(void)
{
	int x0, y0, x1, y1, index;

	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			index = cols*j + i;
			x0 = (int)(xLeft + xBorder + i*(cellWidth + border));
			y0 = (int)(yTop + yBorder + j*(cellHeight + border));
			x1 = x0 + cellWidth;
			y1 = y0 + cellHeight;

			if (gridVec[index] == nullptr) // draw empty cell
			{
				glBegin(GL_QUADS);
				glColor3ub(200, 200, 200);
				glVertex2i(x0, y0);
				glVertex2i(x1, y0);
				glVertex2i(x1, y1);
				glVertex2i(x0, y1);
				glEnd();
			}
			else // draw item icon
			{
				auto &elem = *gridVec[index];
				elem.Draw(x0, y0, x1, y1);
			}
		}
	}
	glColor3ubArray(backgroundColor);
	glBegin(GL_QUADS);

	glVertex2i(xLeft, yTop);
	glVertex2i(xRight, yTop);
	glVertex2i(xRight, yBottom);
	glVertex2i(xLeft, yBottom);
	glEnd();
}

void Grid::MoveCell(std::unique_ptr<Item> &origin, std::unique_ptr<Item> &destination)
{
	auto &orig = *origin, &dest = *destination;
	orig.highlight = FALSE;
	if (destination == nullptr || strcmp(orig.name, dest.name) || !orig.stackable || !dest.stackable)
	{
		origin.swap(destination);
	}
	else
	{
		dest.Increase(orig.quantity);
		origin.release();
	}
	activeCell = NULLINT;
}

bool Grid::InsideBounds(int &mx, int &my)
{
	return xLeft + xBorder < mx && xRight - xBorder > mx && yTop + yBorder < my && yBottom - yBorder > my;
}

void Grid::TryTransfer(int &mx, int &my, Grid &other)
{
	printf("Try transferring");
	other.transfer = TRUE;
	gridVec[activeCell]->highlight = FALSE;
	int destIndex = other.CheckClick(mx, my);
	printf("dest index = %d\n", destIndex);
	if (destIndex >= 0 && destIndex < other.rows*other.cols)
	{
		printf("Before Transfer");
		MoveCell(gridVec[activeCell], other.gridVec[destIndex]);
		printf("After Transfer");
	}
	printf("Past");
	activeCell = NULLINT;
	other.activeCell = NULLINT;
	transfer = FALSE;
	other.transfer = FALSE;
}

int Grid::CheckClick(int &mx, int &my)
{
	int index;
	printf("%d, %d\n", mx, my);
	if (InsideBounds(mx, my))
	{
		int xLoc, yLoc;
		xLoc = mx - xLeft - xBorder;
		yLoc = my - yTop - yBorder;

		xLoc = (int)(xLoc / (cellWidth + border));
		yLoc = (int)(yLoc / (cellHeight + border));

		index = yLoc*cols + xLoc;
		if (transfer)
		{
			return index;
		}
		printf("Cell clicked = (%d, %d)\n", xLoc, yLoc);
		if (activeCell == NULLINT)
		{
			if (gridVec[index] == nullptr) // Generate new item (in the game, this should be removed)
			{
				printf("Enter new item name >\n");
				std::unique_ptr<Item> tempP(new Material);
				AddElement(index, tempP);
			}
			else // if the cell is occupied and there is currently no active cell, highlight it
			{
				activeCell = index;
				gridVec[index]->highlight = TRUE;
			}
		}
		else
		{
			printf("Gonna Crash\n");
			MoveCell(gridVec[activeCell], gridVec[index]); // pass Item obj pointers to MoveCell func
			printf("Swap cell %d with cell %d\n", activeCell, index);
		}
		return -1;
	}
	else if (activeCell != NULLINT)
	{
		printf("Cell");
		transfer = TRUE;
		return -2;
	}
}

//void Grid::AddPermElement()
//{
//	std::unique_ptr<Item> Axe(new Material("Axe",1));
//	this->AddElement(Axe);
//	std::unique_ptr<Item> Plank(new Material("Plank",1));
//	this->AddElement(Plank);
//}

void Grid::AddPermElement()
{
	printf("Made it\n");
	std::unique_ptr<Item> recipe(new Axe);
	for (auto &elem : gridVec)
	{
		if (elem == nullptr)
		{
			elem.swap(recipe);
		}
	}
	recipe.reset(new Hammer);
	for (auto &elem : gridVec)
	{
		if (elem == nullptr)
		{
			elem.swap(recipe);
		}
	}
	recipe.release();
	//AddElement(recipe);
}

void Grid::Tellinfo(int &mx, int &my,Grid &other)
{
	printf("Showing Info");
	gridVec[activeCell]->highlight = FALSE;
	int i = 0;
	for (auto &item : gridVec[activeCell]->materialList)
	{
		other.gridVec[i].reset(new Material(item->name, item->quantity));
		i++;
	}
}

int Grid::InfoCell(std::unique_ptr<Item> &origin)
{
	auto &orig = *origin;

	printf("Showing Info %s\n",orig.name);
	
	orig.highlight = FALSE;
	

	if (strcmp(orig.name, "Axe") == 0)
	{
		return 1;
	}
	else if (strcmp(orig.name, "Plank") == 0)
	{
		return 2;
	}
	else
		return 0;
	activeCell = NULLINT;
}
  
//int main(void)
//{
//	FsOpenWindow(800, 100, WINWID, WINHEI, 1);
//
//	Grid rectGrid(20, 20, 350, 500, 20);
//	Grid toolbar(100, 500, 700, 600, 10);
//
//	Grid crafting(400, 20, 700, 240, 10);
//	Grid ReqChart(400, 260, 700, 500, 10);
//
//	crafting.AddPermElement();
//	
//	int lb, mb, rb, mx, my, eventType;
//	while (FsInkey() != FSKEY_ESC)
//	{
//		FsPollDevice();
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		eventType = FsGetMouseEvent(lb, mb, rb, mx, my);
//		if (eventType == FSMOUSEEVENT_LBUTTONDOWN)
//		{
//			if (toolbar.activeCell == NULLINT && rectGrid.CheckClick(mx, my) == -2) // close
//			{
//				rectGrid.TryTransfer(mx, my, toolbar);
//			}
//			else if (rectGrid.activeCell == NULLINT && toolbar.CheckClick(mx, my) == -2)
//			{
//				toolbar.TryTransfer(mx, my, rectGrid);
//			}
//			else if (crafting.CheckClick(mx, my) == -1)
//			{
//				crafting.Tellinfo(mx, my, ReqChart);
//				crafting.activeCell = NULLINT;
//			}
//			
//			printf("Trying next");
//		}
//
//		rectGrid.Draw();
//		toolbar.Draw();
//
//		crafting.Draw();
//		ReqChart.Draw();
//		//Table.Draw();
//
//		FsSwapBuffers();
//		FsSleep(50);
//	}
//	FsCloseWindow();
//	getchar();
//	return 0;
//}
//// No newline at end of file