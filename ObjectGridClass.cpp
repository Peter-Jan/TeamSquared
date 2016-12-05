#include "ObjectGridClass.h"

#define NULLINT -1

void glColor3ubArray(int colorArray[3])
{
	glColor3ub(colorArray[0], colorArray[1], colorArray[2]);
}

Item::Item()
{
}

Item::Item(int ClassCode, int classIDIN, char *itemName, int quantIn, double texLocation, int weightIn, double rangeIn,
	int damageIn, int healthIn, int str, double speedIn, bool hitscanIn, bool stackIn, bool high,
	bool outlineIn)
{
	classType = ClassCode;
	classID = classIDIN;
	sprintf(name, itemName);
	quantity = quantIn;
	texLoc = texLocation;
	weight = weightIn;
	range = rangeIn;
	damage = damageIn;
	health = healthIn;
	strength = str;
	speed = speedIn;
	hitscan = hitscanIn;
	stackable = stackIn;
	highlight = high;
	outline = outlineIn;
}

Item::Item(int ClassCode, int classIDIN, char *itemName, int quantIn, double texLocation, int weightIn, double rangeIn,
	int damageIn, int healthIn, int str, double speedIn, bool hitscanIn, bool stackIn, bool high,
	bool outlineIn, int numberOfIngredients, int itemCodes[], int itemQuants[], int craftedCode, int craftedQuant)
{
	classType = ClassCode;
	classID = classIDIN;
	sprintf(name, itemName);
	printf("creating %s\n", itemName);
	quantity = quantIn;
	texLoc = texLocation;
	weight = weightIn;
	range = rangeIn;
	damage = damageIn;
	health = healthIn;
	strength = str;
	speed = speedIn;
	hitscan = hitscanIn;
	stackable = stackIn;
	highlight = high;
	outline = outlineIn;
	numIngredients = numberOfIngredients;
	printf("creating %d\n", numberOfIngredients);
	if (numberOfIngredients != 0)
	{
		for (int i = 0; i < numberOfIngredients; i++)
		{
			ingredients[itemCodes[i]] = itemQuants[i];
			printf("code %d | quant %d\n", itemCodes[i], itemQuants[i]);
		}
		craftItem = craftedCode;
		craftQuantity = craftedQuant;
	}
	else
	{
		// stuff stays nullptr'd
	}
}

void Item::copyFrom(const Item &fromItem)
{
	classType = fromItem.classType;
	classID = fromItem.classID;
	sprintf(name, fromItem.name);
	quantity = fromItem.quantity;
	texLoc = fromItem.texLoc;
	weight = fromItem.weight;
	range = fromItem.range;
	damage = fromItem.damage;
	health = fromItem.health;
	strength = fromItem.strength;
	speed = fromItem.speed;
	hitscan = fromItem.hitscan;
	stackable = fromItem.stackable;
	highlight = fromItem.highlight;
	outline = fromItem.outline;
	numIngredients = fromItem.numIngredients;
	for (auto &iPair : fromItem.ingredients)
	{
		ingredients[iPair.first] = iPair.second;
	}
	craftItem = fromItem.craftItem;
	craftQuantity = fromItem.craftQuantity;
}

Item &Item::operator=(const std::unique_ptr<Item> &fromPtr)
{
	copyFrom(*fromPtr);
	return *this;
}

