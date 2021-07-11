#include "Tanooki.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
void CTanooki::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}
void CTanooki::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (attackTimer.ElapsedTime() >= TAIL_HIT_TIME && attackTimer.IsStarted())
		attackTimer.Reset();
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (x < mario->x)
		nx = -1;
	else
		nx = 1;
	if (mario->nx > 0)
	{
		if (mario->tailState == 1 || mario->tailState == 5)
			x = mario->x - TAIL_BBOX_WIDTH;
		else if (mario->tailState == 3)
			x = mario->x + MARIO_BIG_BBOX_WIDTH;
		else x = mario->x + 2;
	}
	else
	{
		if (mario->tailState == 1 || mario->tailState == 5)
			x = mario->x + MARIO_BIG_BBOX_WIDTH;
		else if (mario->tailState == 3)
			x = mario->x - TAIL_BBOX_WIDTH;
		else x = mario->x + 2;
	}
	//x = mario->x - TAIL_BBOX_WIDTH;
	y = mario->y + TAIL_BBOX_WIDTH * 2 + 1;
}
void CTanooki::Render()
{
	if (attackTimer.IsStarted())
	{
		if (nx < 0)
			CSprites::GetInstance()->sprites[TAIL_HIT_SPRITE_ID]->Draw(x - 2, y - HUD_HEIGHT);
		if (nx > 0)
			CSprites::GetInstance()->sprites[TAIL_HIT_SPRITE_ID]->Draw(x + TAIL_BBOX_WIDTH - 2, y - HUD_HEIGHT);
	}
	RenderBoundingBox();
}