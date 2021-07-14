#pragma once
#include "GameObject.h"
#include "Timer.h"

#define MUSIC_BRICK_ANI_SET 100

#define MUSIC_BRICK_BBOX_WIDTH 15

#define MUSIC_BRICK_SPEED 0.05f
#define MUSIC_BRICK_SPEED_UP 0.0001f
#define MUSIC_PUSH_MAX_HEIGHT	10
#define MUSICBRICK_BOUND_TIME 10

#define MUSIC_BRICK_STATE_IDLE 0
#define MUSIC_BRICK_STATE_HIT_FROM_TOP 3
#define MUSIC_BRICK_STATE_HIT_FROM_DOWN 4

#define MUSIC_BRICK_STATE_UP 1
#define MUSIC_BRICK_STATE_DOWN 2

class CMusicalBrick :public CGameObject {
	Timer boundTimer;

public:
	float start_y = 0;
	float start_x = 0;
	CMusicalBrick();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};