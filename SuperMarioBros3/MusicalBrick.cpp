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
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = {};
	if (scene != NULL)
		mario = ((CPlayScene*)scene)->GetPlayer();
	if (boundTimer.ElapsedTime() > MUSICBRICK_BOUND_TIME && boundTimer.IsStarted() && state == MUSIC_BRICK_STATE_HIT_FROM_TOP && mario != nullptr)
	{
		boundTimer.Reset();
		vy += ay * dt;
	}
	if (boundTimer.ElapsedTime() > MUSICBRICK_BOUND_TIME && boundTimer.IsStarted() && state == MUSIC_BRICK_STATE_HIT_FROM_DOWN && mario != nullptr)
	{
		boundTimer.Reset();
		vy -= ay * dt;
	}
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
	GetBoundingBox(oLeft, oTop, oRight, oBottom);
	if (state == MUSIC_BRICK_STATE_IDLE && mario != NULL)
	{
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		if (mario->tailTimer.IsStarted() && mario->GetMode() == CMario::Mode::Tanooki)
		{
			mario->getTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->getTail()->hit_times == 0)
			{
				SetState(MUSIC_BRICK_STATE_HIT_FROM_TOP);
			}
		}
	}
	if (y > start_y + 5)
	{
		y = start_y;
		SetState(MUSIC_BRICK_STATE_IDLE);
	}
	else if (y < start_y - 5)
	{
		y = start_y;
		SetState(MUSIC_BRICK_STATE_IDLE);
	}
}
void CMusicalBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSIC_BRICK_STATE_IDLE:
		ay = vy = 0;
		boundTimer.Reset();
		break;
	case MUSIC_BRICK_STATE_HIT_FROM_TOP:
		vy = MUSIC_BRICK_SPEED;
		ay = MUSIC_BRICK_SPEED_UP;
		boundTimer.Start();
		break;
	case MUSIC_BRICK_STATE_HIT_FROM_DOWN:
		vy = -MUSIC_BRICK_SPEED;
		ay = -MUSIC_BRICK_SPEED_UP;
		boundTimer.Start();
		break;
	}
}