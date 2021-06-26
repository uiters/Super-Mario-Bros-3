#include "Switch.h"
#include "BreakableBrick.h"
#include "Mario.h"
#include "PlayScene.h"

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDestroyed)
		return;
	CGameObject::Update(dt);
	if (state == SWITCH_STATE_UP)
	{
		y += dy;
		if (start_y - y >= SWITCH_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - SWITCH_BBOX_HEIGHT - 0.1f;
			SetState(SWITCH_STATE_IDLE);
		}
	}

}

void CSwitch::Render()
{
	if (!isAppear || isDestroyed)
		return;
	if (state == SWITCH_STATE_PRESSED)
		animation_set->at(SWITCH_ANI_PRESSED)->Render(x, y);
	else
		animation_set->at(SWITCH_ANI_IDLE)->Render(x, y);
	RenderBoundingBox();
}

void CSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SWITCH_BBOX_WIDTH;
	if (state == SWITCH_STATE_PRESSED)
		b = y + SWITCH_BBOX_PRESSED_HEIGHT;
	else
		b = y + SWITCH_BBOX_HEIGHT;
}

void CSwitch::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SWITCH_STATE_UP:
		vy = -0.05f;
		start_y = y;
		break;
	case SWITCH_STATE_PRESSED:
		//add handle later
		y += SWITCH_BBOX_HEIGHT - SWITCH_BBOX_PRESSED_HEIGHT;
		CPlayScene* swscene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT> objs = swscene->GetObjects();
		for (UINT i = 0; i < objs.size(); i++)
		{
			if (dynamic_cast<CBreakableBrick*>(objs[i]) && !objs[i]->isDestroyed)
			{
				objs[i]->isDestroyed = true;
				CCoin* item = new CCoin(COIN_TYPE_TRANSFORM);

				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET tmp_ani_set = animation_sets->Get(COIN_ANI_SET_ID);

				item->SetAnimationSet(tmp_ani_set);
				item->SetPosition(objs[i]->x, objs[i]->y);
				swscene->GetUnit()->AddUnit(item, swscene->GetGrid());
			}
		}
		break;
	}
}