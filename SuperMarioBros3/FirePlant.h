#pragma once
#include "GameObject.h"
#define PIRANHAPLANT_GREEN_TYPE					0

class CFirePlant : public CGameObject
{
public:
	CFirePlant(int tag = PIRANHAPLANT_GREEN_TYPE) {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render() {};
	virtual void SetState(int state) {};
};

