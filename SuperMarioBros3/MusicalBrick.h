#pragma once
#include "GameObject.h"

#define MUSIC_BRICK_ANI_SET 100

#define MUSIC_BRICK_BBOX_WIDTH 16

#define MUSIC_BRICK_SPEED 0.05f
#define MUSIC_BRICK_SPEED_UP 0.0001f
#define MUSIC_PUSH_MAX_HEIGHT	10

#define MUSIC_BRICK_STATE_IDLE 0
#define MUSIC_BRICK_STATE_UP 1
#define MUSIC_BRICK_STATE_DOWN 2

class CMusicalBrick :public CGameObject {
	bool isGoUp = false;
	bool isPushedDown = false;
	bool isPushedUp = false;
	bool isGoDown = false;
	float start_y = 0;
	float start_x = 0;

	void Reset() { isGoUp = false; isPushedDown = false; isPushedUp = false; isGoDown = false; }
public:
	CMusicalBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

	void SetIsPushedUp(bool isPushedUp) { this->isPushedUp = isPushedUp; }
	void SetIsPushedDown(bool isPushedDown) { this->isPushedDown = isPushedDown; }

	void StartPushedDown() { isPushedDown = true; isGoUp = false; }
	void StopPushedDown() { isPushedDown = false; isGoUp = true; }

	void StartPushedUp() { isPushedUp = true; isGoDown = false; }
	void StopPushedUp() { isPushedUp = false; isGoDown = true; }

	bool GetIsGoUp() { return this->isGoUp; }
};