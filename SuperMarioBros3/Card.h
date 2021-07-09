#pragma once
#include "GameObject.h"
#include "Timer.h"

#define CARD_ANI_RANDOM		0
#define CARD_ANI_MUSHROOM	1
#define CARD_ANI_FIREFLOWER	2
#define CARD_ANI_STAR		3

#define CARD_STATE_RANDOM		0
#define CARD_STATE_MUSHROOM		1
#define CARD_STATE_FIREFLOWER	2
#define CARD_STATE_STAR			3

#define CARD_BBOX_WIDTH		16

#define CARD_SPEED 0.1f

#define CARD_TIME  1000

class CCard : public CGameObject
{
	Timer startTimer;
	bool isAppear = true;
public:
	CCard();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};