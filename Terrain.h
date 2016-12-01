#ifndef TERRAIN_CLASS_INCLUDED
#define TERRAIN_CLASS_INCLUDED

#include "CameraObject.h"
#include "yspng.h"
#include "materialBlock.h"

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
	GLuint texId;
	YsRawPngDecoder png;

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

	void GenerateMaterial(materialBlock matBlock);

	void HideSides(void);
	void HideSingleBlockSides(int i);
	void ShowSingleBlockSides(int i);
	bool FindBlock(CameraObject &camera, int &x, int &y, int &z, int ADDORREMOVE);
	void AddBlock(int x, int y, int z,std::vector<int> matVec);
	void RemoveBlock(int x, int y, int z);

	void DrawTerrain(CameraObject &cameraView, bool reductionMode, int &key, bool texturesOn);
	void DrawOffsetMode(int &drawCount, CameraObject &camera, bool texturesOn);
};

#endif // !TERRAIN_CLASS_INCLUDED
