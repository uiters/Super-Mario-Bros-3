#include <iostream>
#include <fstream>

#include "Switch.h"
#include "Block.h"
#include "Card.h"
#include "Abyss.h"
#include "Portal.h"
#include "BoomerangBro.h"
#include "Boomerang.h"
#include "BreakableBrick.h"
#include "Brick.h"
#include "MusicalBrick.h"
#include "Coin.h"
#include "Define.h"
#include "FirePlant.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "Plant.h"
#include "PlayScene.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "Sprites.h"
#include "Textures.h"
#include "Utils.h"
#include "Score.h"
#include "PlantBullet.h"
#include "PlantBullet.h"


using namespace std;

#define GAMEDONE_1_DIFF_X	80
#define GAMEDONE_1_DIFF_Y	20
#define GAMEDONE_2_DIFF_X	60
#define GAMEDONE_2_DIFF_Y	45

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_BLOCK				4
#define OBJECT_TYPE_COIN				6
#define OBJECT_TYPE_PIRANHAPLANT		7
#define OBJECT_TYPE_LEAF				36
#define OBJECT_TYPE_MUSHROOM			37
#define OBJECT_TYPE_BOOMERANGBROTHER	48
#define OBJECT_TYPE_CARD				57
#define OBJECT_TYPE_FIREPIRANHAPLANT	70
#define OBJECT_TYPE_QUESTIONBRICK		142
#define OBJECT_TYPE_BREAKABLEBRICK		112
#define OBJECT_TYPE_MUSICALBRICK		150
#define OBJECT_TYPE_ABYSS				113
#define GRID							999

#define OBJECT_TYPE_PORTAL	50

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlaySceneKeyHandler(this);
	cam = Camera::GetInstance();
	cam->SetCameraPosition(0, 0);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
	if (ID == GAMEDONE1_SPRITE_ID)
		gamedone1 = CSprites::GetInstance()->Get(ID);
	if (ID == GAMEDONE2_SPRITE_ID)
		gamedone2 = CSprites::GetInstance()->Get(ID);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id
	int ani_set_id = atoi(tokens[0].c_str());
	LPANIMATION_SET s;
	if (CAnimationSets::GetInstance()->animation_sets[ani_set_id] != NULL)
		s = CAnimationSets::GetInstance()->animation_sets[ani_set_id];
	else
		s = new CAnimationSet();
	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	wstring path = ToWSTR(line);
	ParseObjFromFile(path.c_str());

}

void CPlayScene::ParseObjFromFile(LPCWSTR path)
{
	ifstream f;
	f.open(path);
	DebugOut(L"path::%s", path);

	if (!f)
		DebugOut(L"\nFailed to open object file!");
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);

		//DebugOut(L"--> %s\n", ToWSTR(line).c_str());

		if (line[0] == '#') continue;
		if (tokens.size() < 3) continue; // skip invalid lines - an object set must have at least id, x, y


		int ani_set_id, tag = 0, option_tag_1 = 0, option_tag_2 = 0, option_tag_3 = 0;
		int object_type = atoi(tokens[0].c_str());
		float  x, y;
		if (object_type != 999)
		{
			x = atof(tokens[1].c_str());
			y = atof(tokens[2].c_str());

			ani_set_id = atoi(tokens[3].c_str());
			if (tokens.size() >= 5)
				tag = atof(tokens[4].c_str());
			if (tokens.size() >= 6)
				option_tag_1 = atof(tokens[5].c_str());
			if (tokens.size() >= 7)
				option_tag_2 = atof(tokens[6].c_str());
			if (tokens.size() >= 8)
				option_tag_3 = atof(tokens[7].c_str());
		}

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		CGameObject* obj = NULL;

		switch (object_type)
		{
		case OBJECT_TYPE_MARIO:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y);
			player = (CMario*)obj;
			DebugOut(L"[INFO] Player object created!\n");
			break;
		case OBJECT_TYPE_GOOMBA:
			obj = new CGoomba();
			obj->SetTag(tag);
			obj->SetType(MOVING);

			break;
		case OBJECT_TYPE_BRICK:
			obj = new CBrick();
			obj->SetTag(tag);
			break;
		case OBJECT_TYPE_QUESTIONBRICK:
			obj = new CQuestionBrick(option_tag_1, option_tag_2, option_tag_3); // op_1: reward  --- op_2: appearance of the brick  ------- op_3: repeating
			if (tokens.size() >= 8)
			{
				int nboitem = atoi(tokens[7].c_str());
				if (nboitem > 0)
					((CQuestionBrick*)obj)->items = nboitem;
			}
			((CQuestionBrick*)obj)->start_y = y;
			break;
		case OBJECT_TYPE_BREAKABLEBRICK:
			obj = new CBreakableBrick();
			break;
		case OBJECT_TYPE_MUSICALBRICK:
			obj = new CMusicalBrick();
			((CMusicalBrick*)obj)->start_y = y;
			break;
		case OBJECT_TYPE_KOOPAS:
			obj = new CKoopas();
			obj->SetTag(tag);
			((CKoopas*)obj)->start_tag = tag;
			obj->SetType(MOVING);
			((CKoopas*)obj)->start_x = x;
			((CKoopas*)obj)->start_y = y;
			break;
		case OBJECT_TYPE_BOOMERANGBROTHER:
			obj = new CBoomerangBrother();
			obj->SetType(MOVING);
			((CBoomerangBrother*)obj)->start_x = x;
			break;
		case OBJECT_TYPE_BLOCK:
			obj = new CBlock();
			break;
		case OBJECT_TYPE_ABYSS:
			obj = new CAbyss();
			break;
		case OBJECT_TYPE_PIRANHAPLANT:
			obj = new CPlant();
			((CPlant*)obj)->SetLimitY(y);
			obj->SetType(MOVING);
			break;
		case OBJECT_TYPE_FIREPIRANHAPLANT:
			obj = new CFirePlant(tag);
			((CFirePlant*)obj)->SetLimitY(y);
			obj->SetType(MOVING);
			break;
		case OBJECT_TYPE_COIN:
			obj = new CCoin(tag);
			obj->SetType(IGNORE);
			break;
		case OBJECT_TYPE_CARD:
			obj = new CCard();
			break;
		case OBJECT_TYPE_PORTAL:
		{
			int scene_id = atoi(tokens[4].c_str());
			int isToExtraScene = atoi(tokens[5].c_str());
			float start_x = 0, start_y = 0;
			start_x = atoi(tokens[6].c_str());
			start_y = atoi(tokens[7].c_str());
			obj = new CPortal(scene_id, start_x, start_y);
			int pu = atoi(tokens[8].c_str());
			if (pu == 1)
				((CPortal*)obj)->pipeUp = true;
			else
				((CPortal*)obj)->pipeUp = false;
			obj->SetTag(isToExtraScene);
			break;
		}
		case GRID:
		{
			int gridCols = atoi(tokens[1].c_str());
			int gridRows = atoi(tokens[2].c_str());
			grid = new Grid(gridCols, gridRows);
			DebugOut(L"\nParseSection_GRID: Done\n");
			break;
		}
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
		}
		if (object_type != GRID)
		{
			// General object setup
			obj->SetPosition(x, y);
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
		}
		// Insert objects to grid from file
		if (object_type != OBJECT_TYPE_MARIO && object_type != GRID)
		{
			int gridCol = (int)atoi(tokens[tokens.size() - 1].c_str());
			int gridRow = (int)atoi(tokens[tokens.size() - 2].c_str());
			//add unit into grid
			Unit* unit = new Unit(grid, obj, gridRow, gridCol);
		}
	}
	f.close();
	grid->Out();
}

