#include "ObjectGridClass.h"

//const int WINWID = 800, WINHEI = 600;
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
	for (auto &c : name)
	{
		if (c == '\n')
		{
			c = 0;
		}
	}
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
	//strcpy(name, perm);
	sprintf(name, perm);
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = q;
	stackable = TRUE;
}

void Material::CleanUp(void)
{
}

Axe::Axe()
{
	damage = 3;
	strength = 1;
	speed = 0.5; //attack per sec
	hitscan = true;
	range = 3 * blockSize;
	sprintf(name, "Axe");
	quantity = 1;
}

AxeRecipe::AxeRecipe()
{
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = 1;
	sprintf(name, "Axe");
	materialList.resize(2);
	materialList[0].reset(new Material("Wood", 2));
	materialList[1].reset(new Material("Stone", 3));
	craftedItem.reset(new Axe);
}

Hammer::Hammer()
{
	damage = 2;
	strength = 2;
	speed = 1.0;
	hitscan = true;
	range = 8 * blockSize;
	sprintf(name, "Hammer");
	quantity = 1;
}

HammerRecipe::HammerRecipe()
{
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = 1;
	sprintf(name, "Hammer");
	materialList.resize(2);
	materialList[0].reset(new Material("Wood", 4));
	materialList[1].reset(new Material("Stone", 10));
	craftedItem.reset(new Axe);
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

int Grid::MoveCell(std::unique_ptr<Item> &origin, std::unique_ptr<Item> &destination, int number)
{
	auto &orig = *origin, &dest = *destination;
	orig.highlight = FALSE;
	if (destination == nullptr)// if dest. is empty 
	{
		printf("nullprt?\n");
		return 0;
	}
	printf("\n %s | %s | %d\n", orig.name, dest.name, strcmp(orig.name, dest.name));
	if (orig.quantity <= dest.quantity && (strcmp(orig.name, dest.name) == 0))    //if inv has less quantity than req
	{
		dest.Decrease(orig.quantity);
		origin.release();		//    remove values of curent cell
		printf("In here\n");
	}
	else if (orig.quantity > dest.quantity && (strcmp(orig.name, dest.name) == 0))  //if inv has more quantity than req
	{
		int temp = dest.quantity;
		dest.Decrease(dest.quantity);
		orig.Decrease(temp);
		//return 0;
	}
	activeCell = NULLINT;    //    cell in grid that calls movecell is empty
}

bool Grid::InsideBounds(int &mx, int &my)
{
	return xLeft + xBorder < mx && xRight - xBorder > mx && yTop + yBorder < my && yBottom - yBorder > my;
}

//void Grid::TryTransfer(int &mx, int &my, Grid &other)
//{
//	printf("Try transferring");
//	other.transfer = TRUE;
//	gridVec[activeCell]->highlight = FALSE;
//	int destIndex = other.CheckClick(mx, my);
//	printf("dest index = %d\n", destIndex);
//	if (destIndex >= 0 && destIndex < other.rows*other.cols)
//	{
//		printf("Before Transfer");
//		MoveCell(gridVec[activeCell], other.gridVec[destIndex]);
//		printf("After Transfer");
//	}
//	printf("Past");
//	activeCell = NULLINT;
//	other.activeCell = NULLINT;
//	transfer = FALSE;
//	other.transfer = FALSE;
//}

void Grid::TryTransfer(int &mx, int &my, Grid &other)
{
	printf("Try transferring");
	other.transfer = TRUE;
	gridVec[activeCell]->highlight = FALSE;
	int destIndex = other.CheckClick(mx, my);
	if (destIndex >= 0 && destIndex < other.rows*other.cols)
	{
		MoveCell(gridVec[activeCell], other.gridVec[destIndex]);
		activeCell = NULLINT;
		transfer = FALSE;
	}
	//activeCell = NULLINT;
	other.activeCell = NULLINT;
	//transfer = FALSE;
	other.transfer = FALSE;
}

void Grid::TryTransfer(int &mx, int &my, Grid &other, int number)
{
	printf("Try transferring");
	other.transfer = TRUE;
	gridVec[activeCell]->highlight = FALSE;
	int destIndex = other.CheckClick(mx, my);
	printf("dest index = %d\n", destIndex);
	printf("ACTIVE CELL = %d, name = %s \n", activeCell, gridVec[activeCell]->name);
	if (destIndex >= 0 && destIndex < other.rows*other.cols)
	{
		printf("Before Transfer");
		MoveCell(gridVec[activeCell], other.gridVec[destIndex], 1);
		printf("After Transfer");
		activeCell = NULLINT;
		transfer = FALSE;
	}
	printf("Past");
	//activeCell = NULLINT;
	other.activeCell = NULLINT;
	//transfer = FALSE;
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
				//return -2;     ------> Will replace above 3 lines in actual game
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
		return NULLINT;
	}
	else if (activeCell != NULLINT)
	{
		printf("Cell");
		transfer = TRUE;
		return -2;
	}
}

void Grid::AddPermElement()
{
	printf("Made it\n");
	std::unique_ptr<Item> recipe(new AxeRecipe);
	for (auto &elem : gridVec)
	{
		if (elem == nullptr)
		{
			elem.swap(recipe);
		}
	}
	recipe.reset(new HammerRecipe);
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
	printf("Telling Info");
	gridVec[activeCell]->highlight = FALSE;
	printf("%d Active Cell\n");
	int i = 0;
	for (auto &item : gridVec[activeCell]->materialList)
	{
		printf("New Item\n");
		other.gridVec[i].reset(new Material(item->name, item->quantity));
		i++;
	}
	printf("Done with Items\n");
}

//int Grid::InfoCell(std::unique_ptr<Item> &origin)
//{
//	auto &orig = *origin;
//
//	printf("Showing Info %s\n",orig.name);
//	
//	orig.highlight = FALSE;
//
//	if (strcmp(orig.name, "Axe") == 0)
//	{
//		printf("In here");
//		return 1;
//	}
//	else if (strcmp(orig.name, "Plank") == 0)
//	{
//		return 2;
//	}
//	else
//		return 0;
//	activeCell = NULLINT;
//}

Button::Button()
{
	x = 550, y = 250;
}

void Button::Draw(int x, int y)
{
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(x - 40, y - 20);
	glVertex2i(x + 40, y - 20);
	glVertex2i(x + 40, y + 20);
	glVertex2i(x - 40, y + 20);
	glEnd();
	glColor3ub(0, 0, 0);
	glRasterPos2d(x - 20, y + 5);
	YsGlDrawFontBitmap10x14("CRAFT");
}

bool Button::CheckCrafting(Grid &ReqChart)
{
	int count = 0;
	for (auto &elem : ReqChart.gridVec)
	{
		if (elem == nullptr && count == 0)
		{
			return FALSE;
		}
		else if (elem == nullptr)
		{
			return TRUE;
		}
		else if (elem->quantity != 0)
		{
			return FALSE;
		}
		count++;
	}
}

int Button::ClickCheck(int &mx, int &my)
{

	if ((mx>(x - 40)) && (mx<(x + 40)) && (my>(y - 20)) && (my<(y + 20)))
		return 1;
	else
		return 0;
}