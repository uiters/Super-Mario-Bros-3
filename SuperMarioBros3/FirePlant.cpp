#include "FirePlant.h"
#include "Brick.h"
#include "Utils.h"
#include "PlantBullet.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Scene.h"
CFirePlant::CFirePlant(int tag)
{
	this->tag = tag;
	SetState(PIRANHAPLANT_STATE_INACTIVE);
}
void CFirePlant::GetDirect() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int mHeight;
	if (mario->GetMode() == CMario::Mode::Small)
		mHeight = MARIO_SMALL_BBOX_HEIGHT;
	else
		mHeight = MARIO_BIG_BBOX_HEIGHT;

	if (mario->y + mHeight < limitY + BBHeight)
		Up = true;
	else
		Up = false;
	if (mario->x <= x)
		Right = false;
	else
		Right = true;
};
void CFirePlant::Shoot()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET tmp_ani_set = animation_sets->Get(BULLET_ANI_SET_ID);
	CPlantBullet* bullet = new CPlantBullet(x, y, Up, Right);
	bullet->SetAnimationSet(tmp_ani_set);
	scene->GetUnit()->AddUnit(bullet, scene->GetGrid());
}
void CFirePlant::Update(DWORD dt,
	vector<LPGAMEOBJECT>* coObjects)
{
	if (dyingTimer.ElapsedTime() >= PIRANHAPLANT_DIYING_TIME && dyingTimer.IsStarted())
		isDestroyed = true;
	if (state == PIRANHAPLANT_STATE_DEATH)
		return;
	CGameObject::Update(dt, coObjects);
	y += dy;
	GetDirect();
	if (y <= limitY && vy < 0)
	{
		y = limitY;
		vy = 0;
		aimTimer.Start();
	}
	if (y >= limitY + BBHeight && vy > 0)
	{
		y = limitY + BBHeight;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
		delayTimer.Start();
	}
	if (aimTimer.ElapsedTime() >= PIRANHAPLANT_AIM_TIME && aimTimer.IsStarted())
	{
		aimTimer.Reset();
		SetState(PIRANHAPLANT_STATE_SHOOTING);

		delayTimer.Start();
	}
	if (delayTimer.ElapsedTime() >= PIRANHAPLANT_DELAY_TIME && delayTimer.IsStarted())
	{
		delayTimer.Reset();
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
	}

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
			&& state == PIRANHAPLANT_STATE_INACTIVE && !delayTimer.IsStarted())
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
void CFirePlant::Render()
{
	int ani = PIRANHAPLANT_ANI_DEATH;
	if (state != PIRANHAPLANT_STATE_DEATH && !dyingTimer.IsStarted())
	{
		if (Up)
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_UP;
			else
				ani = PIRANHAPLANT_ANI_LEFT_UP;
		else
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_DOWN;
			else
				ani = PIRANHAPLANT_ANI_LEFT_DOWN;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox(0);
}
void CFirePlant::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		SetType(MOVING);
		break;
	case PIRANHAPLANT_STATE_SHOOTING:
		vy = 0;
		Shoot();
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		SetType(IGNORE);
		dyingTimer.Start();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		SetType(IGNORE);
		break;
	}
}
void CFirePlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + BBHeight;
}
