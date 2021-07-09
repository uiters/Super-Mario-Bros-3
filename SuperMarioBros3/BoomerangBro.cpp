#include "BoomerangBro.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScene.h"
CBoomerangBrother::CBoomerangBrother()
{
	SetState(BOOMERANG_BROTHER_STATE_FORWARD);
	nx = 1;

}
void CBoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOOMERANG_BROTHER_BBOX_WIDTH;
	bottom = top + BOOMERANG_BROTHER_BBOX_HEIGHT;
}
void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += BOOMERANG_BROTHER_GRAVITY * dt;
	if (x < start_x)
		vx = BOOMERANG_BROTHER_SPEED;

	if (x > start_x + BOOMERANG_BROTHER_LIMIT_RANGE)
		vx = -BOOMERANG_BROTHER_SPEED;


	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != BOOMERANG_BROTHER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty;
		float nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		float x0 = x, y0 = y;
		x = x0 + min_tx * dx + nx * PUSHBACK;
		y = y0 + min_ty * dy + ny * PUSHBACK;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny < 0)
					vy = 0;
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
						vx = -vx;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}
void CBoomerangBrother::Render()
{
	int ani = 0;
	if (nx > 0)
	{
		if (state == BOOMERANG_STATE_IDLE)
			ani = BOOMERANG_BROTHER_ANI_AIM_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_THROW_RIGHT;
	}
	else
	{
		if (state == BOOMERANG_STATE_IDLE)
			ani = BOOMERANG_BROTHER_ANI_AIM_LEFT;
		else
			ani = BOOMERANG_BROTHER_ANI_THROW_LEFT;
		//ani = BOOMERANG_BROTHER_ANI_THROW_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CBoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_BROTHER_STATE_FORWARD:
		vx = BOOMERANG_BROTHER_SPEED;
		break;
	case BOOMERANG_BROTHER_STATE_DIE:
		vy = -BOOMERANG_BROTHER_DEFLECT_SPEED;
		type = IGNORE;
		break;
	}
}