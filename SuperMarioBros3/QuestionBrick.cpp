#include "QuestionBrick.h"
#include "PlayScene.h"
#include "MushRoom.h"
#include "Switch.h"
#include "Leaf.h"

CQuestionBrick::CQuestionBrick(int tag, int type, int repeat)
{
	start_y = y;
	SetState(QUESTIONBRICK_STATE_IDLE);
	this->tag = tag;
	this->type = type;
	repeating = repeat;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = {};
	if (scene != NULL)
		mario = ((CPlayScene*)scene)->GetPlayer();
	y += dy;
	//use tail open box
	if (state == QUESTIONBRICK_STATE_IDLE && mario != NULL)
	{
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		if (mario->tailTimer.IsStarted() && mario->GetMode() == CMario::Mode::Tanooki)
		{
			mario->getTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->getTail()->hit_times == 0)
			{
				SetState(QUESTIONBRICK_STATE_HIT);
				mario->getTail()->hit_times = 1;
			}
		}
	}

	if (boundTimer.ElapsedTime() > QUESTIONBRICK_BOUND_TIME && boundTimer.IsStarted() && state == QUESTIONBRICK_STATE_HIT && mario != nullptr)
	{
		boundTimer.Reset();
		vy += ay * dt;

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
		if (tag == ITEM_LEAF || (tag == ITEM_CUSTOM && mario->GetMode() == CMario::Mode::Super))
		{
			CreateItem(ITEM_LEAF);
			CLeaf* obj = dynamic_cast<CLeaf*>(item);
			obj->isAppear = true;
			obj->SetPosition(x, y);
			obj->SetState(LEAF_STATE_UP);
			scene->GetUnit()->AddUnit(obj, scene->GetGrid());
		}
		if (tag == ITEM_CUSTOM)
		{
			if (mario->GetMode() == CMario::Mode::Small)
			{
				CreateItem(ITEM_MUSHROOM_RED);
				item->tag = MUSHROOM_TYPE_RED;

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
		if (tag == ITEM_COIN_x5 && type == QUESTIONBRICK_TYPE_FLASH && repeating >= 0)
		{
			CreateItem(ITEM_COIN);
			CCoin* obj = dynamic_cast<CCoin*>(item);
			obj->isAppear = true;
			obj->SetPosition(x, y - COIN_BBOX_HEIGHT - 1);
			obj->SetState(COIN_STATE_UP);
			scene->GetUnit()->AddUnit(obj, scene->GetGrid());
		}
		if (tag == ITEM_SWITCH)
		{
			CreateItem(ITEM_SWITCH);
			CSwitch* obj = dynamic_cast<CSwitch*>(item);
			obj->isAppear = true;
			obj->SetPosition(x, y);
			obj->SetState(SWITCH_STATE_UP);
			scene->GetUnit()->AddUnit(obj, scene->GetGrid());
		}
	}
	//bounce
	if (tag == ITEM_COIN_x5 && type == QUESTIONBRICK_TYPE_FLASH && y < start_y - 7)
	{
		SetState(QUESTBRICK_STATE_FLASH);
		y = start_y;
		if (repeating >= 0)
			SetState(QUESTBRICK_STATE_FLASH_IDLE);
		else SetState(QUESTIONBRICK_STATE_EMPTY);
	}
	else if (y < start_y - 7)
	{
		SetState(QUESTIONBRICK_STATE_EMPTY);
		y = start_y;
	}
}

void CQuestionBrick::SetState(int state = BRICK_STATE_IDLE)
{
	switch (state)
	{
	case QUESTIONBRICK_STATE_HIT:
		boundTimer.Start();
		ay = -QUESTIONBRICK_SPEED;
		break;
	case QUESTIONBRICK_STATE_EMPTY:
		vy = 0;
		ay = 0;
		break;
	case QUESTBRICK_STATE_FLASH:
		if (tag == ITEM_SWITCH)
			SetState(QUESTIONBRICK_STATE_HIT);
		else {
			boundTimer.Start();
			ay = -QUESTIONBRICK_SPEED;
			repeating--;
		}
		break;
	case QUESTBRICK_STATE_FLASH_IDLE:
		vy = 0;
		ay = 0;
		break;
	default:
		//DebugOut(L"[INFO]State Questionbrick default\n");
		break;

	}
	CGameObject::SetState(state);
}

void CQuestionBrick::Render()
{
	int ani = -1;
	if (state == QUESTIONBRICK_STATE_EMPTY)
		ani = QUESTIONBRICK_ANI_EMPTY;
	else
		if (type == QUESTIONBRICK_TYPE_FLASH)
			ani = QUESTIONBRICK_ANI_FLASH;
		else
			ani = QUESTIONBRICK_ANI_IDLE; // question brick
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
		item->SetType(IGNORE_DEFINE);
		tmp_ani_set = animation_sets->Get(COIN_ANI_SET_ID);
	}
	else if (itemtype == ITEM_MUSHROOM_RED || itemtype == ITEM_MUSHROOM_GREEN)
	{
		item = new CMushroom();
		item->SetType(IGNORE_DEFINE);
		tmp_ani_set = animation_sets->Get(MUSHROOM_ANI_SET_ID);
	}
	else if (itemtype == ITEM_LEAF)
	{
		item = new CLeaf();
		item->SetType(IGNORE_DEFINE);
		tmp_ani_set = animation_sets->Get(LEAF_ANI_SET_ID);
	}
	else if (itemtype == ITEM_SWITCH)
	{
		item = new CSwitch();
		tmp_ani_set = animation_sets->Get(SWITCH_ANI_SET_ID);
	}
	item->SetAnimationSet(tmp_ani_set);
}