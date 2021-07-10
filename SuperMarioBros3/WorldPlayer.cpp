#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "BackUp.h"

#include "WorldPlayer.h"
#include "WorldScene.h"
#include "WorldMapObject.h"

CWorldPlayer::CWorldPlayer(float x, float y) : CGameObject()
{
	mode = CBackUp::GetInstance()->mode;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	//if scene 1 thì di chuyển xuống đc
	SetMove(false, false, true, false);
}
void CWorldPlayer::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->tag == OBJECT_TYPE_BUSH || coObjects->at(i)->tag == OBJECT_TYPE_HAMMER)
			continue;
		else
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CWorldPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	CGame* game = CGame::GetInstance();
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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		float x0 = x, y0 = y;
		x = x0 + min_tx * dx + nx * PUSHBACK;
		y = y0 + min_ty * dy + ny * PUSHBACK;
		//
		// Collision logic with other objects
		//
		float oLeft, oTop, oRight, oBottom;
		float mLeft, mTop, mRight, mBottom;
		SetState(PLAYER_STATE_IDLE);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			x = e->obj->x;
			y = e->obj->y;
			if (e->obj->tag == OBJECT_TYPE_PORTAL || e->obj->tag == OBJECT_TYPE_STOP)
			{
				CWorldMapObject* tmp = dynamic_cast<CWorldMapObject*>(e->obj);
				bool cl, cr, cu, cd;
				tmp->GetMove(cl, cu, cr, cd);
				SetMove(cl, cu, cr, cd);
				if (e->obj->tag == OBJECT_TYPE_PORTAL)
					sceneId = tmp->GetSceneId();
				else
					sceneId = -1;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		//coEvents[i]->obj->SetDebugAlpha(coEvents[i]->obj->DebugAlpha - 50);
		delete coEvents[i];
	}
	//DebugOut(L"[MARIO] l: %d\tt: %d r: %d\tb: %d\n", cgLeft,cgUp, cgRight,cgDown);
}
void CWorldPlayer::Render()
{
	animation_set->at(int(mode))->Render(x, y);
	//DebugAlpha = 128;
	RenderBoundingBox();
}
void CWorldPlayer::SetState(int state)
{
	SetMove(false, false, false, false);
	switch (state)
	{
	case PLAYER_STATE_IDLE:
		vx = vy = 0;
		break;
	case PLAYER_STATE_RIGHT:
		vx = PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_LEFT:
		vx = -PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_UP:
		vx = 0;
		vy = -PLAYER_SPEED;
		break;
	case PLAYER_STATE_DOWN:
		vx = 0;
		vy = PLAYER_SPEED;
		break;
	}
	CGameObject::SetState(state);
}

