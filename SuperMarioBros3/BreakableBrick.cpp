#include "BreakableBrick.h"
#include "PlayScene.h"
#include "MushRoom.h"

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (isDestroyed)
		return;

}
void CBreakableBrick::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
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