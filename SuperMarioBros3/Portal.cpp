#include "Portal.h"
#include "PlayScene.h"

CPortal::CPortal(int scene_id, float start_x, float start_y)
{
	this->scene_id = scene_id;
	this->start_x = start_x;
	this->start_y = start_y;
	SetType(IGNORE);
}

void CPortal::Render()
{

	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	b = y + PORTAL_BBOX_HEIGHT;
	if (scene_id == 4)
		r = x + PORTAL_BBOX_WIDTH_1_3;
	else
		r = x + PORTAL_BBOX_WIDTH;
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	if (mario != NULL)
	{
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom))
			&& mario->isSitting
			&& mLeft >= oLeft && mRight <= oRight)
		{
			mario->portal = this;
			if (tag == BACKTOPLAYSCENE)
			{
				if (scene_id == 1)
					mario->pipeUpTimer.Start();

				else if (scene_id == 3)
					mario->pipeDownTimer.Start();

				mario->isBackScene = true;
				mario->wannaTele = true;
			}
			if (tag == TOEXTRASCENE && scene_id == 2)
			{
				mario->pipeDownTimer.Start();
				mario->wannaTele = true;
			}
			return;
		}
		if (tag == TOEXTRASCENE && scene_id == 4 && mario->isJumpMusicBrick)
		{
			mario->portal = this;
			mario->isJumpMusicBrick = false;
			mario->pipeUpTimer.Start();
			mario->wannaTele = true;
			mario->isTravel = true;
			DebugOut(L"tele map");
		}
	}
}