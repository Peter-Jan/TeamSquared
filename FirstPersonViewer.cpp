#include <stdio.h>
#include <math.h>
#include <random>
#include <vector>
#include <memory>
#include "fssimplewindow.h"
#include "Block.h"
#include "Camera.h"

#define NULLINT -1
double coneDist, coneRad, orthDist;
const int WINWID = 800, WINHEI = 600;
const double GRAV = -.09;

std::vector<double> dist = { 0,0,0 }, orthVec = { 0,0,0 }, fVec = { 0,0,0 };


void DrawGroundLattice(void)
{
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);

	for (int i = -512; i <= 512; i += 8)
	{
		glVertex3i(i, 0, -512);
		glVertex3i(i, 0, +512);

		glVertex3i(-512, 0, i);
		glVertex3i(+512, 0, i);
	}
	glColor3ub(255, 0, 0);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 100, 0);

	glEnd();
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

void DrawOffsetMode(int &drawCount, std::vector<Block> &blockVec, CameraObject &camera2, double len)
{
	glBegin(GL_QUADS);
	drawCount = 0;
	for (int i = 0; i < blockCount; i++)
	{
		auto b = blockVec[i];
		for (int i = 0; i < 3; i++)
		{
			dist[i] = b.pos[i] - camera2.forwardVector[i] / len;
			//printf("%lf",b.pos[i]);
		}
		//printf("\n");
		coneDist = Dot(dist, camera2.forwardVector);
		//printf("coneDist = %lf\n", coneDist);
		if (coneDist >= camera2.nearZ && coneDist <= camera2.farZ)
		{
			//coneRad = coneDist / abs(camera2.farZ - camera2.nearZ)*abs(camera2.farZ);
			coneRad = coneDist / abs(camera2.farZ - camera2.nearZ) * 400;
			orthVec[0] = dist[0] - coneDist*camera2.forwardVector[0];
			orthVec[1] = dist[1] - coneDist*camera2.forwardVector[1];
			orthVec[2] = dist[2] - coneDist*camera2.forwardVector[2];
			orthDist = VecLen(orthVec);
			//printf("OrthDist = %lf\n coneRad = %lf\n",orthDist, coneRad);
			if (orthDist <= coneRad)
			{
				b.DrawSolid();
				drawCount++;
			}
		}
		//printf("Drawn");
	}
	//printf("Draw Count = %d\n", drawCount);
	glEnd();
}

void MergeVecs(std::vector<Block> &blockVec, std::vector<int> &indexVec, int index1, int end1, int index2, int end2)
{
	int startIndex = index1;
	int size = end2 - startIndex;
	std::vector<int> vec1;
	while (index1 < end1 && index2 < end2) // compare vecs, populate temp vec
	{
		if (blockVec[indexVec[index1]].index < blockVec[indexVec[index2]].index)
		{
			vec1.push_back(indexVec[index1]);
			index1++;
		}
		else
		{
			vec1.push_back(indexVec[index2]);
			index2++;
		}
	} 

	while (index1 < end1) // add rest of 1
	{
		vec1.push_back(indexVec[index1]);
		index1++;
	}

	while (index2 < end2) // add rest of 2
	{
		vec1.push_back(indexVec[index2]);
		index2++;
	}
	//printf("size = %d\n", size);

	for (int i = 0; i < size; i++) // modify orig vec
	{
		indexVec[startIndex + i] = vec1[i];
		//printf("%d | ", indexVec[startIndex + i]);
	}
	//printf("\n");
	//getchar();
	//for (int i = index1; i < end2; i++)
	//{
	//	printf("VecIndex = %d | BlockIndex = %d\n", indexVec[i], blockVec[indexVec[i]].index);
	//}
	//printf("\n");
	//getchar();
}

void SortBlocks(std::vector<Block> &blockVec, std::vector<int> &indexVec, int blockNum)
{
	int vecSize = 1;
	int start1, end1, start2, end2;
	//for (int i = 0; i < blockNum; i++)
	//{
	//	printf("VecIndex = %d | BlockIndex = %d\n", indexVec[i], blockVec[i].index);
	//}
	while (vecSize < blockNum)
	{
		//printf("vecSize = %d\n", vecSize);
		int j = 0;
		for (int i = 0; i < blockNum; i += (2 * vecSize))
		{
			j++;
			start1 = i;
			end1 = start1 + vecSize;
			start2 = end1;
			end2 = min(start2 + vecSize, blockNum);
			//printf("Iteration %d \n", j);
			//printf("start = %d\n", start1);
			//printf("end = %d\n", end2);
			MergeVecs(blockVec, indexVec, start1, end1, start2, end2);
		}

		vecSize *= 2;
	}
	//printf("Sorted");
	//for (int i = 0; i < blockNum; i++)
	//{
	//	printf("VecIndex = %d | BlockIndex = %d\n", indexVec[i], blockVec[indexVec[i]].index);
	//}
	//getchar();
}

