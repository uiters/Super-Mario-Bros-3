#pragma once
#include "GameObject.h"
#include <algorithm>
#include "Define.h"
#include "Utils.h"

#define SWITCH_BBOX_WIDTH			16
#define SWITCH_BBOX_HEIGHT			16
#define SWITCH_BBOX_PRESSED_HEIGHT	7

#define SWITCH_ANI_SET_ID		77

#define SWITCH_ANI_IDLE		0
#define SWITCH_ANI_PRESSED	1


#define SWITCH_STATE_IDLE		0
#define SWITCH_STATE_UP			111
#define SWITCH_STATE_PRESSED	122

class CSwitch : public CGameObject
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