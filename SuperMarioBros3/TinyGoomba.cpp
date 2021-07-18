#include "TinyGoomba.h"
#include "Mario.h"
#include "PlayScene.h"

void CTinyGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + POOP_GOOMBA_BBOX_WIDTH;
	bottom = y + POOP_GOOMBA_BBOX_HEIGHT;
}

void CTinyGoomba::Render() {
	animation_set->at(9)->Render(x, y);
	RenderBoundingBox(100);
}

void CTinyGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGameObject::Update(dt);

	x += dx;
	y += dy;

	vy = ay * dt;
	vx = ax * dt;

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	if (mario != NULL) {
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(mLeft, mTop, mRight, mBottom)) {
			if (x <= mario->x) {
				x = mario->x;
				ax = 0.002f;
			}
			else if (x >= mario->x + MARIO_BIG_BBOX_WIDTH - POOP_GOOMBA_BBOX_WIDTH) {
				ax = -0.002f;
				x = mario->x + MARIO_BIG_BBOX_WIDTH - POOP_GOOMBA_BBOX_WIDTH;
			}
			if (y >= mario->y + MARIO_BIG_BBOX_HEIGHT - POOP_GOOMBA_BBOX_HEIGHT) {
				ay = -0.003f;
				y = mario->y + MARIO_BIG_BBOX_HEIGHT - POOP_GOOMBA_BBOX_HEIGHT;
			}
			if (y <= mario->y) {
				ay = 0.003f;
				y = mario->y;
			}
		}
	}
}