int BinBlockSearch(std::vector<Block> &blockVec, std::vector<int> &indexVec, int idx)
{
	//int length = blockCount;
	int L = 0, R = blockCount;
	int i = (R+L)/2;
	for(;;)
	{
		//printf("idx = %d | blockIndex = %d | blockVal = %d | size = %d\n", idx, i, blockVec[indexVec[i]].index, size);
		if (idx == blockVec[indexVec[i]].index)
		{
			//printf("FOUND MATCH");
			//getchar();
			return i;
		}
		else if (L > R)
		{
			return NULLINT;
		}
		else if (blockVec[indexVec[i]].index < idx)
		{
			L = i + 1;
			i = (R+L)/2;
		}
		else
		{
			R = i - 1;
			i = (R+L)/2;
		}
		//getchar();
	}
}

void CheckSide(std::vector<Block> &blockVec, std::vector<int> &indexVec, int rows, int cols, int height, int blockNum)
{
	int indexCheck[6] = { -1, 1, -cols, cols, -rows*cols, rows*cols }; // left, right, back, front, bottom, top
	int idx;
	int length = sizeof(blockVec) / sizeof(blockVec[0]);
	auto &b = blockVec[blockNum];
	for (int e = 0; e < 6; e++)
	{
		idx = b.index + indexCheck[e];
		//printf("\nBlock Index = %d | BlockCheck Index = %d\n", b.index, idx);
		// binSearch for conjecture block, if it exists then hide
		idx = BinBlockSearch(blockVec, indexVec, idx); // new index in sorted vector
		if (idx != NULLINT)
		{
			auto &b1 = blockVec[indexVec[idx]];
			if (abs(b.getX() - b1.getX()) + abs(b.getY() - b1.getY()) + abs(b.getZ() - b1.getZ()) == 1)
			{
				b.sideVisible[e] = 0;
			}
		}
		//getchar();
	}
}

void HideSides(std::vector<Block> &blockVec, std::vector<int> &indexVec, int rows, int cols, int height)
{
	for (int i = 0; i < rows*cols*height; i++)
	{
		CheckSide(blockVec, indexVec, rows, cols, height, i);
	}
}

int main(void)
{
	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent, key = 0;
	int drawCount = 0;
	CameraObject camera, camera2;
	std::vector<Block> blockVec;
	std::vector<int> indexVec;

	for (int i = 0; i < blockCount; i++)
	{
		indexVec.push_back(i);
	}
	for (int i = 0; i < blockCount; i++) // random blocks
	{
		blockVec.push_back(Block());
	}

	//int y = roomSize/2;
	//for (int z = 0; z < roomSize; z++) // organized blocks
	//{
	//	for (int x = 0; x < roomSize; x++)
	//	{
	//		blockVec.push_back(Block(x, y, z));
	//	}
	//}
	SortBlocks(blockVec, indexVec, blockCount);
	HideSides(blockVec, indexVec, roomSize, roomSize, 1);
	FsOpenWindow(800, 100, WINWID, WINHEI, 1);
	camera.Update(key);
	camera.z = 500;
	bool reductionMode = FALSE;
	double len = VecLen(camera.forwardVector);
	while (0 == terminate)
	{
		FsPollDevice();

		int wid, hei;
		FsGetWindowSize(wid, hei);

		key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			terminate = 1;
			break;
		case FSKEY_SHIFT:
			reductionMode = !reductionMode;
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glViewport(0, 0, wid, hei);

		// Set up 3D drawing
		camera.SetUpCameraProjection();
		camera.SetUpCameraTransformation();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);

		// 3D drawing from here
		DrawGroundLattice();
		drawCount = 0;

		if (reductionMode)
		{
			camera2.Update(key);
			DrawOffsetMode(drawCount, blockVec, camera2, len);
		}
		else
		{
			camera.Update(key);
			glBegin(GL_QUADS);
			for (auto &b : blockVec)
			{
				b.DrawSolid();
				drawCount++;
			}
			glEnd();

			glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			for (auto &b : blockVec)
			{
				b.DrawEdges();
			}
			glEnd();
		}

		//printf("Draw Count = %d\n", drawCount);

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		// 2D drawing from here

		FsSwapBuffers();
		FsSleep(5);
	}

	return 0;
}


//void HideSides(Block(&blockGrid)[blockCount], int rows, int cols, int height)
//{
//	for (int i = 0; i < rows*cols*height; i++)
//	{
//		CheckSide(blockGrid, rows, cols, height, i);
//	}
//}

//void Swap(Block &block1, Block &block2)
//{
//	decltype(block1) &temp = block1;
//	block1 = block2;
//	block2 = temp;
//}

//void CheckSide(Block(&blockGrid)[blockCount], int rows, int cols, int height, int blockNum)
//{
//	int indexCheck[6] = { -1, 1, -cols, cols, -rows*cols, rows*cols }; // left, right, back, front, bottom, top
//	int idx;
//	int length = sizeof(blockGrid) / sizeof(blockGrid[0]);
//	auto &b = blockGrid[blockNum];
//	for (int e = 0; e < 6; e++)
//	{
//		idx = blockNum + indexCheck[e];
//		if (0 <= (idx) && idx <= length)
//		{
//			auto &b1 = blockGrid[idx];
//			if (abs(b.getX() - b1.getX()) + abs(b.getY() - b1.getY()) + abs(b.getZ() - b1.getZ()) == 1)
//			{
//				b.sideVisible[e] = 0;
//			}
//		}
//	}
//}
