#ifndef OBJECT_GRID_CLASSES_INCLUDED
#define OBJECT_GRID_CLASSES_INCLUDED

#ifndef NULLINT
#define NULLINT -1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
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
#include "MiscFunctions.h"
//#include "Terrain.h"

class Item
{
public:
	Item();
	Item(int ClassType, int classID, char *itemName, int quantity, int texLocation, int weight, double range,
		int damage, int health, int str, double speed, bool hitscan, bool stackable, bool highlight,
		bool outline);
	Item(int ClassType, int classID, char *itemName, int quantity, int texLocation, int weight, double range,
		int damage, int health, int str, double speed, bool hitscan, bool stackable, bool highlight,
		bool outline, int numberOfIngredients, int itemCodes[], int itemQuants[], int craftedCode, int craftedQuant); // recipes
	void copyFrom(const Item &fromItem);
	Item(const std::unique_ptr<Item> &fromPtr);
	Item &operator=(const std::unique_ptr<Item> &fromPtr);
	~Item();
	int color[3] = { 0,0,0 };
	char name[256];
	int classType;
	int classID;
	int quantity = 1;
	int texLoc;
	int weight = 0;
	double range;
	int damage; // health dmg dealt
	int health; // restorative hp amount
	int strength; // durability vs another object
	double speed; // cooldown timer, in seconds
	bool hitscan; // true = instant, false = projectile
	bool stackable = FALSE;
	bool highlight = FALSE;
	bool outline = FALSE;
	void Increase(void);
	void Decrease(void);
	void Increase(int quant);
	void Decrease(int quant);
	void Draw(int x0, int y0, int x1, int y1);
	int virtual Use(void) { return 0; }
	int numIngredients = 0;
	std::map<int, int> ingredients; // <item code, quantity>
	int craftItem; // <item code>
	int craftQuantity; // <items gained through crafting>
};

class Material : public Item
{
public:
	Material();
	~Material();
	Material(char perm[], int q);
	//void Use(void);
	void CleanUp(void);
	int Use(void);
};

class Useable : public Item
{
public:
	Useable() {}
	~Useable() {}
};

class Weapon : public Item
{
public:
	Weapon() {}
	~Weapon() {}
};

class Recipe : public Item
{
public:
	Recipe() {}
	~Recipe() {}
};

class Grid
{
public:
	std::vector<std::unique_ptr<Item>> gridVec;
	bool open = FALSE;
	bool transfer = FALSE;
	int activeCell = -1;
	int activeTool = -1;
	int backgroundColor[3] = { 255, 0, 0 };
	int highlightColor[3] = { 100, 100, 0 };
	int activeItemHighlight[3] = { 255,0,0 };
	int tempColor[3] = { 0,0,0 };
	int xLeft, yTop, xRight, yBottom, rows, cols, border;
	double cellWidth, cellHeight, xBorder, yBorder;
	Grid();
	~Grid();
	Grid(int x0, int y0, int x1, int y1, int numObjects);
	void CleanUp(void);
	void Draw(void);
	bool AddElement(std::map<int, std::unique_ptr<Item>> &itemLibrary, int ClassCode);
	bool AddElement(std::map<int, std::unique_ptr<Item>> &itemLibrary, std::unique_ptr<Item> &item);
	bool AddElement(int elemIndex, std::unique_ptr<Item> &item);
	void MoveCell(std::unique_ptr<Item> &origin, std::unique_ptr<Item> &destination);
	int MoveCell(std::unique_ptr<Item> &origin, std::unique_ptr<Item> &destination, int number);
	int CheckClick(int &mx, int &my);
	bool InsideBounds(int &mx, int &my);
	void TryTransfer(int &mx, int &my, Grid &other);
	void TryTransfer(int &mx, int &my, Grid &other, int number);
	void AddPermElement(std::map<int, std::unique_ptr<Item>> &itemLib);
	void Tellinfo(std::map<int, std::unique_ptr<Item>> &itemLib, int &mx, int &my, Grid &other);
};

class Button
{
public:
	int x, y;
	Button();
	~Button() {}
	void Draw(int x, int y);
	bool CheckCrafting(Grid &ReqChart);
	int ClickCheck(int &mx, int &my);
};

#endif // !OBJECT_GRID_CLASSES_INCLUDED