void CPlayScene::_ParseSection_TILEMAP_DATA(string line)
{

	int ID, rowMap, columnMap, columnTile, rowTile, totalTiles;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;
	f.open(path);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles;
	//Init Map Matrix
	DebugOut(L"ID %d>> rowMap %d>> columnMap %d>> rowTile %d>> columnTile %d>> totalTiles %d", ID, rowMap, columnMap, rowTile, columnTile, totalTiles);
	int** TileMapData = new int* [rowMap];
	for (int i = 0; i < rowMap; i++)
	{
		TileMapData[i] = new int[columnMap];
		for (int j = 0; j < columnMap; j++)
			f >> TileMapData[i][j];
	}
	f.close();

	current_map = new CMap(ID, rowMap, columnMap, rowTile, columnTile, totalTiles);
	current_map->ExtractTileFromTileSet();
	current_map->SetTileMapData(TileMapData);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }

		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[TILEMAP DATA]") {
			section = SCENE_SECTION_TILEMAP_DATA; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP_DATA: _ParseSection_TILEMAP_DATA(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Resources\\Textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	fonts = new CFont();
	hud = new HUD();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	if (player == NULL) return;

	cam = Camera::GetInstance();

	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();
	units.clear();
	objectsRenderFirst.clear();
	objectsRenderSecond.clear();
	objectsRenderThird.clear();
	objects.clear();
	grid->Get(cam, units);

	for (UINT i = 0; i < units.size(); i++)
	{
		LPGAMEOBJECT tmp;
		LPGAMEOBJECT obj = units[i]->GetObj();
		objects.push_back(obj);
		//CalRevivable
		for each (LPGAMEOBJECT object in objects)
		{
			if (dynamic_cast<CKoopas*>(object) && !dynamic_cast<CKoopas*>(object)->CalRevivable()
				&& object->isEnable == false)
			{
				object->isEnable = true;
				object->isDestroyed = false;
			}

		}

		if (dynamic_cast<CGoomba*> (obj) || dynamic_cast<CKoopas*> (obj)
			|| dynamic_cast<CPlant*> (obj)
			|| dynamic_cast<CFirePlant*> (obj) || dynamic_cast<CCoin*> (obj)
			|| dynamic_cast<CBoomerangBrother*> (obj)
			|| dynamic_cast<CMushroom*> (obj) && obj->state == MUSHROOM_STATE_UP
			|| dynamic_cast<CLeaf*> (obj) && obj->state == LEAF_STATE_UP
			|| dynamic_cast<CSwitch*> (obj))
			objectsRenderFirst.push_back(obj);
		else if (dynamic_cast<CBrick*> (obj) && obj->tag != WOOD && obj->tag != PLATFORM
			|| (dynamic_cast<CQuestionBrick*> (obj) || (dynamic_cast<CBreakableBrick*> (obj)))
			|| dynamic_cast<CMusicalBrick*>(obj)
			|| dynamic_cast<CBrick*>(obj))
			objectsRenderSecond.push_back(obj);
		else if (dynamic_cast<CFireBullet*> (obj) || dynamic_cast<CPlantBullet*> (obj)
			|| dynamic_cast<CMushroom*>(obj) && obj->state == MUSHROOM_STATE_WALK
			|| dynamic_cast<CLeaf*> (obj) && obj->state == LEAF_STATE_FALLING
			|| dynamic_cast<CScore*>(obj) || dynamic_cast<CPiece*>(obj)
			|| dynamic_cast<CCard*>(obj))
			objectsRenderThird.push_back(obj);
	}

	for (size_t i = 0; i < objects.size(); i++)
		coObjects.push_back(objects[i]);

	//get map and screen information
	if (player->IsLostControl() || player->isPipe())
	{
		player->Update(0, &coObjects);
		hud->Update(0);
		cam->Update(0, isCameraAutoMove, cxcount);
	}
	else {
		player->Update(dt, &coObjects);
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(dt, &coObjects);
		}
		hud->Update(dt);
		cam->Update(dt, isCameraAutoMove, cxcount);
	}
	grid->UpdateGrid(units);
}

