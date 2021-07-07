#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "WorldPlayer.h"
#include "GameObject.h"
#include "CMap.h"
#include "Font.h"
#include "HUD.h"
#include "Camera.h"

#define OBJECT_TYPE_PLAYER 0
#define OBJECT_TYPE_STOP 2
#define OBJECT_TYPE_PORTAL 3
class CWorldScene : public CScene
{
protected:
	CWorldPlayer* player = NULL;					// A play scene has to have player, right? 
	CMap* current_map = NULL;
	HUD* hud = NULL;
	vector<LPGAMEOBJECT> objects;
	Camera* cam;
	float cxcount;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);


public:
	CWorldScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CWorldPlayer* GetPlayer() { return player; }
	CMap* GetMap() { return current_map; }
	HUD* GetHUD() { return hud; }
	//friend class CWorldSceneKeyHandler;
};

class CWorldSceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CWorldSceneKeyHandler(CScene* s) :CSceneKeyHandler(s) {};
};