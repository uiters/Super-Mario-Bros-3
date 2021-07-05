#include "PlantBullet.h"
#include "FirePlant.h"
#include "Mario.h"
#include "PlayScene.h"
#include "CMap.h"
CPlantBullet::CPlantBullet(float bx, float by, bool Up, bool Right)
{
	if (Up)
	{
		y = by;
		vy = -BULLET_SPEED_Y;
	}
	else
	{
		y = by + BULLET_BBOX_HEIGHT;
		vy = BULLET_SPEED_Y;
	}


	if (Right)
	{
		x = bx + PIRANHAPLANT_BBOX_WIDTH;
		vx = BULLET_SPEED_X;
	}
	else
	{
		x = bx - BULLET_BBOX_WIDTH;
		vx = -BULLET_SPEED_X;
	}
	SetType(IGNORE);
}
void CPlantBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Camera* cam = Camera::GetInstance();
	if (!cam->isAreaCamera(x, y))
		isDestroyed = true;
	if (isDestroyed)
		return;
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	float mLeft, mTop, mRight, mBottom;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL)
	{
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)) && !mario->untouchableTimer.IsStarted())
		{
			mario->Attacked();
			isDestroyed = true;
		}
	}
	//Camera* cam = Camera::GetInstance();
	//bool iscam = cam->isAreaCamera(x, y);
	//if (!iscam) {
	//	isDestroyed = true;
	//}
}
void CPlantBullet::Render()
{
	int ani = 0;
	if (vx > 0)
		ani = BULLET_ANI_RIGHT;
	else
		ani = BULLET_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CPlantBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BULLET_BBOX_WIDTH;
	b = y + BULLET_BBOX_HEIGHT;
}