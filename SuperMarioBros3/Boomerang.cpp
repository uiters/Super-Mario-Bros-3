#include "Boomerang.h"
#include "Mario.h"
#include "PlayScene.h"
CBoomerang::CBoomerang(float x, float y)
{
	this->x = x;
	this->y = y;
	nx = 1;
	SetState(BOOMERANG_STATE_IDLE);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET tmp_ani_set = animation_sets->Get(BOOMERANG_ANI_ID);
	SetAnimationSet(tmp_ani_set);
	type = IGNORE;
	//DebugOut(L"CBoomerang construcor::\n");

}
void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOOMERANG_BBOX_WIDTH;
	bottom = top + BOOMERANG_BBOX_HEIGHT;
}
void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isAppear)
		return;
	CGameObject::Update(dt);
	//update speed
	x += dx;
	y += dy;
	if (state_start != 0)
		state_start += dt;
	if (state == 1 && state_start >= BOOMERANG_CHANGE_STATE_TIME)
	{
		state_start = 1;
		SetState(state + 1);
	}
	if (state > BOOMERANG_STATE_1 && state <= BOOMERANG_STATE_6 && state_start >= BOOMERANG_CHANGE_STATE_TIME / BOOMERANG_DIFF)
	{
		state_start = 1;
		SetState(state + 1);
	}
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL && state != BOOMERANG_STATE_IDLE)
	{
		if (!mario->untouchableTimer.IsStarted())
		{
			mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)))
			{
				mario->Attacked();
				//DebugOut(L"Boomerang HIT\n");
			}
		}

	}
}
void CBoomerang::Render()
{
	if (!isAppear)
		return;
	int ani = 0;
	if (nx > 0)
	{
		if (state == BOOMERANG_STATE_IDLE)
			ani = BOOMERANG_ANI_IDLE_RIGHT;
		else
			ani = BOOMERANG_ANI_RIGHT;
	}
	else
	{
		if (state == BOOMERANG_STATE_IDLE)
			ani = BOOMERANG_ANI_IDLE_LEFT;
		else
			ani = BOOMERANG_ANI_LEFT;
	}
	animation_set->at(0)->Render(x- BOOMERANG_DIFF, y - BOOMERANG_DIFF);
	RenderBoundingBox();
}
void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_STATE_IDLE:
		vx = vy = 0;
		break;
	case BOOMERANG_STATE_1:
		vx = nx * BOOMERANG_SPEED_X;
		vy = -BOOMERANG_SPEED_Y;
		StartChangState();
		break;
	case BOOMERANG_STATE_2:
		vy = 0;
		break;
	case BOOMERANG_STATE_3:
		vy = BOOMERANG_SPEED_Y;
		break;
	case BOOMERANG_STATE_4:
		vx = 0;
		vy = BOOMERANG_SPEED_Y * BOOMERANG_SPEED_DIFF;
		break;
	case BOOMERANG_STATE_5:
		vy = BOOMERANG_SPEED_Y;
		vx = -nx * BOOMERANG_SPEED_X;
		break;
	case BOOMERANG_STATE_6:
		vx = -nx * BOOMERANG_SPEED_X;
		vy = 0;
		break;
	}
}