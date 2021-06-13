#pragma once

#include "GameObject.h"

#define TOPLAYSCENE			0
#define TOEXTRASCENE		1
#define BACKTOPLAYSCENE		2

#define PORTAL_BBOX_WIDTH	28
#define PORTAL_BBOX_HEIGHT	1

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 
public:
	float start_x, start_y;
	bool pipeUp = false;
	CPortal(int scene_id, float start_x = 0, float start_y = 0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
};