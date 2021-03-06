// hi from Loren
#include "Terrain.h"
#include "Decoder.h"
#include "ObjectGridClass.h"
#include "enemy.h"

void GetLocalTimeHourMinSec(int &hour,int &min,int &sec)
{
    struct tm *localTime;
    time_t t=time(NULL);
    localTime=localtime(&t);
    
    hour=localTime->tm_hour;
    min=localTime->tm_min;
    sec=localTime->tm_sec;
}

int main(void)
{
	srand((int)time(nullptr));
	FsOpenWindow(800, 100, WINWID, WINHEI, 1);
	int terminate = 0;
	int lb, mb, rb, mx, my, mouseEvent, key = 0;
	int drawCount = 0;
	int roomSize = 128;
	int gotHit = 0;
	int hitCounter = 0;
	int enemyDist = 0;
	int hitEnable = 0;
    int hour,min,sec,time4assWhoop;
    int initTime=0,deltaT=0;
    
    GetLocalTimeHourMinSec(hour,min,sec);
    initTime = hour*3600+min*60+sec;
    int numEnemy = 5;
    std::vector<enemy> enemyList;
    
	std::map<int, std::unique_ptr<Item>> itemLibrary;
	//							 <ClassCode, itemCode, name, quant, texture#, weight, range, damage, health, strength, speed, hitscan, stackable, highlight, outline, numIngedients, matCodes[numIngedients], quantities[numIngedients], craftedItemCode, craftedQuant>
	// materials,   ClassCode == 0,   0 - 100
	itemLibrary[0].reset(new Item(0, 0, "Dirt",    3, 0.0, 1, 8.0, 0,  2, 0, 0.0, true, true, false, false));
	itemLibrary[1].reset(new Item(0, 1, "Stone",   1, 5.0, 1, 8.0, 0,  4, 1, 0.0, true, true, false, false));
	itemLibrary[2].reset(new Item(0, 2, "Steel",   1, 2.0, 1, 8.0, 0,  6, 2, 0.0, true, true, false, false));
	itemLibrary[3].reset(new Item(0, 3, "Wood",    2, 7.0, 1, 8.0, 0,  3, 0, 0.0, true, true, false, false));
	itemLibrary[4].reset(new Item(0, 4, "Ruby",    1, 4.0, 1, 8.0, 0, 10, 2, 0.0, true, true, false, false));
	itemLibrary[5].reset(new Item(0, 5, "Emerald", 1, 1.0, 1, 8.0, 0,  6, 2, 0.0, true, true, false, false));
	itemLibrary[7].reset(new Item(0, 7, "Wood",    2, 7.0, 1, 8.0, 0,  3, 0, 0.0, true, true, false, false));
	itemLibrary[8].reset(new Item(0, 8, "Wood",    2, 7.0, 1, 8.0, 0,  3, 0, 0.0, true, true, false, false));
	itemLibrary[9].reset(new Item(0, 9, "Indestructible", 2, 2.0, 1, 8.0, 0, 3, 20, 0.0, true, true, false, false));

	// weapons,     ClassCode == 1, 101 - 200
	itemLibrary[101].reset(new Item(1, 101, "Stick", 1,       11.0, 2, 4.0, 1, 0,      1, 0.5, true, false, false, false));
	itemLibrary[102].reset(new Item(1, 102, "RockHammer", 1, 15.0, 3, 4.0, 1, 0, 2, 0.5, true, false, false, false));
	itemLibrary[103].reset(new Item(1, 103, "Sword", 1, 13.0, 3, 8.0, 2, 0, 2, 0.5, true, false, false, false));
	itemLibrary[104].reset(new Item(1, 104, "Gun", 1, 12.0, 3, 80.0, 1, 0, 1, 0.5, true, false, false, false));

	// consumables, ClassCode == 2, 201 - 300
	itemLibrary[201].reset(new Item(2, 201, "Orange",  6, 3.0, 1, 0.0, 0, 10, 2, 15, true, true, false, false));

	// recipes,     ClassCode == 3, 301 - 400
	int *ingredientCodes = new int[1]{ 3 };
	int *ingredientQuants = new int[1]{ 5 };
	itemLibrary[301].reset(new Item(3, 301, "Lv1 Stick", 1, 11.0, 1, 0.0, 1, 0, 0, 0.5, true, false, false, false, 1, ingredientCodes, ingredientQuants, 101, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[2]{ 3,1 };
	ingredientQuants = new int[2]{ 10,15 };
	itemLibrary[302].reset(new Item(3, 302,"Lv1 RockHammer", 1, 15.0, 1, 0.0, 1, 0, 0, 0.5, true, false, false, false, 2, ingredientCodes, ingredientQuants, 102, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[3]{ 3,1,2 };
	ingredientQuants = new int[3]{ 5,10,15 };
	itemLibrary[303].reset(new Item(3, 303, "Lv1 Sword", 1, 13.0, 1, 0.0, 1, 0, 0, 0.5, true, false, false, false, 3, ingredientCodes, ingredientQuants, 103, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[2]{ 0,3 };
	ingredientQuants = new int[2]{ 20, 5 };
	itemLibrary[304].reset(new Item(3, 304, "Wood Armor", 1, 21.0, 5, 0.0, 1, 0, 2, 1.0, true, false, false, false, 2, ingredientCodes, ingredientQuants, 401, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[2]{ 3, 1 };
	ingredientQuants = new int[2]{ 10, 5 };
	itemLibrary[305].reset(new Item(3, 305, "Rock Armor", 1, 20.0, 4, 0.0, 1, 0, 4, 0.9, true, false, false, false, 2, ingredientCodes, ingredientQuants, 402, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[3]{ 3, 1, 2 };
	ingredientQuants = new int[3]{ 10, 5, 10 };
	itemLibrary[306].reset(new Item(3, 306, "Steel Armor", 1, 19.0, 3, 0.0, 1, 0, 6, 0.8, true, false, false, false, 3, ingredientCodes, ingredientQuants, 403, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[4]{ 3,  1, 5, 4 };
	ingredientQuants = new int[4]{ 20, 15, 4, 2 };
	itemLibrary[307].reset(new Item(3, 307, "Gemstone Armor", 1, 18.0, 10, 0.0, 1, 0, 6, 1.1, true, false, false, false, 4, ingredientCodes, ingredientQuants, 404, 1));
	delete[] ingredientCodes, ingredientQuants;
	ingredientCodes = new int[4]{ 3, 1, 5, 4 };
	ingredientQuants = new int[4]{ 35, 20, 10, 5 };
	itemLibrary[308].reset(new Item(3, 308, "Gun", 1, 12.0, 10, 0.0, 1, 0, 1, 0.5, true, false, false, false, 4, ingredientCodes, ingredientQuants, 104, 1));
	delete[] ingredientCodes, ingredientQuants;

	// armor,	    ClassCode == 4, 401 - 500
	itemLibrary[401].reset(new Item(4, 401, "Wood Armor",     1, 21.0, 1,  0.0, 0, 2, 2,  1.0, true, false, false, false));
	itemLibrary[402].reset(new Item(4, 402, "Rock Armor",     1, 20.0, 1,  0.0, 0, 3, 4,  0.9, true, false, false, false));
	itemLibrary[403].reset(new Item(4, 403, "Steel Armor",    1, 19.0, 1,  0.0, 0, 6, 6,  0.8, true, false, false, false));
	itemLibrary[404].reset(new Item(4, 404, "Gemstone Armor", 1, 18.0, 1,  0.0, 0, 6, 10, 1.1, true, false, false, false));

	//					<itemCode, texX, texY, r,g,b, strength, health, quant
	std::vector<int> dirt =      { 0,		0,0,255,255,255,  0,  2, 3 };
	std::vector<int> stone =     { 1,		5,0,255,255,255,  1,  4, 1 };
	std::vector<int> steel =	 { 2,		2,0,255,255,255,  1,  6, 0 };
	std::vector<int> wood =      { 3,	    1,1,255,255,255,  0,  3, 2 };
	std::vector<int> ruby =      { 4,		4,0,255,255,255,  2, 10, 0 };
	std::vector<int> emerald =   { 5,		1,0,255,255,255,  2,  6, 0 };
	std::vector<int> orange =  { 201,       3,0,255,255,255,  0, 10, 6 };
	std::vector<int> greenLeaf = { 3,       3,1,255,255,255,  0,  3, 2 };
	std::vector<int> redLeaf =   { 3,       4,1,255,255,255,  0,  3, 2 };
	std::vector<int> mapEdge =   { 9,       2,0,255,255,255, 20,  3, 2 };


	std::vector<std::vector<int>> materials;
	materials.push_back(dirt);
	materials.push_back(stone);
	materials.push_back(steel);
	materials.push_back(wood);
	materials.push_back(ruby);
	materials.push_back(emerald);
	materials.push_back(orange);
	materials.push_back(greenLeaf);
	materials.push_back(redLeaf);
	materials.push_back(mapEdge);

	// structures <itemCode, xGrid, yGrid, zGrid>
	// Tree type 1
	std::vector<std::vector<int>> tree1;
	tree1.push_back({ 3,   0, 0, 0 });
	tree1.push_back({ 3,   0, 1, 0 });
	tree1.push_back({ 3,   0, 2, 0 });
	tree1.push_back({ 3,   -1, 2, 0 });
	tree1.push_back({ 7,   1, 2, 0 });
	tree1.push_back({ 8,   0, 2, 1 });
	tree1.push_back({ 7,   0, 2, -1 });
	tree1.push_back({ 6,   0, 3, 0 });
	tree1.push_back({ 8,   1, 3, 0 });
	tree1.push_back({ 7,   -1, 3, 0 });

	//Tree Type 2
	std::vector<std::vector<int>> tree2;
	tree2.push_back({ 3,   0, 0, 0 });
	tree2.push_back({ 3,   0, 1, 0 });
	tree2.push_back({ 3,   0, 2, 0 });
	tree2.push_back({ 3,   0, 3, 0 });
	tree2.push_back({ 7,   -1, 3, 0 });
	tree2.push_back({ 7,   1, 3, 0 });
	tree2.push_back({ 8,   2, 3, 0 });
	tree2.push_back({ 7,   -2, 3, 0 });
	tree2.push_back({ 8,   0, 4, 0 });
	tree2.push_back({ 7,   -1, 4, 0 });
	tree2.push_back({ 6,   1, 4, 0 });
	tree2.push_back({ 3,   0, 5, 0 });
	tree2.push_back({ 7,   0, 3, -1 });
	tree2.push_back({ 7,   0, 3, 1 });
	tree2.push_back({ 8,   0, 3, 2 });
	tree2.push_back({ 6,   0, 3, -2 });
	tree2.push_back({ 7,   0, 4, 1 });
	tree2.push_back({ 8,   0, 4, -1 });

	//Shrub
	std::vector<std::vector<int>> shrub;
	shrub.push_back({ 7,  0,0,0 });
	shrub.push_back({ 7,  1,0,0 });
	shrub.push_back({ 7,  -1,0,0 });
	shrub.push_back({ 7,  0,0,1 });
	shrub.push_back({ 7,  -1,0,1 });
	shrub.push_back({ 7,  1,0,1 });
	shrub.push_back({ 7,  -1,0,-1 });
	shrub.push_back({ 7,  1,0,-1 });
	shrub.push_back({ 7,  0,0,-1 });
	shrub.push_back({ 7,  2,0,0 });
	shrub.push_back({ 7,  -2,0,0 });
	shrub.push_back({ 7,  0,0,-2 });
	shrub.push_back({ 7,  0,0,2 });
	shrub.push_back({ 7,  0,1,0 });
	shrub.push_back({ 7,  1,1,0 });
	shrub.push_back({ 7,  -1,1,0 });
	shrub.push_back({ 7,  0,1,-1 });
	shrub.push_back({ 7,  0,1,1 });

	//Stone Structure
	std::vector<std::vector<int>> stonetower;
	stonetower.push_back({ 1,  0,0,0 });
	stonetower.push_back({ 1,  1,0,0 });
	stonetower.push_back({ 1,  -1,0,0 });
	stonetower.push_back({ 1,  0,0,1 });
	stonetower.push_back({ 1,  0,0,-1 });
	stonetower.push_back({ 1,  -1,0,1 });
	stonetower.push_back({ 1,  1,0,1 });
	stonetower.push_back({ 1,  -1,0,-1 });
	stonetower.push_back({ 1,  1,0,-1 });
	stonetower.push_back({ 1,  0,1,0 });
	stonetower.push_back({ 1,  1,1,0 });
	stonetower.push_back({ 1,  -1,1,0 });
	stonetower.push_back({ 1,  0,1,1 });
	stonetower.push_back({ 1,  0,1,-1 });
	stonetower.push_back({ 1,  -1,1,1 });
	stonetower.push_back({ 1,  1,1,1 });
	stonetower.push_back({ 1,  -1,1,-1 });
	stonetower.push_back({ 1,  1,1,-1 });
	stonetower.push_back({ 1,  1,2,0 });
	stonetower.push_back({ 1,  -1,2,0 });
	stonetower.push_back({ 1,  0,2,1 });
	stonetower.push_back({ 1,  0,2,-1 });
	stonetower.push_back({ 1,  -1,2,1 });
	stonetower.push_back({ 1,  1,2,1 });
	stonetower.push_back({ 1,  -1,2,-1 });
	stonetower.push_back({ 1,  1,2,-1 });
	stonetower.push_back({ 1,  0,2,0 });
	stonetower.push_back({ 1,  1,2,0 });
	stonetower.push_back({ 1,  -1,2,0 });
	stonetower.push_back({ 1,  0,2,1 });
	stonetower.push_back({ 1,  0,2,-1 });
	stonetower.push_back({ 1,  -1,2,1 });
	stonetower.push_back({ 1,  1,2,1 });
	stonetower.push_back({ 1,  -1,2,-1 });
	stonetower.push_back({ 1,  1,2,-1 });

	std::vector< std::vector<std::vector<int>>> structureLibrary;
	structureLibrary.push_back(tree1);
	structureLibrary.push_back(tree2);
	structureLibrary.push_back(shrub);
	structureLibrary.push_back(stonetower);

	Grid inventory(20, 20, 350, 500, 20);
	Grid toolbar(100, 500, 700, 600, 10);
	Grid crafting(400, 20, 700, 240, 10);
	Grid ReqChart(400, 260, 700, 500, 10);
	Grid ActiveWepArmor(WINWID - 250, WINHEI - 150, WINWID - 50, WINHEI - 50, 2);
	Button but;

	crafting.AddPermElement(itemLibrary);
    
	Terrain worldGrid(roomSize, 2, materials, structureLibrary);

	CameraObject camera(roomSize, (double)(roomSize*blockSize / 2), (double)(roomSize*blockSize / 2), (double)(roomSize*blockSize / 2));
	CameraObject camera2(roomSize, (double)(roomSize*blockSize / 2), (double)(roomSize*blockSize / 2), (double)(roomSize*blockSize / 2));
	
	worldGrid.texId = decodePng();
	GLuint texId = decodePng();

	glBindTexture(GL_TEXTURE_2D, texId);	// Select the current texture.

	worldGrid.AddBlock(5, 5, 5, orange);

	camera.playerBlock.roomSize = worldGrid.roomSize;
	camera2.playerBlock.roomSize = worldGrid.roomSize;

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
	glBindTexture(GL_TEXTURE_2D, texId);	// Select the current texture.
	int xGrid, yGrid, zGrid;
	char* guiText;

    int waveCount=0;int release =0;

	while (0 == terminate)
	{
        int wid1, hei1;
        FsGetWindowSize(wid1, hei1);
        // Set up 2D drawing
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, (float)wid1 - 1, (float)hei1 - 1, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // draw player health
        int x000 = 50, y0 = 50;
        glColor3ub(0, 0, 0);
        glRasterPos2d(x000, y0);
        char playerHealth[20];
        sprintf(playerHealth, "Get ready for wave %d", waveCount        );
        YsGlDrawFontBitmap20x32(playerHealth);
        
        
		FsPollDevice();
        GetLocalTimeHourMinSec(hour,min,sec);
        time4assWhoop = hour*3600+min*60+sec;
        deltaT = time4assWhoop -initTime;

        
        if(deltaT>=120)
        {
            initTime = hour*3600+min*60+sec;
            deltaT =0;
            waveCount++;
            release = 1;
        }

        if(waveCount>0 && release==1)
        {
            for (int ii = 0; ii < waveCount*numEnemy; ii++)
            {
                enemyList.push_back(enemy(roomSize));                
            }
            if(waveCount%3==0)
            {
                for (int ii = 0; ii < (waveCount/3)*numEnemy; ii++)
                {
                    enemyList.push_back(enemy(roomSize,rand()%roomSize,(rand()%roomSize)/2,rand()%roomSize,20,10,4,2,2.0));
                    
                }
            }
            release=0;
        }

        
        
		int wid, hei;
		FsGetWindowSize(wid, hei);
		toolbar.UpdateToolbarSize(wid, hei);
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
		case FSKEY_P:
			enemyList.push_back(enemy(roomSize));
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

		//Enemy Chase Dynamics
		if (camera.cursorLock)
		{
			int calcHit, damage2Player = 0;
			int bumpCheck = 0;
			int enemyNum = 0;
			for (auto &enemy : enemyList)
			{
				int calcHit, damage2Player = 0;
				int bumpCheck = 0;
				int enemyNum = 0;

				if (enemy.frenemy.health <= 0) // remove enemy if it died
				{
					printf("DELETING ENEMY\n");
					enemyList.erase(enemyList.begin() + enemyNum);
				}

				for (auto &enemy : enemyList) // environment collision
				{
					gotHit = enemy.chase(camera, worldGrid.blockMap);
				}

				if (gotHit == 1)
				{
					hitEnable = 1;
					camera.xVel += enemy.chaseVec[0];
					camera.zVel += enemy.chaseVec[2];
					camera.yVel += .02;
				}

				if (hitEnable == 1)
				{
					camera.vertVel = 2;		//enemy bump function here
				}

				for (auto &enemy : enemyList)
				{
					if (enemy.xGrid() == camera.xGrid() && enemy.yGrid() == camera.yGrid() && enemy.zGrid() == camera.zGrid())
					{
						bumpCheck++;
					}
				}

				if (bumpCheck == 0)
				{
					for (auto &enemy : enemyList)
					{
						damage2Player += (enemy.hitPlayer*enemy.damage);
						enemy.hitPlayer = 0;
					}
					bumpCheck = 0;
					hitEnable = 0;
				}

				camera.health -= (int)(damage2Player / (double)camera.armor);
				damage2Player = 0;
			}
		}

		// draw enemies
		for (auto &enemy : enemyList)
		{
			enemy.drawEnemy();
		}

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

		if (camera.activeTool != NULLINT)
		{
			//printf("activeTool = %d\n", camera.activeTool);
			toolbar.activeTool = camera.activeTool;
			//printf("classType = %d\n", toolbar.gridVec[toolbar.activeTool]->classType);
			if (toolbar.gridVec[camera.activeTool] == nullptr || toolbar.gridVec[toolbar.activeTool]->classType == 4)
			{
				ActiveWepArmor.gridVec[0].reset();
			}
			else
			{
				printf("setting new wep\n");
				if (ActiveWepArmor.gridVec[0] == nullptr || toolbar.gridVec[toolbar.activeTool]->classID != ActiveWepArmor.gridVec[0]->classID)
				{
					switch (toolbar.gridVec[toolbar.activeTool]->classType)
					{
					case 0:
						ActiveWepArmor.gridVec[0].reset(new Material);
						ActiveWepArmor.gridVec[0]->copyFrom(itemLibrary[toolbar.gridVec[toolbar.activeTool]->classID]);
						break;
					case 1:
						ActiveWepArmor.gridVec[0].reset(new Weapon);
						ActiveWepArmor.gridVec[0]->copyFrom(itemLibrary[toolbar.gridVec[toolbar.activeTool]->classID]);
						break;
					case 2:
						ActiveWepArmor.gridVec[0].reset(new Useable);
						ActiveWepArmor.gridVec[0]->copyFrom(itemLibrary[toolbar.gridVec[toolbar.activeTool]->classID]);
						break;
					}
				}
			}
		}
#endif
		if (!camera.cursorLock)
		{
			switch (FsGetMouseEvent(lb, mb, rb, mx, my))
			{
			case FSMOUSEEVENT_LBUTTONDOWN:
				printf("USER CLICKED\n");
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
							//printf("Inside crafting table, activeCell == %d", crafting.activeCell);
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

			case FSMOUSEEVENT_RBUTTONDOWN:
			{
				// do nothing
			}
			break;
			}
		}
		else
		{
			//printf("USER CLICKED IN WORLD\n");
			int damage = 1, strength = 0, range = 8;
			switch (FsGetMouseEvent(lb, mb, rb, mx, my))
			{
			case FSMOUSEEVENT_LBUTTONDOWN:
				printf("WORLD LEFT BUTTON\n");
				if (camera.activeTool == NULLINT || toolbar.gridVec[toolbar.activeTool] == nullptr) // no item selected
				{
					// do nothing
					if (camera.activeTool == 9)
					{
						if (worldGrid.FindBlock(camera, range, xGrid, yGrid, zGrid, ADD))
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
							printf("Did Not Find");
						}
					}
					break;
				}
				else // item is selected
				{
					printf("Class Code = %d\n", toolbar.gridVec[toolbar.activeTool]->classType);

					switch (toolbar.gridVec[toolbar.activeTool]->classType)
					{
					case 0: // material
						range = toolbar.gridVec[toolbar.activeTool]->range;
						if (worldGrid.FindBlock(camera, range, xGrid, yGrid, zGrid, ADD))
						{
							if (xGrid != camera.xGrid() || (yGrid != camera.yGrid() && yGrid != camera.yGrid() + 1) || zGrid != camera.zGrid())
							{
								//printf("Found one at %d %d %d\n", xGrid, yGrid, zGrid);
								worldGrid.AddBlock(xGrid, yGrid, zGrid, materials[toolbar.gridVec[toolbar.activeTool]->classID]);		//right now default add dirt blocks
								toolbar.gridVec[toolbar.activeTool]->quantity--;
								if (toolbar.gridVec[toolbar.activeTool]->quantity == 0)
								{
									toolbar.gridVec[toolbar.activeTool].reset();
								}
							}
						}
						break;
					case 1: // weapons (changes damage, strength, and range of attack)
						damage = toolbar.gridVec[toolbar.activeTool]->damage;
						strength = toolbar.gridVec[toolbar.activeTool]->strength;
						range = toolbar.gridVec[toolbar.activeTool]->range;
						goto ATTACK;
					case 2: // consumables (HP)
						if (camera.health != camera.maxHealth)
						{
							camera.health += toolbar.gridVec[toolbar.activeTool]->health;
							if (camera.health > camera.maxHealth)
							{
								camera.health = camera.maxHealth;
							}
							toolbar.gridVec[toolbar.activeTool]->quantity--;
							if (toolbar.gridVec[toolbar.activeTool]->quantity == 0)
							{
								toolbar.gridVec[toolbar.activeTool].reset();
							}
						}
						break;
					case 4: // armor
						if (ActiveWepArmor.gridVec[1] == nullptr || toolbar.gridVec[toolbar.activeTool]->classID != ActiveWepArmor.gridVec[1]->classID)
						{
							printf("In here\n");
							camera.armor = toolbar.gridVec[toolbar.activeTool]->strength;
							camera.speedMult = toolbar.gridVec[toolbar.activeTool]->speed;
							camera.maxHealth = toolbar.gridVec[toolbar.activeTool]->health*camera.baseMaxHealth;
							camera.maxJump = toolbar.gridVec[toolbar.activeTool]->weight;
							ActiveWepArmor.gridVec[1].reset(new Armor);
							ActiveWepArmor.gridVec[1]->copyFrom(toolbar.gridVec[toolbar.activeTool]);
						}
						else
						{
							//do nothing
						}
						break;
					}
				}
				break;
			//case FSMOUSEEVENT_MBUTTONDOWN:
				
			case FSMOUSEEVENT_RBUTTONDOWN:
			ATTACK:
				printf("WORLD RIGHT BUTTON\n");
				int i = 0;
				for (auto &enemy : enemyList) // check all enemies first
				{
					enemyDist = camera.findEnemy(16, enemy.frenemy);
					printf("hitEnemy %d, enemy is %d away\n", i, enemyDist);
					if (enemyDist != NULLINT) // enemy found in the distance
					{
						if (worldGrid.FindBlock(camera, enemyDist/blockSize, xGrid, yGrid, zGrid, REMOVE)) // checkForBlocks between enemy and player
						{
							if (xGrid != camera.xGrid() || yGrid != camera.yGrid() || zGrid != camera.zGrid())
							{
								int idx = xGrid + zGrid*worldGrid.roomSize + yGrid*pow(worldGrid.roomSize, 2);
								//printf("Found one at %d %d %d\n", xGrid, yGrid, zGrid);
								int blockHealth = worldGrid.blockMap[idx]->TakeDamage(strength, damage);
								if (blockHealth <= 0)
								{
									worldGrid.RemoveBlock(itemLibrary, inventory, xGrid, yGrid, zGrid);
									goto end;
								}
							}
						}
						else // if no block in between
						{
							printf("HITTING ENEMY\n");
							enemy.frenemy.health -= 5*damage;
							goto end;
						}
					}
					i++;
				}

				if (worldGrid.FindBlock(camera, range, xGrid, yGrid, zGrid, REMOVE)) // no enemies hit, try hitting block
				{
					if (xGrid != camera.xGrid() || yGrid != camera.yGrid() || zGrid != camera.zGrid())
					{
						int idx = xGrid + zGrid*worldGrid.roomSize + yGrid*pow(worldGrid.roomSize, 2);
						//printf("Found one at %d %d %d\n", xGrid, yGrid, zGrid);
						int blockHealth = worldGrid.blockMap[idx]->TakeDamage(strength, damage);
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
				end: // hitting an enemy or block in the enemy check jumps to here
					break;
			}
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

		if (camera.activeTool != NULLINT)
		{
			//printf("activeTool = %d\n", camera.activeTool);
			toolbar.activeTool = camera.activeTool;
			//printf("classType = %d\n", toolbar.gridVec[toolbar.activeTool]->classType);
			if (toolbar.gridVec[camera.activeTool] == nullptr || toolbar.gridVec[toolbar.activeTool]->classType == 4)
			{
				ActiveWepArmor.gridVec[0].reset();
			}
			else
			{
				printf("setting new wep\n");
				if (ActiveWepArmor.gridVec[0] == nullptr || toolbar.gridVec[toolbar.activeTool]->classID != ActiveWepArmor.gridVec[0]->classID)
				{
					switch (toolbar.gridVec[toolbar.activeTool]->classType)
					{
					case 0:
						ActiveWepArmor.gridVec[0].reset(new Material);
						ActiveWepArmor.gridVec[0]->copyFrom(itemLibrary[toolbar.gridVec[toolbar.activeTool]->classID]);
						break;
					case 1:
						ActiveWepArmor.gridVec[0].reset(new Weapon);
						ActiveWepArmor.gridVec[0]->copyFrom(itemLibrary[toolbar.gridVec[toolbar.activeTool]->classID]);
						break;
					case 2:
						ActiveWepArmor.gridVec[0].reset(new Useable);
						ActiveWepArmor.gridVec[0]->copyFrom(itemLibrary[toolbar.gridVec[toolbar.activeTool]->classID]);
						break;
					}
				}
			}
		}
#endif

		// Set up 2D drawing
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		{ // Draw GUI
			if (!camera.cursorLock) // draw inventory
			{
				inventory.Draw(texId);
				crafting.Draw(texId);
				ReqChart.Draw(texId);
				but.Draw(but.x, but.y);
			}
			else // draw crosshairs
			{
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

				ActiveWepArmor.UpdateWepBarSize(wid, hei);
				ActiveWepArmor.Draw(texId);
			}

			// draw toolbar
			FsGetWindowSize(wid, hei);
			toolbar.activeTool = camera.activeTool;
			toolbar.Draw(texId); // always draw the toolbar

			// draw player health
			int x0 = 50, y0 = 50;
			glColor3ub(0, 0, 0);
			glRasterPos2d(x0, y0);
			guiText = new char[50];
			sprintf(guiText, "Player Health = %2d / %2d", camera.health, camera.maxHealth);
			YsGlDrawFontBitmap20x32(guiText);

			glBegin(GL_QUADS);
			glColor3ub(0, 255, 0);
			glVertex2i(x0 - 10, y0 + 8);
			glVertex2i(x0 - 10, y0 - 8 - 32);
			glVertex2i(x0 + 20 * strlen(guiText)*(double)camera.health / (double)camera.maxHealth + 10, y0 - 8 - 32);
			glVertex2i(x0 + 20 * strlen(guiText)*(double)camera.health / (double)camera.maxHealth + 10, y0 + 8);

			glColor3ub(255, 0, 0);
			glVertex2i(x0 - 10, y0 + 8);
			glVertex2i(x0 - 10, y0 - 8 - 32);
			glVertex2i(x0 + 20 * strlen(guiText) + 10, y0 - 8 - 32);
			glVertex2i(x0 + 20 * strlen(guiText) + 10, y0 + 8);

			glEnd();
			delete[] guiText;

			y0 += 50;

			// draw player damage
			glColor3ub(0, 0, 0);
			glRasterPos2d(x0, y0);
			guiText = new char[50];
			if (ActiveWepArmor.gridVec[0] == nullptr)
			{
				sprintf(guiText, "Damage = %d", 1);
			}
			else
			{
				sprintf(guiText, "Damage = %d", ActiveWepArmor.gridVec[0]->damage);
			}
			YsGlDrawFontBitmap12x16(guiText);
			delete[] guiText;
			y0 += 25;

			// draw player strength
			glColor3ub(0, 0, 0);
			glRasterPos2d(x0, y0);
			guiText = new char[50];
			if (ActiveWepArmor.gridVec[0] == nullptr)
			{
				sprintf(guiText, "Power = %d", 0);
			}
			else
			{
				sprintf(guiText, "Power = %d", ActiveWepArmor.gridVec[0]->strength);
			}
			YsGlDrawFontBitmap12x16(guiText);
			delete[] guiText;
			y0 += 25;

			// draw player range
			glColor3ub(0, 0, 0);
			glRasterPos2d(x0, y0);
			guiText = new char[50];
			if (ActiveWepArmor.gridVec[0] == nullptr)
			{
				sprintf(guiText, "Range = %lf", 8.0*blockSize);
			}
			else
			{
				sprintf(guiText, "Range = %lf", ActiveWepArmor.gridVec[0]->range*blockSize);
			}
			YsGlDrawFontBitmap12x16(guiText);
			delete[] guiText;
			y0 += 25;

			// draw player armor
			glColor3ub(0, 0, 0);
			glRasterPos2d(x0, y0);
			guiText = new char[50];
			sprintf(guiText, "Armor = %2d", camera.armor);
			YsGlDrawFontBitmap12x16(guiText);
			delete[] guiText;
			y0 += 25;

			// draw player speed
			glColor3ub(0, 0, 0);
			glRasterPos2d(x0, y0);
			guiText = new char[50];
			sprintf(guiText, "Speed = %lfx", camera.speedMult);
			YsGlDrawFontBitmap12x16(guiText);
			delete[] guiText;
			y0 += 25;

			// draw player jump
			glColor3ub(0, 0, 0);
			glRasterPos2d(x0, y0);
			guiText = new char[50];
			sprintf(guiText, "Jumps = %d", camera.jumps);
			YsGlDrawFontBitmap12x16(guiText);
			delete[] guiText;

			glColor4ub(200, 200, 200, 1);
			glBegin(GL_QUADS);
			glVertex2i(45, 45);
			glVertex2i(270, 45);
			glVertex2i(270, 250);
			glVertex2i(45, 250);
			glEnd();
            
            int x00 = wid - 200, y00 = 50               ;
            glColor3ub(0, 0, 0);
            glRasterPos2d(x00, y00      );
            char waveDisp[20];
            sprintf(waveDisp, "Wave # %d", waveCount);
            YsGlDrawFontBitmap20x32(waveDisp);
            
            glBegin(GL_QUADS);
            glColor3ub(255, 0, 0);
            glVertex2i(x00 - 10, y00 + 8);
            glVertex2i(x00 - 10, y00 - 8 - 32);
            glVertex2i(x00 + 20 * strlen(waveDisp) + 10, y00 - 8 - 32);
            glVertex2i(x00 + 20 * strlen(waveDisp) + 10, y00 + 8);
            
            glEnd();
        }
        
        glDisable(GL_DEPTH_TEST);

		// 2D drawing from here
		FsSwapBuffers();
		FsSleep(dt);
	}

	return 0;
}
