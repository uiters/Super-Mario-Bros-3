#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"

#define	INTROSCENE	0
#define	WORLDSCENE	1
#define	PLAYSCENE	2

#define SCENE_SECTION_UNKNOWN		   -1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TILEMAP_DATA		7

#define MAX_SCENE_LINE					1024

class CScene
{
protected:
	CKeyEventHandler* key_handler;
	int id;
	bool isCameraAutoMove = false;
	LPCWSTR sceneFilePath;

public:
	CScene(int id, LPCWSTR filePath);
	bool isUnLoaded = false;
	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	int GetId() { return id; }
	void SetCamerAutoMove(bool l) { this->isCameraAutoMove = l; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};
typedef CScene* LPSCENE;


class CSceneKeyHandler : public CKeyEventHandler
{
protected:
	CScene* scene;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CSceneKeyHandler(CScene* s) :CKeyEventHandler() { scene = s; }
};