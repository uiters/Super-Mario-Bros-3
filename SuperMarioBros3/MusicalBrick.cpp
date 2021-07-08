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

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	CMario* mario = currentScene->GetPlayer();

	y += dy;
	//DebugOut(L"y - start_y = %f\n", y - start_y);
	if (state == MUSIC_BRICK_STATE_DOWN) {
		if (isPushedDown) {
			if (y - start_y >= MUSIC_PUSH_MAX_HEIGHT) {
				StopPushedDown();
				SetState(MUSIC_BRICK_STATE_UP);
			}
		}
		if (isGoDown) {
			if (y >= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
			}
		}
	}

	if (state == MUSIC_BRICK_STATE_UP) {
		if (isPushedUp) {
			if (start_y - y >= MUSIC_PUSH_MAX_HEIGHT) {
				StopPushedUp();
				SetState(MUSIC_BRICK_STATE_DOWN);
			}
		}
		if (isGoUp) {
			if (y <= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
			}
		}
	}

	float oLeft, oTop, oRight, oBottom;
	float mLeft, mTop, mRight, mBottom;
	if (mario != NULL)
	{
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);

		if (isColliding(mLeft, mTop, mRight, mBottom)) {
			//DebugOut(L"Mario->vy::%f\n", mario->vy);
			if (state == MUSIC_BRICK_STATE_UP) {
				mario->SetIsJumpOnMusicBrick(true);
				mario->vy = -0.5f;
			}
			if (mario->state == MARIO_STATE_JUMPING) {
				DebugOut(L"MARIO_JUMP\n");
				mario->vy = -0.6f;
			}
		}
		else {
			mario->SetIsJumpOnMusicBrick(false);
		}


	}
}
void CMusicalBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSIC_BRICK_STATE_IDLE:
		vx = vy = 0;
		Reset();
		break;
	case MUSIC_BRICK_STATE_DOWN:
		vy = MUSIC_BRICK_SPEED;
		break;
	case MUSIC_BRICK_STATE_UP:
		vy = -MUSIC_BRICK_SPEED;
		break;
	}
}