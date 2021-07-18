#include "Goomba.h"
#include "Brick.h"
#include "Utils.h"
#include "Block.h"
#include "Mario.h"
#include "PlayScene.h"
#include "PlantBullet.h"
#include "TinyGoomba.h"

CGoomba::CGoomba(int tag)
{
	nx = -1;
	SetState(GOOMBA_STATE_WALKING);
	if (tag == GOOMBA_SUPER)
		SetState(GOOMBA_SUPER_STATE_WALKING);
}
void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE_BY_MARIO)
	{
		left = top = right = bottom = 0;
		return;
	}
	left = x;
	top = y;
	right = x + GOOMBA_NORMAL_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
	if (tag == GOOMBA_RED)
	{
		right = x + GOOMBA_RED_BBOX_WIDTH;
		bottom = y + GOOMBA_RED_BBOX_WINGS_HEIGHT;
		if (state == GOOMBA_STATE_RED_WINGSWALKING)
			bottom = y + GOOMBA_RED_BBOX_HEIGHT;
	}
	else if (tag == GOOMBA_SUPER) {
		right = x + GOOMBA_BIG_BBOX_WIDTH;
		if (!flyingTimer.IsStarted()) {
			bottom = y + GOOMBA_BIG_BBOX_HEIGHT;
		}
		else bottom = y + GOOMBA_BIG_BBOX_WINGS_HEIGHT;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CMario* mario = {};
	mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (dyingTimer.ElapsedTime() >= GOOMBA_TIME_DIYING && isDying)
	{
		dyingTimer.Reset();
		isDestroyed = true;
		return;
	}
	if (dyingTimer.ElapsedTime() >= GOOMBA_TIME_DIYING_BY_TAIL && isWhackedDying)
	{
		isWhackedDying = false;
		isDestroyed = true;
		return;
	}
	if (tag == GOOMBA_RED && state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_BY_MARIO)
	{
		if (walkingTimer.ElapsedTime() >= GOOMBA_RED_TIME_WALKING && walkingTimer.IsStarted())
		{
			walkingTimer.Reset();
			jumpingStacks = 0;
			y -= GOOMBA_RED_BBOX_WINGS_HEIGHT - GOOMBA_RED_BBOX_HEIGHT;
			SetState(GOOMBA_STATE_RED_JUMPING);
		}
		if (chasingTimer.ElapsedTime() >= GOOMBA_RED_TIME_CHASING && chasingTimer.IsStarted())
		{
			chasingTimer.Reset();
		}
	}
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	// Simple fall down
	vy += ay * dt;

	//// limit
	if (vy < -GOOMBA_JUMP_SPEED && state == GOOMBA_STATE_RED_JUMPING)
	{
		vy = -GOOMBA_JUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}
	if (vy < -GOOMBA_HIGHJUMP_SPEED && state == GOOMBA_STATE_RED_HIGHJUMPING)
	{
		vy = -GOOMBA_HIGHJUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	//// turn off collision when goomba kicked 
	if (state != GOOMBA_STATE_DIE_BY_MARIO)
		CalcPotentialCollisions(coObjects, coEvents);

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL && state != GOOMBA_STATE_DIE_BY_MARIO && state != GOOMBA_STATE_DIE)
	{
		if (mario->tailTimer.IsStarted() && mario->GetMode() == CMario::Mode::Tanooki)
		{
			mario->getTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom))
			{
				mario->AddScore(x, y, 100, true);
				this->nx = mario->nx;
				SetState(GOOMBA_STATE_DIE_BY_MARIO);
				return;
			}
		}
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom)))
		{
			if (abs(mBottom - oTop) <= 1.0f && mario->vy > 0)
			{
				mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
				if (tag == GOOMBA_RED)
					SetTag(GOOMBA_RED_NORMAL);
				else
					SetState(GOOMBA_STATE_DIE);
				return;
			}
		}
		if (abs(mario->x - x) <= GOOMBA_RED_RANGE_CHASING && tag == GOOMBA_RED && chasingTimer.IsStarted())
			chasingTimer.Start();
		if (abs(mario->x - x) >= GOOMBA_RED_RANGE_CHASING && tag == GOOMBA_RED && !chasingTimer.IsStarted())
		{
			if (x < mario->x)
			{
				nx = 1;
				vx = GOOMBA_WALKING_SPEED;
			}
			else
			{
				nx = -1;
				vx = -GOOMBA_WALKING_SPEED;
			}
			//DebugOut(L"[INFO] goomba %d\n", chasingTimer.IsStarted() ? 1 : 0);
		}
	}
	HandleBigGoomba();
	//// No collision occured, proceed normally
	if (coEvents.size() == 0 || state == GOOMBA_STATE_DIE_BY_MARIO)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, x0, y0;
		float nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x0 = x;
		y0 = y;

		x += min_tx * dx + nx * PUSHBACK;		// nx*PUSHBACK : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * PUSHBACK;

		if (ny != 0)
			vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj != NULL)
				if (e->obj->isDestroyed == true)
					continue;
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					if (goomba->tag == GOOMBA_RED || tag == GOOMBA_RED)
					{
						x = x0 + dx;
						y = y0;
					}
					else
					{
						goomba->vx = -goomba->vx;
						this->vx = -this->vx;
						goomba->nx = -goomba->nx;
						this->nx = -this->nx;
					}

				}

			}
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* object = dynamic_cast<CBrick*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);

				if (e->ny != 0)
				{
					vy = 0;
					if (e->ny < 0 && tag == GOOMBA_RED && state != GOOMBA_STATE_DIE)
					{
						if (!walkingTimer.IsStarted())
						{
							if (jumpingStacks == GOOMBA_RED_JUMPING_STACKS)
							{
								SetState(GOOMBA_STATE_RED_HIGHJUMPING);
								jumpingStacks = -1;
							}
							else
							{
								if (jumpingStacks == -1)
									SetState(GOOMBA_STATE_RED_WINGSWALKING);
								else
									SetState(GOOMBA_STATE_RED_JUMPING);
								jumpingStacks++;
							}
						}
						else
							ay = GOOMBA_GRAVITY;
					}
					else if (e->ny > 0)
						ay = GOOMBA_GRAVITY;
				}
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
					{
						vx = -vx;
						this->nx = -this->nx;
					}
				}
			}
			if (dynamic_cast<CBlock*>(e->obj))
			{
				x = x0 + dx;
				y = y0 + dy;
			}
		}
	}
	// limit screen
	if (vx < 0 && x <= 0) {
		x = 0;
		vx = -vx;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}

