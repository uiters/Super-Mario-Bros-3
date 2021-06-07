#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Game.h"
class CIntroScene : public CScene {
public:
	CIntroScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};
class IntroSceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};

	IntroSceneKeyHandler(CScene* s) :CSceneKeyHandler(s) {};
};

