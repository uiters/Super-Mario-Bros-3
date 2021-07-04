#include "Plant.h"
#include "Brick.h"
#include "Utils.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Scene.h"
void CPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
}
void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (dyingTimer.ElapsedTime() >= PIRANHAPLANT_DIYING_TIME && dyingTimer.IsStarted())
		isDestroyed = true;
	if (state == PIRANHAPLANT_STATE_DEATH)
		return;
	CGameObject::Update(dt, coObjects);
	y += dy;

	if (y <= limitY && vy < 0)
	{
		y = limitY;
		SetState(PIRANHAPLANT_STATE_BITING);
	}
	if (y >= limitY + PIRANHAPLANT_BBOX_HEIGHT && vy > 0)
	{
		y = limitY + PIRANHAPLANT_BBOX_HEIGHT;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
	}
	if (bitingTimer.ElapsedTime() >= PIRANHAPLANT_BITING_TIME && bitingTimer.IsStarted())
	{
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
		bitingTimer.Reset();
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL)
	{
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		float l, r;
		mario->GetBoundingBox(l, mTop, r, mBottom);

		int mWidth = r - l;

		if ((floor(mario->x) + (float)mWidth + PIRANHAPLANT_ACTIVE_RANGE <= x
			|| ceil(mario->x) >= x + PIRANHAPLANT_BBOX_WIDTH + PIRANHAPLANT_ACTIVE_RANGE)
			&& state == PIRANHAPLANT_STATE_INACTIVE && !bitingTimer.IsStarted())
			SetState(PIRANHAPLANT_STATE_DARTING);

		if (mario->tailTimer.IsStarted() && mario->GetMode() == CMario::Mode::Tanooki)
		{
			mario->getTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom))
			{
				SetState(PIRANHAPLANT_STATE_DEATH);
				mario->AddScore(x, y, 100, true);

			}
		}
	}
}
void CPlant::Render()
{
	int ani = PIRANHAPLANT_ANI_DEATH;
	if (state != PIRANHAPLANT_STATE_DEATH && !dyingTimer.IsStarted())
		ani = PIRANHAPLANT_ANI_DARTING;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
CPlant::CPlant()
{
	SetState(PIRANHAPLANT_STATE_INACTIVE);
}
void CPlant::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		SetType(MOVING);
		break;
	case PIRANHAPLANT_STATE_BITING:
		vy = 0;
		bitingTimer.Start();
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		SetType(IGNORE);
		dyingTimer.Start();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		SetType(IGNORE);
		bitingTimer.Start();
		break;
	}
}

