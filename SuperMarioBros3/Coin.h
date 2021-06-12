#pragma once
#include "GameObject.h"
class CCoin : public CGameObject
{
public:
	CCoin(int tag = 6) {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render() {};
};

