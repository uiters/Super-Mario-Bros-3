#include "Fire.h"
#include "Utils.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Plant.h"
#include "FirePlant.h"

CFireBullet::CFireBullet(float x, float y) : CGameObject()
{
	vx = vy = 0;
	this->x = x;
	this->y = y;
	animation_set = CAnimationSets::GetInstance()->Get(FIRE_BULLET_ANI_ID);
	type = IGNORE;
}

void CFireBullet::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0;
	ny = 0;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; min_ix = i; rdx = c->dx;
			if (ny == 0)
				nx = c->nx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; min_iy = i; rdy = c->dy;
			if (nx == 0)
				ny = c->ny;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Camera* cam = Camera::GetInstance();
	CGameObject::Update(dt);
	if (!cam->isAreaCamera(x, y))
	{
		isDestroyed = true;
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->ShootTimes--;
		return;
	}
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty;
		float nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		float x0, y0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj != NULL)
				if (e->obj->isDestroyed == true)
					continue;
			if (dynamic_cast<CBrick*>(e->obj))
			{
				x0 = x;
				y0 = y;
				x += min_ty * dy + ny * PUSHBACK;
				y += min_tx * dx + nx * PUSHBACK;
				CBrick* obj = dynamic_cast<CBrick*>(e->obj);

				if (ny != 0)
				{
					tempHeight = y;
					vy = -FIRE_BULLET_SPEED_Y;
					y = y0;
				}

				if (nx != 0)
				{
					isDestroyed = true;
					mario->ShootTimes--;
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				mario->AddScore(x, y, 100, true);
				e->obj->SetState(GOOMBA_STATE_DIE_BY_MARIO);
				isDestroyed = true;
				mario->ShootTimes--;
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				mario->AddScore(x, y, 100, true);
				if (e->obj->tag == KOOPAS_GREEN_PARA)
				{
					e->obj->tag = KOOPAS_GREEN;
					e->obj->SetState(KOOPAS_STATE_WALKING);
				}
				else {

					e->obj->SetState(KOOPAS_STATE_DEATH);
					isDestroyed = true;
				}
				mario->ShootTimes--;
			}
			else if (dynamic_cast<CPlant*>(e->obj) || dynamic_cast<CFirePlant*>(e->obj))
			{
				mario->AddScore(x, y, 100, true);
				e->obj->SetState(PIRANHAPLANT_STATE_DEATH);
				isDestroyed = true;
				mario->ShootTimes--;
			}
			else
			{
				x += dx;
				y += dy;
			}
		}
		//DebugOut(L"%f %f\n", tempHeight - FIRE_BULLET_LIMITED_HEIGHT, y);
	}
	if (y <= tempHeight - FIRE_BULLET_LIMITED_HEIGHT)
	{
		y = tempHeight - FIRE_BULLET_LIMITED_HEIGHT;
		vy = FIRE_BULLET_SPEED_Y;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CFireBullet::Render()
{
	int ani = 0;
	if (vx > 0)
	{
		ani = FIRE_BULLET_ANI_RIGHT;
	}
	else
		ani = FIRE_BULLET_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox(100);
}

void CFireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIRE_BULLET_BBOX_WIDTH;
	b = y + FIRE_BULLET_BBOX_HEIGHT;
}