#pragma once
#include "Brick.h"
#include "Coin.h"
#include "Utils.h"
#include "Timer.h"

#define ITEM_COIN				0
#define ITEM_CUSTOM				1
#define ITEM_LEAF				2
#define ITEM_MUSHROOM_RED		3
#define ITEM_MUSHROOM_GREEN		4
#define ITEM_SWITCH				6

#define QUESTIONBRICK_TYPE_MARK		0
#define QUESTIONBRICK_TYPE_FLASH	1

#define QUESTIONBRICK_STATE_HIT			142
#define QUESTIONBRICK_STATE_IDLE		100

#define QUESTIONBRICK_PUSH_MAX_HEIGHT 8

#define QUESTIONBRICK_ANI_IDLE	0
#define QUESTIONBRICK_ANI_HIT	1
#define QUESTIONBRICK_ANI_FLASH 2

#define QUESTIONBRICK_SPEED		0.05f

class CQuestionBrick :
	public CBrick
{
	int type = 0;
public:
	int items = 0;
	float start_y = 0;
	CQuestionBrick(int tag = ITEM_COIN, int type = QUESTIONBRICK_TYPE_MARK);
	CGameObject* item = NULL;
	void SetState(int vState);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void CreateItem(int itemtype);
};

