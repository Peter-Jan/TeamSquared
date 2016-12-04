// hi from Loren
#include "Terrain.h"
#include "Decoder.h"
#include "ObjectGridClass.h"
#include "enemy.h"

int main(void)
{
	FsOpenWindow(800, 100, WINWID, WINHEI, 1);

	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent, key = 0;
	int drawCount = 0;

	// texX, texY, bgColor[3], health, quantity

	std::map<int, std::unique_ptr<Item>> itemLibrary;
	//							 <ClassCode, itemCode, name, quant, texture#, weight, range, damage, health, strength, speed, hitscan, stackable, highlight, outline, numIngedients, matCodes[numIngedients], quantities[numIngedients], craftedItemCode, craftedQuant>
	// materials,   ClassCode == 0,   0 - 100
	itemLibrary[0].reset(new Item(0, 0, "Dirt",    3, 0, 1, 8.0, 0,  2, 0, 0.0, true, true, false, false));
	itemLibrary[1].reset(new Item(0, 1, "Stone",   1, 1, 1, 8.0, 0,  4, 1, 0.0, true, true, false, false));
	itemLibrary[2].reset(new Item(0, 2, "Steel",   0, 2, 1, 8.0, 0,  6, 1, 0.0, true, true, false, false));
	itemLibrary[3].reset(new Item(0, 3, "Wood",    2, 3, 1, 8.0, 0,  3, 0, 0.0, true, true, false, false));
	itemLibrary[4].reset(new Item(0, 4, "Ruby",    0, 4, 1, 8.0, 0, 10, 2, 0.0, true, true, false, false));
	itemLibrary[5].reset(new Item(0, 5, "Emerald", 0, 5, 1, 8.0, 0,  6, 2, 0.0, true, true, false, false));

	// weapons,     ClassCode == 1, 101 - 200
	itemLibrary[101].reset(new Item(1, 101, "Stick", 1, 101, 1, 4.0, 1, 0, 0, 0.5, true, false, false, false));
	itemLibrary[102].reset(new Item(1, 102, "RockHammer", 1, 102, 1, 4.0, 1, 0, 0, 0.5, true, false, false, false));

	// consumables, ClassCode == 2, 201 - 300
	itemLibrary[201].reset(new Item(2, 201, "Orange",  6, 201, 1, 0.0, 0, 10, 2, 15, true, true, false, false));

	// recipes,     ClassCode == 3, 301 - 400
	int *ingredientCodes = new int[1]{ 3 };
	int *ingredientQuants = new int[1]{ 1 };
	itemLibrary[301].reset(new Item(3, 301, "Lv1 Stick", 1, 301, 1, 0.0, 1, 0, 0, 0.5, true, false, false, false, 1, ingredientCodes, ingredientQuants, 101, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[2]{ 3,1 };
	ingredientQuants = new int[2]{ 3,2 };
	itemLibrary[302].reset(new Item(3, 302,"Lv1 RockHammer", 1, 302, 1, 0.0, 1, 0, 0, 0.5, true, false, false, false, 2, ingredientCodes, ingredientQuants, 102, 1));
	delete[] ingredientCodes, ingredientQuants;

	//					<itemCode, texX, texY, r,g,b, strength, health, quant
	std::vector<int> dirt =    { 0,   0,0,255,255,255, 0,  2, 3 };
	std::vector<int> stone =   { 1,   5,0,255,255,255, 1,  4, 1 };
	std::vector<int> steel =   { 2,   2,0,255,255,255, 1,  6, 0 };
	std::vector<int> wood =    { 3,   1,1,255,255,255, 0,  3, 2 };
	std::vector<int> ruby =    { 4,   4,0,255,255,255, 2, 10, 0 };
	std::vector<int> emerald = { 5,   1,0,255,255,255, 2,  6, 0 };
	std::vector<int> orange =  { 201, 3,0,255,255,255, 0, 10, 6 };

	std::vector<std::vector<int>> materials;
	materials.push_back(dirt);
	materials.push_back(stone);
	materials.push_back(steel);
	materials.push_back(wood);
	materials.push_back(ruby);
	materials.push_back(emerald);
	materials.push_back(orange);

	// structures <itemCode, xGrid, yGrid, zGrid>
	//std::vector<int[4]> indices;
	//indices.push_back({  0, 0, 0 });
	//indices.push_back({  0, 1, 0});
	//indices.push_back({  0, 2, 0});
	//indices.push_back({ -1, 2, 0});
	//indices.push_back({  1, 2, 0});
	//indices.push_back({  0, 2,-1});
	//indices.push_back({  0, 2, 1});

	Grid inventory(20, 20, 350, 500, 20);
	Grid toolbar(100, 500, 700, 600, 10);
	Grid crafting(400, 20, 700, 240, 10);
	Grid ReqChart(400, 260, 700, 500, 10);
	Button but;

	crafting.AddPermElement(itemLibrary);

	Terrain worldGrid(30, 2, materials);


	CameraObject camera(worldGrid.roomSize), camera2(worldGrid.roomSize);
	worldGrid.texId = decodePng();

	worldGrid.AddBlock(5, 5, 5, orange);
	enemy dasEnemy(worldGrid.roomSize,21*8,21*8,21*8);

	camera.playerBlock.roomSize = worldGrid.roomSize;
	camera2.playerBlock.roomSize = worldGrid.roomSize;

	camera.pos[0] = 200.0;
	camera.pos[1] = 50.0;
	camera.pos[2] =  20.0;
	camera2.pos[0] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera2.pos[1] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera2.pos[2] += worldGrid.roomSize / 2 * camera2.blockSize;
	camera.Update(key, worldGrid.blockMap);
	camera2.Update(key, worldGrid.blockMap);
	camera2.stationary = TRUE;

	bool reductionMode = false;
	bool switchCamera = false;
	bool texturesOn = false;
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // vertices of any object's front face (aka outside face) should always be specified in CLOCKWISE order
	glCullFace(GL_BACK); // back 3 sides of each block are automatically removed by this openGL culling function
	int xGrid, yGrid, zGrid;

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
			break;
		case FSKEY_C:
			switchCamera = !switchCamera;
			break;
		case FSKEY_G:
			camera.gravityOn = !camera.gravityOn;
			break;
		case FSKEY_T:
			texturesOn = !texturesOn;
			if (texturesOn)
			{
				glEnable(GL_TEXTURE_2D); // Turn on the texture mapping
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}
			break;
		case FSKEY_TAB:
			camera.cursorLock = !camera.cursorLock;
			camera2.cursorLock = camera.cursorLock;
			camera.Update(key, worldGrid.blockMap);
			camera2.Update(key, worldGrid.blockMap);
			break;
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
		dasEnemy.drawEnemy();
		terminate = dasEnemy.chase(camera, worldGrid.blockMap);

#if defined(_WIN32_WINNT)
		if (switchCamera)
		{
			if (camera2.cursorLock)
			{
				camera2.Update(key, worldGrid.blockMap);
			}
			worldGrid.DrawTerrain(camera2, reductionMode, key, texturesOn);
		}
		else
		{
			if (camera.cursorLock)
			{
				camera.Update(key, worldGrid.blockMap);
			}
			worldGrid.DrawTerrain(camera, reductionMode, key, texturesOn);
		}
#endif

		switch (FsGetMouseEvent(lb, mb, rb, mx, my))
		{
		case FSMOUSEEVENT_LBUTTONDOWN:
			printf("USER CLICKED\n");

			if (!camera.cursorLock)
			{
				if (but.ClickCheck(mx, my) == 1)
				{
					printf("\nInside the Button!");
					if (but.CheckCrafting(ReqChart))
					{
						printf("\nReady to craft!"); //then call crafting implementation function

						if (inventory.AddElement(itemLibrary, crafting.gridVec[crafting.activeCell]) == true)
						{
							crafting.activeCell = NULLINT;
							for (auto &elem : ReqChart.gridVec)
							{
								if (elem != nullptr)
								{
									printf("resetting elements");
									elem.reset(nullptr);
								}
								else
								{
									break;
								}
							}
						}
					}
					else
					{
						printf("\nNot enough ingredients!");
					}
				}

				if (inventory.activeCell == NULLINT && toolbar.activeCell == NULLINT) // if no cells are highlighted
				{
					if (inventory.CheckClick(mx, my) == -2)
					{
						printf("Inside Inventory\n");
						inventory.transfer = FALSE;
						// do nothing
					}
					else if (toolbar.CheckClick(mx, my) == -2)
					{
						printf("Inside Toolbar\n");
						toolbar.transfer = FALSE;
						// do nothing
					}
					else if (crafting.activeCell != NULLINT) // There's an active recipe
					{
						int tempCell = crafting.activeCell;
						crafting.activeCell = NULLINT;
						if (crafting.CheckClick(mx, my) == NULLINT && crafting.gridVec[crafting.activeCell] != nullptr) // if clicked cell has a recipe
						{
							printf("Inside crafting table, activeCell = %d", tempCell);
							int retQuant = 0;
							int i = 0;
							for (auto &part : crafting.gridVec[tempCell]->ingredients)
							{
								retQuant = part.second - ReqChart.gridVec[i]->quantity;
								if (retQuant > 0)
								{
									for (auto &item : inventory.gridVec)
									{
										if (item == nullptr)
										{
											item.swap(ReqChart.gridVec[i]);
											item->quantity = retQuant;
											break;
										}
										else if (item->classID == ReqChart.gridVec[i]->classID)
										{
											item->quantity += retQuant;
											break;
										}
									}
								}
								ReqChart.gridVec[i].reset(nullptr);
								i++;
							}
							crafting.Tellinfo(itemLibrary, mx, my, ReqChart);
						}
						else
						{
							crafting.activeCell = tempCell;
						}
					}
					else if (crafting.CheckClick(mx, my) == NULLINT) // if click inside crafting area
					{
						if (crafting.gridVec[crafting.activeCell] != nullptr) // if clicked cell has a recipe
						{
							printf("Inside crafting table, activeCell == %d", crafting.activeCell);
							crafting.Tellinfo(itemLibrary, mx, my, ReqChart);
							//crafting.activeCell = NULLINT;
						}
						else
						{
							crafting.activeCell = NULLINT;
						}
					}
					else
					{
					}
				}
				else if (toolbar.activeCell == NULLINT && inventory.CheckClick(mx, my) == -2) // if inventory highlighted
				{
					inventory.TryTransfer(mx, my, toolbar);
					if (inventory.activeCell == NULLINT) // if item transferred to toolbar
					{
						// do nothing
					}
					else
					{
						inventory.TryTransfer(mx, my, ReqChart, 1);
					}
					inventory.activeCell = NULLINT;
					inventory.transfer = FALSE;
				}
				else if (inventory.activeCell == NULLINT && toolbar.CheckClick(mx, my) == -2) // if toolbar highlighted
				{
					toolbar.TryTransfer(mx, my, inventory);
					if (toolbar.activeCell == NULLINT)
					{
						// do nothing
					}
					else
					{
						toolbar.TryTransfer(mx, my, ReqChart, 1);
					}
					toolbar.activeCell = NULLINT;
					toolbar.transfer = FALSE;
				}
				else
				{
				}
				break;
			}
			else // use selected item left click
			{
				printf("IN LEFT BUTTON\n");
				if (camera.activeTool == NULLINT || toolbar.gridVec[toolbar.activeCell] == nullptr)
				{
					// do nothing
				}
				else
				{
					//toolbar.gridVec[toolbar.activeCell]->Use(camera, worldGrid, materials);
					if (worldGrid.FindBlock(camera, xGrid, yGrid, zGrid, ADD))
					{
						if (xGrid != camera.xGrid() || (yGrid != camera.yGrid() && yGrid != camera.yGrid() + 1) || zGrid != camera.zGrid())
						{
							//printf("Found one at %d %d %d\n", xGrid, yGrid, zGrid);
							int blockType = rand() % 7; // select random block
							worldGrid.AddBlock(xGrid, yGrid, zGrid, materials[blockType]);		//right now default add dirt blocks
						}
					}
					else
					{
						//printf("Did Not Find");
					}
				}
				//int itemCode = toolbar.gridVec[toolbar.activeCell]->Use();
				
				break;
			}
		case FSMOUSEEVENT_RBUTTONDOWN:
			if (camera.cursorLock)
			{
				printf("IN RIGHT BUTTON\n");
				if (worldGrid.FindBlock(camera, xGrid, yGrid, zGrid, REMOVE))
				{
					if (xGrid != camera.xGrid() || yGrid != camera.yGrid() || zGrid != camera.zGrid())
					{
						int idx = xGrid + zGrid*worldGrid.roomSize + yGrid*pow(worldGrid.roomSize, 2);
						//printf("Found one at %d %d %d\n", xGrid, yGrid, zGrid);
						int blockHealth = worldGrid.blockMap[idx]->TakeDamage(2, 1);
						if (blockHealth <= 0)
						{
							worldGrid.RemoveBlock(itemLibrary, inventory, xGrid, yGrid, zGrid);
						}
					}
				}
				else
				{
					printf("Did Not Find");
				}
			}
			break;
		}

#if !defined(_WIN32_WINNT)
		if (switchCamera)
		{
			if (camera2.cursorLock)
			{
				camera2.Update(key, worldGrid.blockMap);
			}
			worldGrid.DrawTerrain(camera2, reductionMode, key, texturesOn);
		}
		else
		{
			if (camera.cursorLock)
			{
				camera.Update(key, worldGrid.blockMap);
			}
			worldGrid.DrawTerrain(camera, reductionMode, key, texturesOn);
		}
#endif

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (!camera.cursorLock)
		{
			inventory.Draw();
			crafting.Draw();
			ReqChart.Draw();
			but.Draw(but.x, but.y);
		}

		toolbar.activeTool = camera.activeTool;
		toolbar.Draw(); // always draw the toolbar

		{ // draw crosshairs
			glLineWidth(1);
			glBegin(GL_LINES);
			glLineWidth(5);
			glColor3ub(255, 255, 255);
			glVertex2i(wid / 2 - 10, hei / 2);
			glVertex2i(wid / 2 + 10, hei / 2);
			glVertex2i(wid / 2, hei / 2 - 10);
			glVertex2i(wid / 2, hei / 2 + 10);
			glEnd();

			glLineWidth(5);
			glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			glVertex2i(wid / 2 - 12, hei / 2);
			glVertex2i(wid / 2 + 12, hei / 2);
			glVertex2i(wid / 2, hei / 2 - 12);
			glVertex2i(wid / 2, hei / 2 + 12);
			glEnd();
			glLineWidth(1);
		}

		glDisable(GL_DEPTH_TEST);

		// 2D drawing from here
		FsSwapBuffers();
		FsSleep(dt);
	}

	return 0;
}
