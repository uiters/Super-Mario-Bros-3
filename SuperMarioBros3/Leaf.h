#pragma once
#include "GameObject.h"
#include <algorithm>
#include "Define.h"
#include "Brick.h"
#include "Timer.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_UP_HEIGHT	 48

#define LEAF_ANI_SET_ID		36

#define LEAF_ANI_RIGHT		0
#define LEAF_ANI_LEFT		1

#define LEAF_GRAVITY		0.025f
#define LEAF_SPEED			0.075f

#define LEAF_STATE_IDLE		0
#define LEAF_STATE_UP		100
#define LEAF_STATE_FALLING	111

#define LEAF_FALLING_TIME	500

class CLeaf : public CGameObject
{
	float start_y;
	Timer appearTimer;
public:
	bool isAppear = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	void SetIsAppear(bool isAppearBool)
	{
		isAppear = isAppearBool;
	}
};

