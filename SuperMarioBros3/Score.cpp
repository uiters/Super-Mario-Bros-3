#include "Score.h"
#include "Utils.h"
#include "HUD.h"

CScore::CScore(int score)
{
	vx = 0;
	vy = -SCORE_SPEED;
	isDestroyed = false;
	start.Start();
	state = score;
	SetType(IGNORE_DEFINE);
}
void CScore::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = r = b = t = 0;
}
void CScore::Render()
{
	if (state == 100)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_100_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 200)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_200_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 400)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_400_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 800)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_800_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 1000)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_1000_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 2000)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_2000_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 4000)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_4000_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 8000)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_8000_ID]->Draw(x, y - HUD_HEIGHT);
	if (state == 1)
		CSprites::GetInstance()->sprites[SCORE_SPRITE_1UP_ID]->Draw(x, y - HUD_HEIGHT);
}
void CScore::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	y += dy;
	if (start.ElapsedTime() >= SCORE_TIME)
		isDestroyed = true;
}