void CPlayScene::Render()
{
	if (player == NULL) return;
	if (isGameDone1)
		gamedone1->Draw(Camera::GetInstance()->GetCameraPosition().x + GAMEDONE_1_DIFF_X, Camera::GetInstance()->GetCameraPosition().y + GAMEDONE_1_DIFF_Y);
	if (isGameDone2)
		gamedone2->Draw(Camera::GetInstance()->GetCameraPosition().x + GAMEDONE_2_DIFF_X, Camera::GetInstance()->GetCameraPosition().y + GAMEDONE_2_DIFF_Y);
	current_map->Render();
	player->Render();
	//for (int i = 0; i < objects.size(); i++)
	//	objects[i]->Render();
	for (int i = 0; i < objectsRenderFirst.size(); i++)
		objectsRenderFirst[i]->Render();
	for (int i = 0; i < objectsRenderSecond.size(); i++)
		objectsRenderSecond[i]->Render();
	for (int i = 0; i < objectsRenderThird.size(); i++)
		objectsRenderThird[i]->Render();
	hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	if (grid != nullptr)
		grid->ClearAll();
	if (player != nullptr)
		delete player;

	objects.clear();
	units.clear();
	objectsRenderFirst.clear();
	objectsRenderSecond.clear();
	objectsRenderThird.clear();

	delete hud;
	delete fonts;

	current_map = nullptr;
	player = nullptr;
	grid = nullptr;
	hud = nullptr;
	Camera::GetInstance()->SetCameraPosition(0, 0);
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);

}

void CPlaySceneKeyHandler::OnKeyUp(int KeyCode) {
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	CMario* mario = ((CPlayScene*)scene)->GetPlayer();

	if (mario == NULL || mario->IsLostControl() || mario->IsDead() || mario->isPipe()) return;

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->isSitting = false;
		break;
	case DIK_UP:
		mario->isSitting = false;
	case DIK_A:
		mario->isReadyToRun = false;
		mario->isHold = false;
		mario->isReadyToHold = false;

	}

}

void CPlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scene)->GetPlayer();

	if (mario == NULL || mario->IsLostControl() || mario->IsDead() || mario->isPipe()) return;

	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->isGround)
			mario->SetState(MARIO_STATE_JUMPING);
		break;
		/*case DIK_DOWN:
			break;
		case DIK_UP:
			break;*/
	case DIK_C:
	{
		if (!mario->tailTimer.IsStarted() || !mario->isSitting)
			mario->Attack();
		break;
	}
	break;
	case DIK_1:
		mario->Transform(CMario::Mode::Small);
		break;
	case DIK_2:
		mario->Transform(CMario::Mode::Super);
		break;
	case DIK_3:
		mario->Transform(CMario::Mode::Fire);
		break;
	case DIK_4:
		mario->Transform(CMario::Mode::Tanooki);
		break;

	case DIK_T:
		mario->TelePort();
		break;
	}
}

void CPlaySceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scene)->GetPlayer();
	//DebugOut(L"[INFO] KeyDown: %d\n", mario->GetState());

	//disable control key when Mario die 
	if (mario == NULL || mario->IsLostControl() || mario->IsDead() || mario->isPipe()) return;
	//check current key state
	if (game->IsKeyDown(DIK_DOWN))
		mario->SetState(MARIO_STATE_SITTING);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else
		mario->SetState(MARIO_STATE_IDLE);
	if (game->IsKeyDown(DIK_A))
	{
		mario->isReadyToHold = true;
		if (!mario->isHold)
			mario->isReadyToRun = true;
	}
}