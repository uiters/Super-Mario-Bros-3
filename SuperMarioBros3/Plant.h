#pragma once
#include "GameObject.h"
#include "Timer.h"

#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_BBOX_HEIGHT				24
#define PIRANHAPLANT_DARTING_SPEED				0.03f

#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_STATE_BITING				100
#define PIRANHAPLANT_STATE_DEATH				200
#define PIRANHAPLANT_STATE_INACTIVE				300

#define PIRANHAPLANT_ACTIVE_RANGE			20

#define PIRANHAPLANT_ANI_DARTING			0
#define PIRANHAPLANT_ANI_DEATH				1

#define PIRANHAPLANT_BITING_TIME		1000
#define PIRANHAPLANT_DIYING_TIME		250
class CPlant :public CGameObject
{
	Timer bitingTimer;
	Timer dyingTimer;
	float limitY = 0; //start_y
public:
	CPlant();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void SetLimitY(float ly) { limitY = ly - PIRANHAPLANT_BBOX_HEIGHT; }

};

