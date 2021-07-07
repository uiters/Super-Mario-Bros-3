#pragma once
#include "GameObject.h"
#include <algorithm>
#include "Define.h"
#include "Brick.h"

#define FIRE_BULLET_BBOX_WIDTH  9
#define FIRE_BULLET_BBOX_HEIGHT 9

#define FIRE_BULLET_SPEED_X	0.2f
#define FIRE_BULLET_SPEED_Y	0.1f
#define FIRE_BULLET_LIMITED_HEIGHT  23

#define FIRE_BULLET_STATE_FLYING		100
#define FIRE_BULLET_STATE_HIDDEN		200
#define FIRE_BULLET_SHOOT_DIFF			0.6f

#define FIRE_BULLET_ANI_ID	9

#define FIRE_BULLET_ANI_RIGHT		0
#define FIRE_BULLET_ANI_LEFT		1

class CFireBullet : public CGameObject
{
public:
	bool isBeingUsed = false;
	float tempHeight = -1;
	void SetTemHeight(float ty) { this->tempHeight = ty; };

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetIsBeingUsed(bool m) { isBeingUsed = m; };
	void CFireBullet::FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx, float& min_ty,
		float& nx, float& ny, float& rdx, float& rdy);
	CFireBullet(float x = -1, float y = -1);
};
