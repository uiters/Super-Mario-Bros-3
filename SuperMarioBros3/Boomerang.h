#pragma once
#include "GameObject.h"
#define BOOMERANG_BBOX_WIDTH    10
#define BOOMERANG_BBOX_HEIGHT   10

#define BOOMERANG_STATE_IDLE	0
#define BOOMERANG_STATE_1		1
#define BOOMERANG_STATE_2		2
#define BOOMERANG_STATE_3		3
#define BOOMERANG_STATE_4		4
#define BOOMERANG_STATE_5		5
#define BOOMERANG_STATE_6		6

#define BOOMERANG_ANI_ID				90
#define BOOMERANG_ANI_IDLE_RIGHT		0
#define BOOMERANG_ANI_RIGHT				1
#define BOOMERANG_ANI_IDLE_LEFT			2
#define BOOMERANG_ANI_LEFT				3


#define BOOMERANG_SPEED_X				0.1f
#define BOOMERANG_SPEED_Y      			0.05f
#define BOOMERANG_GRAVITY		      	0.001f
#define BOOMERANG_ACCELERATION	      	0.001f
#define BOOMERANG_CHANGE_STATE_TIME		    750

#define BOOMERANG_DIFF			3

#define BOOMERANG_SPEED_DIFF	1.25f

class CBoomerang :
	public CGameObject
{
	DWORD state_start = 0;
public:
	float start_x = 0;
	bool isAppear = false;
	CBoomerang(float x, float y);
	void StartChangState() { state_start = 1; }
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

