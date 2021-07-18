#include "TinyGoomba.h"
#include "Mario.h"
#include "PlayScene.h"

void CTinyGoomba::Render() {
	animation_set->at(POOP_GOOMBA_ANI)->Render(x, y);
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
			isCollide = true;
		}

		if (isCollide) {
			/*mario->isTakingPoopGoomba = true;*/
			//this->x = mario->x;
			DebugOut(L"[ma->y]::%f\n", mario->y);
			DebugOut(L"[ma->bt]::%f\n", mBottom);
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