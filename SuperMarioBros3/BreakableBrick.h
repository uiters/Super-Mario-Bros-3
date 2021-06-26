#pragma once
#include "Brick.h"
#include "Coin.h"
#include "Utils.h"
#include "Piece.h"


#define BREAKABLEBRICK_ANI_SET_ID 12
class CBreakableBrick :
	public CBrick
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) ;
	virtual void Render() ;
	void Break() ;
};

