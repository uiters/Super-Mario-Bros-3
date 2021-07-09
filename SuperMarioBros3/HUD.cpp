#include "HUD.h"
#include"Game.h"
#include"Textures.h"
#include"PlayScene.h"
#include"WorldScene.h"
#include "BackUp.h"

#define HUD_DIFF_FIRST_ROW		8
#define HUD_DIFF_SECOND_ROW		16
#define HUD_DIFF_PLAYER			11
#define HUD_DIFF_TIME			132
#define HUD_DIFF_MONEY			140
#define HUD_DIFF_LIFE			36
#define HUD_DIFF_SCORE			60
#define HUD_DIFF_P				107
#define HUD_DIFF_CARD			172

#define HUD_TIME_MAX	3
#define HUD_SCORE_MAX	7
HUD::HUD(int type_hud)
{
	CBackUp* backup = CBackUp::GetInstance();
	nlife = backup->life;
	remainTime = DEFAULT_TIME;
	score = backup->score;
	money = backup->money;
	time = backup->time;
	runningStacks = 0;
	cards = backup->cards;
	this->type_hud = type_hud;
	if (type_hud == PLAYSCENE_HUD)
	{
		CGame* game = CGame::GetInstance();
		CSprites* sprite = CSprites::GetInstance();
		CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
		PAni = CAnimations::GetInstance()->Get(ANI_P_ID);
		mario = scene->GetPlayer();
		if (mario != NULL && mario->tag == MARIO)
			playerSprite = CSprites::GetInstance()->sprites[SPRITE_ICONMARIO_ID];
		else
			playerSprite = CSprites::GetInstance()->sprites[SPRITE_ICONLUIGI_ID];

		for (unsigned int i = 0; i < MARIO_RUNNING_STACKS - 1; i++)
			powerMelterSprite.push_back((sprite->Get(SPRITE_FILLARROW_ID)));
		TakenCards = CAnimationSets::GetInstance()->Get(CARD_ANI_SET_ID);
	}
	else
	{
		CSprites* sprite = CSprites::GetInstance();
		PAni = nullptr;
		playerSprite = CSprites::GetInstance()->sprites[SPRITE_ICONMARIO_ID];
	}
}
void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	CFont* fonts = game->GetCurrentScene()->GetFont();
	if (mario != NULL && type_hud == PLAYSCENE_HUD)
	{
		nlife = mario->life;
		runningStacks = mario->RunningStacks;
		score = mario->score;
		money = mario->money;
		if (!mario->GameDoneTimer.IsStarted())
			time += dt;
		remainTime = DEFAULT_TIME - time / 1000;
	}
	string time_str = to_string(remainTime);
	while (time_str.length() < HUD_TIME_MAX) time_str = "0" + time_str;
	remainTimeSprites = fonts->StringToSprite(time_str);

	string score_str = to_string(score);
	while (score_str.length() < HUD_SCORE_MAX) score_str = "0" + score_str;
	scoreSprite = fonts->StringToSprite(score_str);

	lifeSprite = fonts->StringToSprite(to_string(nlife));
	moneySprite = fonts->StringToSprite(to_string(money));

	if (isTakingCard)
	{
		remainTime = 0;
		runningStacks = 0;
	}
	//DebugOut(L"%d\n", money);
}
void HUD::Render()
{
	CSprites::GetInstance()->sprites[SPRITE_HUD_ID]->Draw(x, y);
	playerSprite->Draw(x + HUD_DIFF_PLAYER, y + HUD_DIFF_SECOND_ROW);
	for (unsigned int i = 0; i < remainTimeSprites.size(); i++)
		remainTimeSprites[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_TIME, y + HUD_DIFF_SECOND_ROW);
	for (unsigned int i = 0; i < scoreSprite.size(); i++)
		scoreSprite[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_SCORE, y + HUD_DIFF_SECOND_ROW);
	for (unsigned int i = 0; i < moneySprite.size(); i++)
		moneySprite[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_MONEY, y + HUD_DIFF_FIRST_ROW);
	for (unsigned int i = 0; i < lifeSprite.size(); i++)
		lifeSprite[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_LIFE, y + HUD_DIFF_SECOND_ROW);
	if (mario != NULL && type_hud == PLAYSCENE_HUD)
	{
		for (int i = 1; i <= runningStacks; i++)
			if (i == MARIO_RUNNING_STACKS)
			{
				if (PAni != nullptr)
					PAni->Render(x + HUD_DIFF_P, y + HUD_DIFF_FIRST_ROW + HUD_HEIGHT);
			}
			else
				powerMelterSprite[i - 1]->Draw(x + FONT_BBOX_WIDTH * (i - 1) + HUD_DIFF_SCORE, y + HUD_DIFF_FIRST_ROW);
	}
	if (isTakingCard)
	{
		int index = cards.size() - 1;
		if (index >= 0)
			TakenCards->at(idTakenCard)->Render(x + HUD_DIFF_CARD, y + HUD_HEIGHT + HUD_DIFF_FIRST_ROW);
	}
}
void HUD::SetHUD(HUD* hud)
{
	nlife = hud->nlife;
	remainTime = hud->remainTime;
	score = hud->score;
	money = hud->money;
	time = hud->time;
	runningStacks = hud->runningStacks;
}
