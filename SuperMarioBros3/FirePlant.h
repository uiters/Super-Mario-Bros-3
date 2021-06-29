#pragma once
#include "GameObject.h"
#include "Timer.h"
#define PIRANHAPLANT_GREEN_TYPE					0
#define PIRANHAPLANT_RED_TYPE					1

#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_GREEN_BBOX_HEIGHT			24
#define PIRANHAPLANT_RED_BBOX_HEIGHT			32
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_BBOX_DEATH_WIDTH			16
#define PIRANHAPLANT_BBOX_DEATH_HEIGHT			24
class CFirePlant : public CGameObject
{
	Timer bitingTimer;
	Timer shootTimer;
	Timer aimTimer;
	Timer dyingTimer;
	float limitY = 0; //start_y
	int BBHeight = 0;

public:
	CFirePlant(int tag = PIRANHAPLANT_GREEN_TYPE);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render() {};
	virtual void SetState(int state) {};
	void SetLimitY(float ly) {
		if (tag == PIRANHAPLANT_GREEN_TYPE)
			BBHeight = PIRANHAPLANT_GREEN_BBOX_HEIGHT;
		else
			BBHeight = PIRANHAPLANT_RED_BBOX_HEIGHT;
		limitY = ly - BBHeight;
	}
};

