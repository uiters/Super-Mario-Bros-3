#pragma once
#include "Brick.h"
#include "Coin.h"
#include "Utils.h"
#include "Timer.h"

#define ITEM_COIN				0
#define ITEM_CUSTOM				1
#define ITEM_LEAF				2
#define ITEM_MUSHROOM_RED		3
#define ITEM_COIN_x5			4
#define ITEM_MUSHROOM_GREEN		5
#define ITEM_SWITCH				6	

#define QUESTIONBRICK_TYPE_MARK		0
#define QUESTIONBRICK_TYPE_FLASH	1
#

#define QUESTIONBRICK_STATE_HIT			142
#define QUESTIONBRICK_STATE_IDLE		100
#define QUESTIONBRICK_STATE_EMPTY		101
#define QUESTBRICK_STATE_FLASH			102
#define QUESTBRICK_STATE_FLASH_IDLE		0

#define QUESTIONBRICK_PUSH_MAX_HEIGHT 8

#define QUESTIONBRICK_ANI_IDLE	0
#define QUESTIONBRICK_ANI_EMPTY	1
#define QUESTIONBRICK_ANI_FLASH 2

#define QUESTIONBRICK_BOUND_TIME 10
#define QUESTIONBRICK_SPEED		0.005f

class CQuestionBrick :
	public CBrick
{
	int type = 0;
	int repeating = 0;
	Timer boundTimer;
public:
	int items = 0;
	float start_y = 0;
	CQuestionBrick(int tag = ITEM_COIN, int type = QUESTIONBRICK_TYPE_MARK, int repeating = 0);
	CGameObject* item = NULL;
	void SetState(int vState);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void CreateItem(int itemtype);
};

