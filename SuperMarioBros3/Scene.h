#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"

class CScene
{
protected:
	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;

public: 
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
};
typedef CScene * LPSCENE;


class CSceneKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scene; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CSceneKeyHandler(CScene *s) :CKeyEventHandler() { scene = s; }
};