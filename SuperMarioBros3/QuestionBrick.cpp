#include "QuestionBrick.h"
#include "PlayScene.h"
#include "MushRoom.h"
#include "Switch.h"
#include "Leaf.h"

CQuestionBrick::CQuestionBrick(int tag, int type)
{
	SetState(QUESTIONBRICK_STATE_IDLE);
	this->tag = tag;
	this->type = type;
}
void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	y += dy;
	if (state == QUESTIONBRICK_STATE_IDLE)
	{
		CMario* mario = {};
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		if (scene != NULL)
			mario = ((CPlayScene*)scene)->GetPlayer();
	}
}
void CQuestionBrick::SetState(int state = BRICK_STATE_IDLE)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = {};
	if (scene != NULL)
		mario = ((CPlayScene*)scene)->GetPlayer();
	switch (state)
	{
	case QUESTIONBRICK_STATE_HIT:
		if (this->state != QUESTIONBRICK_STATE_HIT && mario != nullptr)
		{
			if (items > 0)
				items--;
			if (tag == ITEM_COIN)
			{
				CreateItem(ITEM_COIN);
				CCoin* obj = dynamic_cast<CCoin*>(item);
				obj->isAppear = true;
				obj->SetPosition(x, y - COIN_BBOX_HEIGHT - 1);
				obj->SetState(COIN_STATE_UP);
				scene->GetUnit()->AddUnit(obj, scene->GetGrid());
			}
			/*	if (tag == ITEM_LEAF || (tag == ITEM_CUSTOM && mario->GetMode() == CMario::Mode::Super))
				{
					CreateItem(ITEM_LEAF);
					CLeaf* obj = dynamic_cast<CLeaf*>(item);
					obj->isAppear = true;
					obj->SetPosition(x, y);
					obj->SetState(LEAF_STATE_UP);
					scene->PushBack(item);
				}*/
			else if (tag == ITEM_CUSTOM)
			{
				if (mario->GetMode() == CMario::Mode::Small)
				{
					CreateItem(ITEM_MUSHROOM_RED);
					item->tag = ITEM_MUSHROOM_RED;
					CMushroom* obj = dynamic_cast<CMushroom*>(item);
					obj->isAppear = true;
					obj->SetPosition(x, y);
					obj->SetState(MUSHROOM_STATE_UP);
					scene->GetUnit()->AddUnit(obj, scene->GetGrid());
				}
				if (mario->GetMode() == CMario::Mode::Tanooki || mario->GetMode() == CMario::Mode::Fire)
				{
					CreateItem(ITEM_MUSHROOM_GREEN);
					item->tag = MUSHROOM_TYPE_GREEN;
					CMushroom* obj = dynamic_cast<CMushroom*>(item);
					obj->isAppear = true;
					obj->SetPosition(x, y);
					obj->SetState(MUSHROOM_STATE_UP);
					scene->GetUnit()->AddUnit(obj, scene->GetGrid());
				}
			}
		}
		break;
	}
	if (items == 0)
		CGameObject::SetState(state);
}
void CQuestionBrick::Render()
{
	int ani = -1;
	if (state == QUESTIONBRICK_STATE_HIT)
		ani = QUESTIONBRICK_ANI_HIT;
	else
		if (type == QUESTIONBRICK_TYPE_FLASH)
			ani = QUESTIONBRICK_ANI_FLASH;
		else
			ani = QUESTIONBRICK_ANI_IDLE;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CQuestionBrick::CreateItem(int itemtype)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET tmp_ani_set = NULL;

	if (itemtype == ITEM_COIN)
	{
		item = new CCoin(COIN_TYPE_INBRICK);
		item->SetType(IGNORE);
		tmp_ani_set = animation_sets->Get(COIN_ANI_SET_ID);
	}
	else if (itemtype == ITEM_MUSHROOM_RED || itemtype == ITEM_MUSHROOM_GREEN)
	{
		item = new CMushroom();
		item->SetType(IGNORE);
		tmp_ani_set = animation_sets->Get(MUSHROOM_ANI_SET_ID);
	}
	item->SetAnimationSet(tmp_ani_set);

}