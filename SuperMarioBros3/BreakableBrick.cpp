#include "BreakableBrick.h"
#include "PlayScene.h"
#include "MushRoom.h"

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (isDestroyed)
		return;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL && mario->tailTimer.IsStarted() && mario->GetMode() == CMario::Mode::Tanooki)
	{
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		mario->getTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->getTail()->hit_times == 0)
		{
			Break();
			//mario->AddScore(x, y, 10, false, false);
			mario->getTail()->hit_times = 1;
			DebugOut(L"Break");
		}
	}
}
void CBreakableBrick::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox(100);
}
void CBreakableBrick::Break()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET tmp_ani_set = animation_sets->Get(PIECE_ANI_SET_ID);

	CPiece* upleft = new CPiece(-1, -1);
	upleft->SetPosition(x, y);
	upleft->SetAnimationSet(tmp_ani_set);

	CPiece* upright = new CPiece(1, -1);
	upright->SetPosition(x + PIECE_BBOX_WIDTH, y);
	upright->SetAnimationSet(tmp_ani_set);

	CPiece* downleft = new CPiece(-1, 1);
	downleft->SetPosition(x, y + PIECE_BBOX_HEIGHT);
	downleft->SetAnimationSet(tmp_ani_set);

	CPiece* downright = new CPiece(1, 1);
	downright->SetPosition(x + PIECE_BBOX_WIDTH, y + PIECE_BBOX_HEIGHT);
	downright->SetAnimationSet(tmp_ani_set);

	scene->GetUnit()->AddUnit(upleft, scene->GetGrid());
	scene->GetUnit()->AddUnit(upright, scene->GetGrid());
	scene->GetUnit()->AddUnit(downleft, scene->GetGrid());
	scene->GetUnit()->AddUnit(downright, scene->GetGrid());


	isDestroyed = true;
}