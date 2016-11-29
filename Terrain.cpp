#include "Terrain.h"




int BlockDist(const Block &block1, const Block &block2)
{
	return abs(block1.getX() - block2.getX()) + abs(block1.getY() - block2.getY()) + abs(block1.getZ() - block2.getZ());
}

Terrain::Terrain()
{
	roomSize = 10; // default
	Initialize(0);
}

Terrain::Terrain(int size)
{
	roomSize = size;
	Initialize(0);
}

Terrain::Terrain(int size, int randomVsOrdered) // 0 = random, 1 = linear, 2 = Terrain generator
{
	roomSize = size;
	Initialize(randomVsOrdered);
}

void Terrain::Initialize(int randomVsOrdered) // 0 = random, 1 = linear
{
	indexCheck[0] = -1; indexCheck[1] = 1; indexCheck[2] = -roomSize; indexCheck[3] = roomSize;
	indexCheck[4] = -pow(roomSize, 2); indexCheck[5] = pow(roomSize, 2); // left, right, back, front, bottom, top
	if (randomVsOrdered == 1)
	{
		blockNum = pow(roomSize, 2);
		GenerateRandom();
	}
	else if (randomVsOrdered == 0)
	{
		blockNum = pow(roomSize, 2);
		GenerateOrdered();
	}
	else
	{
		GenerateFunctionTerrain();
	}
	HideSides();

}

void Terrain::GenerateFunctionTerrain(void)
{
	int y = 0;

	blockNum = 0;

	int stepSize = roomSize / 10;
	//srand((int)time(nullptr));

	int xScalar = rand() % (roomSize / 10 + 1) + 1;
	int zScalar = rand() % (roomSize / 10 + 1) + 1;
	double xFreq = rand() % 10 + 1;
	double zFreq = rand() % 10 + 1;

	int index;
	for (int x = 0; x < roomSize; x++)
	{
		for (int z = 0; z < roomSize; z++)
		{
			y = (int)(sin(x*xFreq*DEGTORAD + z*zFreq*DEGTORAD)*cos(x*DEGTORAD)*xScalar / 2.0 + sin(x*zFreq*DEGTORAD*z*xFreq*DEGTORAD / 2.0)*cos(z*DEGTORAD)*zScalar / 2.0) + roomSize / 10;

			while (y >= 0)
			{
				index = x + roomSize*z + pow(roomSize, 2)*y;
				std::unique_ptr<Block> newPtr;
				newPtr.reset(new Block(roomSize, x, y, z));
				blockMap[index] = std::move(newPtr);
				y--;
				blockNum++;
			}
		}
	}

	printf("World Initialized\n");
}

void Terrain::GenerateMaterial(materialBlock matBlock)
{
	int matR, matG, matB;
	//AddBlock(matBlock.getX(),matBlock.getY(),matBlock.getY(),matBlock.textMapX,matBlock.textMapY,matBlock.getR(),matBlock.getG(),matBlock.getB());
	//printf("X: %d,Y: %d ", matBlock.textMapX, matBlock.textMapY);
}

Terrain::~Terrain()
{
}

void inline Terrain::GenerateRandom(void)
{
	Block tempBlock;
	for (int i = 0; i < blockNum; i++) // random blocks
	{
		//tempBlock = Block(roomSize);
		std::unique_ptr<Block> newPtr;
		newPtr.reset(new Block(roomSize));
		blockMap[newPtr->index] = std::move(newPtr);
	}
}

void inline Terrain::GenerateOrdered(void) // ordered blocks (starts at "y=0" plane, actually y = roomHeight/2)
{
	//int y = roomSize/2;
	Block tempBlock;
	blockNum = 0;
	for (int y = 0; y < 3; y++)
	{
		for (int z = 0; z < roomSize; z++)
		{
			for (int x = 0; x < roomSize; x++)
			{
				tempBlock = Block(roomSize, x, y, z);
				std::unique_ptr<Block> newPtr;
				newPtr.reset(new Block(roomSize, x, y, z));
				blockMap[tempBlock.index] = std::move(newPtr);
				printf("Index = %d, BlockLocationIndex = %d\n", x + roomSize*z + roomSize*roomSize*y, blockMap[x + roomSize*z + roomSize*roomSize*y]->index);
				blockNum++;
			}
		}
	}
}

void Terrain::HideSides(void)
{
	int visCount = 0;
	int location;
	for (auto &keyValPair : blockMap)
	{
		location = keyValPair.first;
		HideSingleBlockSides(location);
	}

	for (auto &keyValPair : blockMap)
	{
		location = keyValPair.first;
		if (renderMap.find(keyValPair.first) == renderMap.end() && keyValPair.second->renderable == TRUE) // visible object not in the map
		{
			renderMap[location] = location; // add renderable object to the renderMap
		}
		else if (renderMap.find(keyValPair.first) != renderMap.end() && keyValPair.second->renderable == FALSE)
		{
			renderMap.erase(location);
		}
	}
	printf("Total Blocks = %d, Total Visible Blocks = %d\n", blockNum, (int)renderMap.size());
}

