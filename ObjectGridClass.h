#ifndef OBJECT_GRID_CLASSES_INCLUDED
#define OBJECT_GRID_CLASSES_INCLUDED

#ifndef NULLINT
#define NULLINT -1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <memory>
#include "fssimplewindow.h"
#include "yspng.h"
#include "yspngenc.h"
#include "ysglfontdata.h"

//const int WINWID = 800, WINHEI = 600;

class Item
{
public:
	Item();
	~Item();
	int color[3] = { 0,0,0 };
	int quantity = 1;
	char name[256];
	bool stackable = FALSE;
	bool highlight = FALSE;
	bool outline = FALSE;
	void Increase(void);
	void Decrease(void);
	void Increase(int quant);
	void Decrease(int quant);
	void Draw(int x0, int y0, int x1, int y1);
	void virtual Use();
	std::vector<std::unique_ptr<Item>> materialList;
};

class Material : public Item
{
public:
	int weight = 0;
	Material();
	~Material();
	Material(char perm[], int q);
	//void Use(void);
	void CleanUp(void);
};


class Recipe : public Item
{
public:
	Recipe();
};

class Axe : public Recipe
{
public:
	Axe();
	~Axe();
};

class Hammer : public Recipe
{
public:
	Hammer();
	~Hammer();
};

class Grid
{
public:
	std::vector<std::unique_ptr<Item>> gridVec;
	bool open = FALSE;
	bool transfer = FALSE;
	int activeCell = -1;
	int backgroundColor[3] = { 255, 0, 0 };
	int highlightColor[3] = { 100, 100, 0 };
	int tempColor[3] = { 0,0,0 };
	int xLeft, yTop, xRight, yBottom, rows, cols, border;
	double cellWidth, cellHeight, xBorder, yBorder;
	Grid();
	~Grid();
	Grid(int x0, int y0, int x1, int y1, int numObjects);
	void CleanUp(void);
	void Draw(void);
	bool AddElement(std::unique_ptr<Item> &item);
	bool AddElement(int elemIndex, std::unique_ptr<Item> &item);
	//void MoveCell(int origin, int destination);
	void MoveCell(std::unique_ptr<Item> &origin, std::unique_ptr<Item> &destination);
	//void IncrementElement(int elemIndex, int quant);
	int CheckClick(int &mx, int &my);
	bool InsideBounds(int &mx, int &my);
	void TryTransfer(int &mx, int &my, Grid &other);
	void AddPermElement(void);
	void Tellinfo(int &mx, int &my, Grid &other);
	int InfoCell(std::unique_ptr<Item> &origin);
};
#endif // !OBJECT_GRID_CLASSES_INCLUDED
