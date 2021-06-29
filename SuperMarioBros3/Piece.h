#pragma once
#include "GameObject.h"
#include "Timer.h"
#define PIECE_BBOX_WIDTH  8
#define PIECE_BBOX_HEIGHT 8

#define PIECE_ANI_SET_ID	126

#define PIECE_SPEED 0.075f

#define PIECE_TIME 500

class CPiece : public CGameObject
{
	Timer appearTimer;
public:
	CPiece(float vx, float vy);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};