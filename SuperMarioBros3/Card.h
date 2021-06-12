#pragma once
#include "GameObject.h"
class CCard : public CGameObject
{
public:
	CCard() {};
	virtual void Render() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};