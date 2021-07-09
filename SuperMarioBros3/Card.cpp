#include "Card.h"
#include "Utils.h"
#include "PlayScene.h"

#define CARD_RENDER_CLEAR_X_DIFF 189
#define CARD_RENDER_CLEAR_Y_DIFF 83

CCard::CCard()
{
	vx = 0;
	vy = 0;
	state = CARD_STATE_RANDOM;
}
void CCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + CARD_BBOX_WIDTH;
	b = t + CARD_BBOX_WIDTH;
}
void CCard::Render()
{
	if (!isAppear)
		return;
	int ani = CARD_ANI_RANDOM;
	if (state == CARD_STATE_MUSHROOM)
		ani = CARD_ANI_MUSHROOM + CARD_ANI_STAR;
	if (state == CARD_STATE_FIREFLOWER)
		ani = CARD_ANI_FIREFLOWER + CARD_ANI_STAR;
	if (state == CARD_STATE_STAR)
		ani = CARD_ANI_STAR + CARD_ANI_STAR;
	if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->isGameDone2)
		ani -= CARD_ANI_STAR;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	y += dy;
	if (startTimer.ElapsedTime() >= CARD_TIME && startTimer.IsStarted())
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = scene->GetPlayer();
		mario->cards.push_back(state);
		isAppear = false;
		startTimer.Reset();
	}
	if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->isGameDone2)
	{
		isAppear = true;
		Camera* cam = Camera::GetInstance();
		x = cam->GetCameraPosition().x + CARD_RENDER_CLEAR_X_DIFF;
		y = cam->GetCameraPosition().y + CARD_RENDER_CLEAR_Y_DIFF;
		HUD* hud = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetHUD();
		hud->isTakingCard = true;
		hud->idTakenCard = state - 1;
		hud->cards.push_back(state);
	}

}