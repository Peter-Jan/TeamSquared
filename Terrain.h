#ifndef TERRAIN_CLASS_INCLUDED
#define TERRAIN_CLASS_INCLUDED

#include "CameraObject.h"

class Terrain
{
public:
	int roomSize, blockNum, totalSpace, drawCount;
	int indexCheck[6];

	std::map<int, std::unique_ptr<Block>> blockMap;
	std::map<int, int> renderMap;

	Terrain();
	Terrain(int size);
	Terrain(int size, int randomVsOrdered);
	~Terrain();
	void Initialize(int randomVsOrdered);

	void inline GenerateRandom(void);
	void inline GenerateOrdered(void);
	void GenerateFunctionTerrain(void);

	void HideSides(void);
	void HideSingleBlockSides(int i);
	void ShowSingleBlockSides(int i);
	bool FindBlock(CameraObject &camera, int &x, int &y, int &z, int ADDORREMOVE);
	void AddBlock(int x, int y, int z);
	void RemoveBlock(int x, int y, int z);

	void DrawTerrain(CameraObject &cameraView, bool reductionMode, int &key);
	void DrawOffsetMode(int &drawCount, CameraObject &camera);
};

#endif // !TERRAIN_CLASS_INCLUDED
