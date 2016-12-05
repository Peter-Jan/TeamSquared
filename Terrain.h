#ifndef TERRAIN_CLASS_INCLUDED
#define TERRAIN_CLASS_INCLUDED

#include "CameraObject.h"
#include "yspng.h"
#include "materialBlock.h"
#include "ObjectGridClass.h"
#include <time.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

class Terrain
{
public:
	int roomSize, blockNum, totalSpace, drawCount;
	int indexCheck[6];
	std::vector<std::vector<int>>* matList;
	std::vector<std::vector<std::vector<int>>>* structList;

	GLuint texId;
	YsRawPngDecoder png;

	std::map<int, std::unique_ptr<Block>> blockMap;
	std::map<int, int> renderMap;

	Terrain();
	Terrain(int size, std::vector<std::vector<int>>& materials, std::vector<std::vector<std::vector<int>>> &structureList);
	Terrain(int size, int randomVsOrdered, std::vector<std::vector<int>>& materials, std::vector<std::vector<std::vector<int>>> &structureList);
	~Terrain();
	void Initialize(int randomVsOrdered);

	void inline GenerateRandom(void);
	void inline GenerateOrdered(void);
	void GenerateFunctionTerrain(void);

	void HideSides(void);
	void HideSingleBlockSides(int i);
	void ShowSingleBlockSides(int i);
	bool FindBlock(CameraObject &camera, int range, int &x, int &y, int &z, int ADDORREMOVE);
	void AddBlock(int x, int y, int z,std::vector<int> matVec);
	void RemoveBlock(std::map<int,std::unique_ptr<Item>> &itemLibrary, Grid &playerInventory, int x, int y, int z);
	void DrawTerrain(CameraObject &cameraView, bool reductionMode, int &key, bool texturesOn);
	void DrawOffsetMode(int &drawCount, CameraObject &camera, bool texturesOn);
	void PlaceStructure(std::vector<std::vector<int>> &structure, std::vector<std::vector<int>> &materials, int xLoc, int yLoc, int zLoc);
	void CreateResourceDeposit(std::vector<int> &resource, int size, int xLoc, int yLoc, int zLoc);
};



#endif // !TERRAIN_CLASS_INCLUDED
