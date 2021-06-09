#pragma once
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
#include "Scene.h"
#include "Utils.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Camera.h"
#include "CMap.h"
#include "Portal.h"

#define GAMEDONE1_SPRITE_ID		50070
#define GAMEDONE2_SPRITE_ID		50071


#define WORLD_1			0
#define WORLD_INTRO		6
#define WORLD_1_1		1
#define WORLD_1_4		4

#define CAM_CHANGE_TIME		30

class CPlayScene: public CScene
{
protected: 
	CMario *player = NULL;					// A play scene has to have player, right? 
	CMap* current_map = NULL;
	vector<LPGAMEOBJECT> objects;
	//Camera
	Camera* cam ;
	float cxcount = 0;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);
	void ParseObjFromFile(LPCWSTR path);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; } 
	CMap* GetMap() { return current_map; }
	//friend class CPlaySceneKeyHandler;
};

class CPlaySceneKeyHandler : public CSceneKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlaySceneKeyHandler(CScene *s) :CSceneKeyHandler(s) {};
};

