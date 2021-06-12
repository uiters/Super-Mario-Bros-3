#pragma once
#include "GameObject.h"
class CPlant : public CGameObject
{
public:
	CPlant() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render() {};
	virtual void SetState(int state) {};
};

