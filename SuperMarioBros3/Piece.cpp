#include "Piece.h"
#include "Utils.h"

CPiece::CPiece(float nx, float ny)
{
	vx = nx * PIECE_SPEED;
	vy = ny * PIECE_SPEED;
	isDestroyed = false;
	appearTimer.Start();
	SetType(IGNORE_DEFINE);
}
void CPiece::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PIECE_BBOX_WIDTH;
	b = y + PIECE_BBOX_HEIGHT;
}
void CPiece::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox(50);
	//DebugOut(L"[Piece]\n");
}
void CPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (isDestroyed)
		return;
	x += dx;
	y += dy;
	//DebugOut(L"[Piece] %f %f %f %f \n", x,y,vx,vy);
	if (appearTimer.ElapsedTime() >= PIECE_TIME)
	{
		x = y = -50;
		isDestroyed = true;
	}
}