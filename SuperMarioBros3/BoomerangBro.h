#pragma once
#include "GameObject.h"
#include "Boomerang.h"
#include "Timer.h"

#define BOOMERANG_BROTHER_BBOX_WIDTH    16
#define BOOMERANG_BROTHER_BBOX_HEIGHT   24

#define BOOMERANG_BROTHER_STATE_FORWARD		1
#define BOOMERANG_BROTHER_STATE_DIE			2

#define BOOMERANG_BROTHER_ANI_AIM_RIGHT		0
#define BOOMERANG_BROTHER_ANI_AIM_LEFT		1
#define BOOMERANG_BROTHER_ANI_THROW_RIGHT	2
#define BOOMERANG_BROTHER_ANI_THROW_LEFT	3

#define BOOMERANG_BROTHER_SPEED				0.025f
#define BOOMERANG_BROTHER_GRAVITY			0.001f
#define BOOMERANG_BROTHER_DEFLECT_SPEED		0.35f

#define BOOMERANG_BROTHER_LIMIT_RANGE		48
#define BOOMERANG_BROTHER_ACTIVE_RANGE		80


#define BOOMERANG_BROTHER_AIM_TIME			500
#define BOOMERANG_BROTHER_RELOAD_TIME		2000
#define BOOMERANG_BROTHER_THROW_TIME		500
#define BOOMERANG_BROTHER_CHANGE_TIME		3000

#define BOOMERANG_BROTHER_BOOMERANGS		2


class CBoomerangBrother :
	public CGameObject
{
	Timer aimTimer;
	Timer throwTimer;
	Timer reloadTimer;
	Timer chargeTimer;
	CBoomerang* boomerang = NULL;
	int currentBoomerang = 0;
public:
	float start_x = 0;
	CBoomerangBrother();
	void HoldBoomerang();
	void ThrowBoomerang();
	void DemolishBoomerang();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};