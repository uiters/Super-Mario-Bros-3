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
	r = x + PORTAL_BBOX_WIDTH;
	b = y + PORTAL_BBOX_HEIGHT;
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
		if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)) && mario->isSitting
			&& mLeft >= oLeft && mRight <= oRight)
		{
			DebugOut(L"Portal change map::\n");
			mario->portal = this;
			if (tag == BACKTOPLAYSCENE)
			{
				mario->pipeUpTimer.Start();
				mario->wannaTele = true;
			}
			if (tag == TOEXTRASCENE)
			{
				mario->pipeDownTimer.Start();
				mario->wannaTele = true;
			}
			return;
		}
		else if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)) && true)
		{
			mario->portal = this;
			if (tag == BACKTOPLAYSCENE)
			{
				mario->pipeUpTimer.Start();
				mario->wannaTele = true;
			}
			if (tag == TOEXTRASCENE)
			{
				mario->pipeDownTimer.Start();
				mario->wannaTele = true;
			}
			return;
		}
	}
}