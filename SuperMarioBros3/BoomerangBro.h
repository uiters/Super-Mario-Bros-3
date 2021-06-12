#pragma once
#include "GameObject.h"
class CBoomerangBro : public CGameObject
{
public:
	CBoomerangBro() {};
	virtual void SetState(int state) {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render() {};
};