void Terrain::HideSingleBlockSides(int i) // for block i in the block-Vector, check the 6 surrounding sides and hide any shared faces
{
	//printf("Hide Blocks around index = %d", i);
	int idx;
	auto &b = blockMap[i];
	b->renderable = FALSE;
	int mostRecentIndex;
	int eReverse;
	for (int e = 0; e < 6; e++)
	{
		eReverse = e + (e % 2 == 0 ? 1 : -1);
		//printf("%d vs %d\n", e, eReverse);
		idx = b->index + indexCheck[e];
		if (blockMap.find(idx) != blockMap.end())
		{
			auto &b1 = blockMap[idx];
			if (BlockDist(b, b1) == 1)
			{
				b->sideVisible[e] = 0;
				b1->sideVisible[eReverse] = 0;
			}
			else if ((b->getX() % roomSize == 0 || (b->getY() / roomSize) % roomSize == 0) || b1->getX() % roomSize == 0 || (b1->getZ() / roomSize) % roomSize)
			{
				// don't render
			}
			else
			{
				if (renderMap.find(idx) != renderMap.end())
				{
					printf("Add block %d\n", idx);
					renderMap[idx] = idx;
				}
				b->renderable = TRUE;
			}
		}
		else if (b->getY() != 0) // don't draw the floor, saves a bunch of blocks being rendered if the roomSize gets large
		{
			b->renderable = TRUE;
		}
	}
}

void Terrain::ShowSingleBlockSides(int i) // for block i in the block-Vector, check the 6 surrounding sides and show any unshared faces
{
	printf("Show Blocks around index = %d\n", i);
	int idx;
	auto &&b = blockMap[i];
	b->renderable = FALSE;
	int eReverse;
	for (int e = 0; e < 6; e++)
	{
		eReverse = e + (e % 2 == 0 ? 1 : -1);
		printf("eRev = %d\n", eReverse);
		idx = b->index + indexCheck[e];

		if (blockMap.find(idx) != blockMap.end())
		{
			auto &&b1 = blockMap[idx];
			if (BlockDist(b, b1) == 1)
			{
				b->sideVisible[e] = 1;
				b1->sideVisible[eReverse] = 1;
				b1->renderable = TRUE;
				b->renderable = TRUE;
				if (renderMap.find(idx) == renderMap.end()) // block not current present
				{
					printf("Add block %d\n", idx);
					renderMap[idx] = idx; // add block
				}
			}
		}
		else
		{
			b->sideVisible[e] = 1;
			b->renderable = TRUE;
		}
	}
}

void Terrain::AddBlock(int x, int y, int z, std::vector<int> matVec)
{
	int newLocation = x + z*roomSize + y*roomSize*roomSize;

	if (blockMap.find(newLocation) == blockMap.end()) // free spot
	{
		std::unique_ptr<materialBlock> newPtr;
		//printf("BEFORE %d %d %d\n", x, y, z);

		//printf("BEFORE x = %d, y = %d, z = %d\n", newPtr->getX(), newPtr->getY(), newPtr->getZ());
		newPtr.reset(new materialBlock(roomSize, x, y, z, matVec));

		//printf("AFTER x = %d, y = %d, z = %d\n", newPtr->getX(), newPtr->getY(), newPtr->getZ());
		blockMap[newLocation] = std::move(newPtr);
		//printf("AFTER2 x = %d, y = %d, z = %d\n", blockMap[newLocation]->getX(), blockMap[newLocation]->getY(), blockMap[newLocation]->getZ());
		//printf("Index = %d, BlockLocationIndex = %d\n", newLocation, blockMap[newLocation]->index);
		blockNum++;

		HideSingleBlockSides(newLocation);

		if (blockMap[newLocation]->renderable == TRUE)
		{
			renderMap[newLocation] = newLocation;
		}
	}
}

void Terrain::RemoveBlock(int x, int y, int z)
{
	int newLocation = x + z*roomSize + y*roomSize*roomSize;
	//printf("newLocation = %d\n", newLocation);
	//printf("BEFORE renderMap size = %d, blockMap size = %d\n", renderMap.size(), blockMap.size());
	if (blockMap.find(newLocation) != blockMap.end()) // if the removed block is in the list of existing blocks
	{
		//printf("In here\n");
		ShowSingleBlockSides(newLocation);
		blockNum--;
		if (renderMap.find(newLocation) != renderMap.end())
		{
			renderMap.erase(newLocation);
		}
		blockMap.erase(newLocation);
		//printf("renderMap size = %d, blockMap size = %d\n", renderMap.size(), blockMap.size());
	}
	else
	{
		//blockMap.erase(blockMap[newLocation].index);
		//printf("Out of Bounds \n");
	}
	//printf("AFTER renderMap size = %d, blockMap size = %d\n", renderMap.size(), blockMap.size());
}

