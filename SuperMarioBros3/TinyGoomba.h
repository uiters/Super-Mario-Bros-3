#pragma once
#include "GameObject.h"

#define POOP_GOOMBA_BBOX_WIDTH	8
#define POOP_GOOMBA_BBOX_HEIGHT	9

#define POOP_GOOMBA_ANI	9

#define POOP_GOOMBA_STATE_DROP	0
#define POOP_GOOMBA_STATE_FLY_AROUND	1

#define POOP_GOOMBA_GRAVITY	0.005f

class CTinyGoomba : public CGameObject {
public:
	CTinyGoomba() {
		type = IGNORE_DEFINE;
		ay = POOP_GOOMBA_GRAVITY;
	}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};