void CGoomba::Render()
{
	int ani = 0;
	switch (tag)
	{
	case GOOMBA_NORMAL:
		ani = GOOMBA_NORMAL_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE)
			ani = GOOMBA_NORMAL_ANI_DIE;
		break;
	case GOOMBA_RED:
		ani = GOOMBA_RED_ANI_WINGSWALKING;
		if (state == GOOMBA_STATE_RED_JUMPING || state == GOOMBA_STATE_RED_HIGHJUMPING)
			ani = GOOMBA_RED_ANI_JUMPING;
		if (state == GOOMBA_STATE_DIE_BY_MARIO)
			ani = GOOMBA_RED_ANI_WALKING;
		break;
	case GOOMBA_RED_NORMAL:
		ani = GOOMBA_RED_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE)
			ani = GOOMBA_RED_ANI_DIE;
		break;
	case GOOMBA_SUPER:
		ani = GOOMBA_ANI_WALK;
		if (state == GOOMBA_SUPER_STATE_FLYING) {
			ani = GOOMBA_ANI_FLY;
		}
		else if (state == GOOMBA_SUPER_STATE_WALKING) {
			if (!flyingTimer.IsStarted()) ani = GOOMBA_ANI_WALK;
			else if (flyingTimer.IsStarted()) ani = GOOMBA_ANI_WING_WALK;
		}
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_NORMAL_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		StartDying();
		break;
	case GOOMBA_STATE_DIE_BY_MARIO:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = -vx;
		ay = GOOMBA_GRAVITY;
		StartDying(true);
		SetType(IGNORE_DEFINE);
		break;
	case GOOMBA_STATE_RED_JUMPING:
		ay = -GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_RED_HIGHJUMPING:
		ay = -GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_WALKING:
		vx = nx * GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_RED_WINGSWALKING:
		walkingTimer.Start();
		ay = GOOMBA_GRAVITY;
		break;
		//super goomba
	case GOOMBA_SUPER_STATE_WALKING:
		goombaWalkingTimer.Start();
		SetSpeedDirection(mario->x);
		ay = GOOMBA_GRAVITY;
		break;
	case GOOMBA_SUPER_STATE_FLYING:
		ay = -0.0005f;
		SetSpeedDirection(mario->x);
		droppingTimer.Start();
		break;
	}
	CGameObject::SetState(state);
}

void CGoomba::HandleBigGoomba() {

	if (tag == GOOMBA_SUPER) {

		if (goombaWalkingTimer.ElapsedTime() >= GOOMBA_BIG_TIME_WALKING && goombaWalkingTimer.IsStarted() && !droppingTimer.IsStarted()) {
			flyingTimer.Start();
			goombaWalkingTimer.Reset();
			y -= GOOMBA_BIG_BBOX_WINGS_HEIGHT - GOOMBA_BIG_BBOX_HEIGHT + 2;
			//DebugOut(L"Waling\n");
		}

		if (flyingTimer.ElapsedTime() >= GOOMBA_BIG_TIME_READY_FLY && flyingTimer.IsStarted() && !droppingTimer.IsStarted()) {
			flyingTimer.Reset();
			SetState(GOOMBA_SUPER_STATE_FLYING);
			//DebugOut(L"Fly\n");
		}

		if (droppingTimer.IsStarted() && droppingTimer.ElapsedTime() >= GOOMBA_BIG_TIME_DROP && createTinyGoomba < MAX_TINY_GOOMBA) {
			createTinyGoomba++;
			CreatePoopGoomba();
		}

		if (createTinyGoomba > MAX_TINY_GOOMBA) {
			droppingTimer.Reset();
			createTinyGoomba = 0;
			SetState(GOOMBA_SUPER_STATE_WALKING);
		}

		//DebugOut(L"[POOP_GOOMBA]::%d\n");

		//LIMIT Y
		if (y <= 300.0f) {
			if (vy < 0) {
				vy = 0.03f;
			}
		}

		if (vy <= -0.07f) {
			vy = -0.07f;
		}
	}
}
// SUPER GOOMBA ONLY
void CGoomba::SetSpeedDirection(int condition) {
	if (tag == GOOMBA_SUPER) {
		if (x < condition) {
			vx = GOOMBA_WALKING_SPEED;
		}
		if (x > condition) {
			vx = -GOOMBA_WALKING_SPEED;
		}
	}
}
void CGoomba::CreatePoopGoomba() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(GOOMBA_ANISET_ID);
	CTinyGoomba* pGoomba = new CTinyGoomba();

	pGoomba->SetAnimationSet(ani_set);
	pGoomba->SetPosition(x, y);
	currentScene->GetUnit()->AddUnit(pGoomba, currentScene->GetGrid());

	DebugOut(L"[POOP_GOOMBA]::\n");

}