double VecLen(std::vector<double> const &vec)
{
	double sum = 0;
	for (double e : vec)
	{
		sum += e*e;
	}
	return sqrt(sum);
}

double Dot(std::vector<double> const &vec1, std::vector<double> const &vec2)
{
	double len = VecLen(vec2);
	return (vec1[0] * (vec2[0] / len) + vec1[1] * (vec2[1] / len) + vec1[2] * (vec2[2] / len));
}

void Terrain::DrawOffsetMode(int &drawCount, CameraObject &camera, bool texturesOn)
{
	std::vector<double> dist = { 0,0,0 };
	double coneDist, lenObj;

	if (FsGetKeyState(FSKEY_P) != 1)
	{
		if (texturesOn) // method only works if you have large group of objects with the same texture
		{
			glBindTexture(GL_TEXTURE_2D, texId);	// Select the current texture.
		}
		glBegin(GL_QUADS);
		for (auto &keyVal : renderMap)
		{
			auto &b = blockMap[keyVal.second];
			for (int i = 0; i < 3; i++)
			{
				dist[i] = b->centerPos[i] - camera.playerBlock.centerPos[i]; // get cam->object vector
			}

			lenObj = VecLen(dist); // get cam->object scalar distance
			if (lenObj <= 30)
			{
				if (texturesOn)
				{
					b->DrawTexture(texId,b->textMapX,b->textMapY);
				}
				else
				{
					b->DrawSolid();
				}
			}
			else if (lenObj > camera.farZ)
			{
				// don't render
			}
			else
			{
				dist[0] /= lenObj; dist[1] /= lenObj; dist[2] /= lenObj; // unit-normalize
				coneDist = Dot(dist, camera.forwardVector);
				if (coneDist >= camera.coneAngle)
				{
					if (texturesOn)
					{
						b->DrawTexture(texId,b->textMapX,b->textMapY);
					}
					else
					{
						b->DrawSolid();
					}
				}
			}
		}
		glEnd();
		//glBegin(GL_LINES);
		//for (auto &keyVal : renderMap)
		//{
		//	auto &b = blockMap[keyVal.second];
		//	for (int i = 0; i < 3; i++)
		//	{
		//		dist[i] = b->centerPos[i] - camera.playerBlock.centerPos[i]; // get cam->object vector
		//	}
		//	
		//	lenObj = VecLen(dist); // get cam->object scalar distance
		//	if (lenObj <= 30)
		//	{
		//		b->DrawEdges();
		//	}
		//	else if (lenObj > camera.farZ)
		//	{
		//		// don't render
		//	}
		//	else
		//	{
		//		dist[0] /= lenObj; dist[1] /= lenObj; dist[2] /= lenObj; // unit-normalize
		//		coneDist = Dot(dist, camera.forwardVector);
		//		if (coneDist >= camera.coneAngle)
		//		{
		//			b->DrawEdges();
		//		}
		//	}
		//}
		//glEnd();
	}
	else
	{
		if (texturesOn) // method only works if you have large group of objects with the same texture
		{
			glBindTexture(GL_TEXTURE_2D, texId);	// Select the current texture.
		}
		glBegin(GL_QUADS);
		for (auto &keyVal : blockMap)
		{
			auto &b = keyVal.second;
			for (int i = 0; i < 3; i++)
			{
				dist[i] = b->centerPos[i] - camera.playerBlock.centerPos[i]; // get cam->object vector
			}

			lenObj = VecLen(dist); // get cam->object scalar distance
			if (lenObj <= 30)
			{
				if (texturesOn)
				{
					b->DrawTexture(texId,b->textMapX,b->textMapY);
				}
				else
				{
					b->DrawSolid();
				}
			}
			else if (lenObj > camera.farZ)
			{
				// don't render
			}
			else
			{
				dist[0] /= lenObj; dist[1] /= lenObj; dist[2] /= lenObj; // unit-normalize
				coneDist = Dot(dist, camera.forwardVector);
				if (coneDist >= camera.coneAngle)
				{
					if (texturesOn)
					{
						b->DrawTexture(texId,b->textMapX,b->textMapY);
					}
					else
					{
						b->DrawSolid();
					}
				}
			}
		}
		glEnd();

		//glBegin(GL_LINES);
		//for (auto &keyVal : blockMap)
		//{
		//	auto &b = keyVal.second;
		//	for (int i = 0; i < 3; i++)
		//	{
		//		dist[i] = b->centerPos[i] - camera.playerBlock.centerPos[i]; // get cam->object vector
		//	}
		//	lenObj = VecLen(dist); // get cam->object scalar distance
		//	if (lenObj <= 30)
		//	{
		//		b->DrawEdges();
		//	}
		//	else if (lenObj > camera.farZ)
		//	{
		//		// don't render
		//	}
		//	else 			{
		//		dist[0] /= lenObj; dist[1] /= lenObj; dist[2] /= lenObj; // unit-normalize
		//		coneDist = Dot(dist, camera.forwardVector);
		//		if (coneDist >= camera.coneAngle)
		//		{
		//			b->DrawEdges();
		//		}
		//	}
		//}
		//glEnd();
	}
}