Item::Item(const std::unique_ptr<Item> &fromPtr)
{
	copyFrom(*fromPtr);
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

void Item::Draw(GLuint texId, int x0, int y0, int x1, int y1)
{
	char quant[10];
	glColor3ub(255, 255, 255);
	glRasterPos2d((double)x0 + 10, (double)y0 + 14);
	YsGlDrawFontBitmap6x7(name);
	glColor3ub(255, 255, 255);
	glRasterPos2d((double)x1 - 10, (double)y1 - 10);
    sprintf(quant,"%d",quantity);
    YsGlDrawFontBitmap10x14(quant);

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

	glColor3ub(255, 255, 255); // Current color is solid white
	
	double imageX = (int)texLoc % 6;
	double imageY = (double)((int)texLoc / 6);

	//printf("ImgX = %lf, ImgY = %d\n, %lf %lf \n", imageX, imageY, imageX / 6, imageY / 2);
	//getchar();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);

	glTexCoord2d(imageX / 6, (imageY) / 4);
	glVertex2d(x0, y0);
	glTexCoord2d((imageX + 1) / 6, (imageY) / 4);
	glVertex2d(x1, y0);
	glTexCoord2d((imageX + 1) / 6, (imageY + 1) / 4);
	glVertex2d(x1, y1);
	glTexCoord2d(imageX / 6, (imageY + 1) / 4);
	glVertex2d(x0, y1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

Material::Material()
{
}

Material::~Material()
{
	CleanUp();
}

Material::Material(char perm[],int q)
{
	sprintf(name, perm);
	color[rand() % 3] = (strlen(name) * 10) % 255;
	quantity = q;
	stackable = TRUE;
}

void Material::CleanUp(void)
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

bool Grid::AddElement(std::map<int, std::unique_ptr<Item>> &itemLibrary, int itemCode) // add item from environment
{
	printf("Trying to Add Element\n");
	int addQuant = 1;
	if (itemLibrary[itemCode]->quantity != 0)
	{
		addQuant += rand() % itemLibrary[itemCode]->quantity;
	}
	
	for (auto &elem : gridVec)
	{
		if (elem == nullptr)
		{
			printf("Item Code = %d\n", itemCode);
			printf("Item ClassType = %d\n", itemLibrary[itemCode]->classType);
			switch (itemLibrary[itemCode]->classType)
			{
			case 0:
				printf("Made it\n");
				elem.reset(new Material);
				break;
			case 1:
				elem.reset(new Weapon);
				break;
			case 2:
				elem.reset(new Useable);
				break;
			case 3:
				elem.reset(new Recipe);
				break;
			case 4:
				elem.reset(new Armor);
				break;
			}
			printf("Adding to empty cell");
			elem->copyFrom(itemLibrary[itemCode]);
			elem->quantity = addQuant;
			return TRUE;
		}
		else if (elem->classID == itemCode && elem->stackable)
		{
			elem->quantity += addQuant;
			return TRUE;
		}
	}
	printf("Inventory full\n");
	return FALSE;
}

bool Grid::AddElement(std::map<int, std::unique_ptr<Item>> &itemLibrary, std::unique_ptr<Item> &item)
{
	for (auto &elem : gridVec)
	{

		if (elem == nullptr) // empty spot
		{
			switch (itemLibrary[item->craftItem]->classType)
			{
			case 0:
				elem.reset(new Material);
				break;
			case 1:
				elem.reset(new Weapon);
				break;
			case 2:
				elem.reset(new Useable);
				break;
			case 3:
				elem.reset(new Recipe);
				break;
			case 4:
				elem.reset(new Armor);
				break;
			}
			elem->copyFrom(itemLibrary[item->craftItem]);
			elem->quantity = item->craftQuantity;
			return TRUE;
		}
		else if (strcmp(itemLibrary[item->craftItem]->name, elem->name) && itemLibrary[item->craftItem]->stackable == true)
		{
			elem->quantity += item->craftQuantity;
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

void Grid::UpdateWepBarSize(int screenWidth, int screenHeight)
{
	xLeft = (int)((25));
	xRight = (int)((gridWidth + 25));
	yTop = (int)((screenHeight - 300));
	yBottom = (int)((screenHeight - 300 + gridHeight));
}

void Grid::UpdateToolbarSize(int screenWidth, int screenHeight)
{
	xLeft = (int)((screenWidth - gridWidth) / 2.0);
	xRight = (int)((screenWidth + gridWidth) / 2.0);
	yTop = (int)((screenHeight - 100));
	yBottom = (int)((screenHeight - 100 + gridHeight));
}

Grid::Grid(int x0, int y0, int x1, int y1, int numObjects)
{
	gridWidth = abs(x1 - x0);
	gridHeight = abs(y1 - y0);
	//printf("%d, %d\n", gridWidth, gridHeight);
	double crRatio = gridWidth / gridHeight;
	border = 5;
	xLeft = x0; yTop = y0; xRight = x1; yBottom = y1;
	//printf("ColRow Ratio = %lf\n", crRatio);
	rows = round(sqrt((double)numObjects / crRatio));
	cols = floor((double)numObjects / (double)rows);
	int cellX, cellY;
	//printf("%d, %d\n", rows, cols);
	for (;;)
	{
		cellX = (double)(gridWidth - (cols + 1)*border) / (double)cols;
		cellY = (double)(gridHeight - (rows + 1)*border) / (double)rows;

		cellWidth = fmin(cellX, cellY);
		cellHeight = cellWidth;

		xBorder = (gridWidth - cellWidth*cols - border*(cols - 1)) / 2;
		yBorder = (gridHeight - cellHeight*rows - border*(rows - 1)) / 2;
		//printf("xBorder = %lf, yBorder = %lf\n", xBorder, yBorder);
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

void Grid::Draw(GLuint texId)
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
				elem.Draw(texId, x0, y0, x1, y1);
			}
			if (index == activeTool)
			{
				glLineWidth(5);
				glBegin(GL_LINE_LOOP);
				glColor3ub(0, 255, 0);
				glVertex2i(x0, y0);
				glVertex2i(x1, y0);
				glVertex2i(x1, y1);
				glVertex2i(x0, y1);
				glEnd();
				glLineWidth(1);
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
    return -2;
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
	if (destIndex >= 0 && destIndex < other.rows*other.cols)
	{
		MoveCell(gridVec[activeCell], other.gridVec[destIndex]);
		activeCell = NULLINT;
		transfer = FALSE;
	}
	other.activeCell = NULLINT;
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
	other.activeCell = NULLINT;
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
				return -2;    // ------> Will replace above 3 lines in actual game
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
    return -5; // arbitrary return value
}

void Grid::AddPermElement(std::map<int,std::unique_ptr<Item>> &itemLib) // always meant to be recipe
{
	printf("Made it\n");
	for (auto &item : itemLib)
	{
		if (item.first > 300 && item.first <= 400)
		{
			for (auto &elem : gridVec)
			{
				if (elem == nullptr)
				{
					elem.reset(new Recipe);
					elem->copyFrom(item.second);
					break;
				}
			}
		}
	}
}

void Grid::Tellinfo(std::map<int, std::unique_ptr<Item>> &itemLib, int &mx, int &my,Grid &other)
{
	printf("Telling Info\n");
	gridVec[activeCell]->highlight = FALSE;
	//printf("%d Active Cell\n", activeCell);
	int i = 0;
	//printf("Number of Items = %d\n", gridVec[activeCell]->numIngredients);
	//printf("12345\n");
	for (auto &itemCodeQuant : gridVec[activeCell]->ingredients)
	{
		switch (itemLib[itemCodeQuant.first]->classType)
		{
		case 0:
			other.gridVec[i].reset(new Material);
			break;
		case 1:
			other.gridVec[i].reset(new Weapon);
			break;
		case 2:
			other.gridVec[i].reset(new Useable);
			break;
		case 3:
			other.gridVec[i].reset(new Recipe);
			break;
		case 4:
			other.gridVec[i].reset(new Armor);
			break;
		}
		//printf("Item Code %d || Item Quant %d\n", itemCodeQuant.first, itemCodeQuant.second);
		other.gridVec[i]->copyFrom(itemLib[itemCodeQuant.first]);
		other.gridVec[i]->quantity = itemCodeQuant.second;
		i++;
	}
	//printf("Done with Items\n");
}

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

	glColor3ub(200, 200, 200);
	glBegin(GL_QUADS);
	glVertex2i(x - 40, y - 20);
	glVertex2i(x + 40, y - 20);
	glVertex2i(x + 40, y + 20);
	glVertex2i(x - 40, y + 20);
	glEnd();
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
    return false;
}

int Button::ClickCheck(int &mx, int &my)
{

	if ((mx>(x - 40)) && (mx<(x + 40)) && (my>(y - 20)) && (my<(y + 20)))
		return 1;
	else
		return 0;
}
