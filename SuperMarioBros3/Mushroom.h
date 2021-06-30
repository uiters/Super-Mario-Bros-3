#pragma once
#include "GameObject.h"
#include <algorithm>
#include "Define.h"
#include "Brick.h"

#define MUSHROOM_BBOX_WIDTH  16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_ANI_SET_ID		37

#define MUSHROOM_ANI_RED_ID		0
#define MUSHROOM_ANI_GREEN_ID	1

#define MUSHROOM_GRAVITY		0.2f
#define MUSHROOM_SPEED			0.07f

#define MUSHROOM_STATE_IDLE		0
#define MUSHROOM_STATE_UP		100
#define MUSHROOM_STATE_WALK		111

#define MUSHROOM_TYPE_RED		3
#define MUSHROOM_TYPE_GREEN		5

class CMushroom : public CGameObject
{
	float start_y;
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