void Terrain::DrawTerrain(CameraObject &cameraView, bool reductionMode, int &key, bool texturesOn)
{
	// Draw ground lattice
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);

	for (int i = 0; i <= roomSize * 8; i += 8)
	{
		glVertex3i(i, 0, 0);
		glVertex3i(i, 0, roomSize * 8);

		glVertex3i(0, 0, i);
		glVertex3i(roomSize * 8, 0, i);
	}
	glColor3ub(255, 0, 0);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 100, 0);

	glEnd();
	cameraView.Update(key);

	if (reductionMode)
	{
		DrawOffsetMode(drawCount, cameraView, texturesOn);
	}
	else
	{
		if (FsGetKeyState(FSKEY_P) != 1) // only render visible objects
		{
			if (texturesOn) // method only works if you have large group of objects with the same texture
			{
				glBindTexture(GL_TEXTURE_2D, texId);	// Select the current texture.
			}
			glBegin(GL_QUADS);
			for (auto &keyVal : renderMap)
			{
				if (texturesOn)
				{
					blockMap[keyVal.first]->DrawTexture(texId, blockMap[keyVal.first]->textMapX, blockMap[keyVal.first]->textMapY);
				}
				else
				{
					blockMap[keyVal.first]->DrawSolid();
				}
			}
			glEnd();
			/*glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			for (auto &keyVal : renderMap)
			{
				blockMap[keyVal.first]->DrawEdges();
			}
			glEnd();*/
		}
		else
		{
			cameraView.Update(key);
			if (texturesOn) // method only works if you have large group of objects with the same texture
			{
				glBindTexture(GL_TEXTURE_2D, texId);	// Select the current texture.
			}
			glBegin(GL_QUADS);
			for (auto &keyVal : blockMap)
			{
				if (texturesOn)
				{
					keyVal.second->DrawTexture(texId, keyVal.second->textMapX, keyVal.second->textMapY);
				}
				else
				{
					keyVal.second->DrawSolid();
				}
			}
			glEnd();

			/*glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			for (auto &keyVal : blockMap)
			{
				keyVal.second->DrawEdges();
			}
			glEnd();*/
		}
	}
}

bool Terrain::FindBlock(CameraObject &camera, int &x, int &y, int &z, int ADDORREMOVE) // #DEFINE ADD = 1 #DEFINE REMOVE = 0
{
	int i = 0;
	int blockSize = camera.blockSize;
	int index;
	std::vector<double> location = { camera.playerBlock.xM, camera.playerBlock.pos[1] + camera.camHeight, camera.playerBlock.zM };
	printf("Camera Position = %lf, %lf, %lf\n", location[0], location[1], location[2]);
	printf("Forward Vector = %lf, %lf, %lf\n", camera.forwardVector[0], camera.forwardVector[1], camera.forwardVector[2]);
	//SetVec(location, camera.pos);
	index = (int)location[0] / blockSize + (int)location[2] / blockSize * roomSize + (int)location[1] / blockSize * pow(roomSize, 2);

	while (i <= blockSize * 8)
	{
		VecPlus(location, camera.forwardVector); // increment location by camera's forward Vector
		x = (int)location[0] / blockSize;
		y = (int)location[1] / blockSize;
		z = (int)location[2] / blockSize;
		printf("%d %d %d\n", x, y, z);
		index = x + z*roomSize + y*pow(roomSize, 2);
		if (this->blockMap.find(index) != this->blockMap.end()) // found potential block
		{
			if (this->blockMap[index]->getX() == x && this->blockMap[index]->getY() == y && this->blockMap[index]->getZ() == z) // check if it's the correct block
			{
				if (ADDORREMOVE == REMOVE)
				{
					return TRUE;
				}
				if (ADDORREMOVE == ADD) // move back one step, this will be the empty block position
				{
					VecPlus(location, -camera.forwardVector[0], -camera.forwardVector[1], -camera.forwardVector[2]);
					x = (int)location[0] / blockSize;
					y = (int)location[1] / blockSize;
					z = (int)location[2] / blockSize;
					return TRUE;
				}
			}
		}
		i++;
	}
	return FALSE;
}