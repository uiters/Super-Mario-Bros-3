#include "Block.h"

void CBlock::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox(0);
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLOCK_BBOX_WIDTH;
	b = y + BLOCK_BBOX_HEIGHT;
}