#include "MusicalBrick.h"
#include "PlayScene.h"
#include "Mario.h"

CMusicalBrick::CMusicalBrick() {
	start_y = y;
	start_x = x;
	SetState(MUSIC_BRICK_STATE_IDLE);

}

void CMusicalBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MUSIC_BRICK_BBOX_WIDTH;
	bottom = y + MUSIC_BRICK_BBOX_WIDTH;
}

void CMusicalBrick::Render() {
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CMusicalBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	y += dy;

	if (state == MUSIC_BRICK_STATE_HIT_FROM_TOP)
	{
		y += 3;
		if (isDown && !isUp)
		{
			isDown = false;
			isUp = true;
			//DebugOut(L"condition1::");
			
		}
		else
			if (y >= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
				//DebugOut(L"condition2::");
			}
	}
	if (state == MUSIC_BRICK_STATE_HIT_FROM_DOWN)
	{
		y -= 3;
		if (isUp && !isDown)
		{
			isUp = false;
			isDown = true;

			//DebugOut(L"condition3::");
		}
		else
			if (y <= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
				//DebugOut(L"condition4::");
			}

	}

}
void CMusicalBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSIC_BRICK_STATE_IDLE:
		ay = vy = 0;
		isDown = false;
		isUp = false;
		boundTimer.Reset();
		break;
	case MUSIC_BRICK_STATE_HIT_FROM_TOP:
		vy = MUSIC_BRICK_SPEED;
		ay = MUSIC_BRICK_SPEED_UP;
		break;
	case MUSIC_BRICK_STATE_HIT_FROM_DOWN:
		vy = -MUSIC_BRICK_SPEED;
		ay = -MUSIC_BRICK_SPEED_UP;
		break;
	}
}