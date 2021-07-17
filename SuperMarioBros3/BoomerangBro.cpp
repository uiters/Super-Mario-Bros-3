#include "BoomerangBro.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScene.h"
CBoomerangBrother::CBoomerangBrother()
{
	SetState(BOOMERANG_BROTHER_STATE_FORWARD);
	nx = 1;
	reloadTimer.Start();
}
void CBoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOOMERANG_BROTHER_BBOX_WIDTH;
	bottom = top + BOOMERANG_BROTHER_BBOX_HEIGHT;
}

void CBoomerangBrother::HoldBoomerang()
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_ID);

	this->boomerang = new CBoomerang(this->x, this->y);
	this->boomerang->SetAnimationSet(ani_set);
	if (nx < 0) {
		this->boomerang->SetPosition(this->x + 8, this->y - 4);
		this->boomerang->nx = -1;
	}
	if (nx > 0) {
		this->boomerang->SetPosition(this->x - 8, this->y - 4);
		this->boomerang->nx = 1;
	}
	this->boomerang->isAppear = true;
	this->boomerang->SetState(BOOMERANG_STATE_IDLE);
	currentScene->GetUnit()->AddUnit(boomerang, currentScene->GetGrid());
}

void CBoomerangBrother::ThrowBoomerang()
{
	if (boomerang != NULL)
		this->boomerang->SetState(BOOMERANG_STATE_1);
}

void CBoomerangBrother::DemolishBoomerang() {
	if (state == BOOMERANG_BROTHER_STATE_DIE)
	{
		x += dx;
		y += dy;
		if (this->boomerang != NULL) {
			this->boomerang->isDestroyed = true;
			this->boomerang->isAppear = false;
			this->boomerang = NULL;
			reloadTimer.Reset();
			//DebugOut(L"DemolishBoomerang");
		}
		return;
	}
}

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	vy += BOOMERANG_BROTHER_GRAVITY * dt;
	if (x < start_x)
		vx = BOOMERANG_BROTHER_SPEED;

	if (x > start_x + BOOMERANG_BROTHER_LIMIT_RANGE)
		vx = -BOOMERANG_BROTHER_SPEED;

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL && state != BOOMERANG_BROTHER_STATE_DIE)
	{
		if (mario->tailTimer.IsStarted() && mario->GetMode() == CMario::Mode::Tanooki)
		{
			mario->getTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom))
			{
				mario->AddScore(x, y, 1000, true);
				this->nx = mario->nx;
				SetState(BOOMERANG_BROTHER_STATE_DIE);
			}
		}
		if (mario->x > x)
			nx = 1;
		else
			nx = -1;
	}

	if (reloadTimer.IsStarted() && reloadTimer.ElapsedTime() >= BOOMERANG_BROTHER_RELOAD_TIME
		&& abs(x - mario->x) <= BOOMERANG_BROTHER_ACTIVE_RANGE && state != BOOMERANG_BROTHER_STATE_DIE)
	{
		reloadTimer.Reset();
		aimTimer.Start();
		//DebugOut(L"reloadTimer");
	}
	if (aimTimer.IsStarted() && aimTimer.ElapsedTime() >= BOOMERANG_BROTHER_AIM_TIME)
	{
		aimTimer.Reset();
		if (currentBoomerang == BOOMERANG_BROTHER_BOOMERANGS)
			chargeTimer.Start();
		else if (currentBoomerang < BOOMERANG_BROTHER_BOOMERANGS)
		{
			HoldBoomerang();
			throwTimer.Start();
		}
		//DebugOut(L"aimTimer");
	}
	if (throwTimer.IsStarted() && throwTimer.ElapsedTime() >= BOOMERANG_BROTHER_THROW_TIME && currentBoomerang < BOOMERANG_BROTHER_BOOMERANGS)
	{
		throwTimer.Reset();
		ThrowBoomerang();
		currentBoomerang++;
		reloadTimer.Start();
		//DebugOut(L"throwTimer");

	}
	if (chargeTimer.IsStarted() && chargeTimer.ElapsedTime() >= BOOMERANG_BROTHER_CHANGE_TIME
		&& abs(x - mario->x) <= BOOMERANG_BROTHER_ACTIVE_RANGE && state != BOOMERANG_BROTHER_STATE_DIE)
	{
		chargeTimer.Reset();
		aimTimer.Start();
		currentBoomerang = 0;
		//DebugOut(L"chargeTimer");

	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != BOOMERANG_BROTHER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty;
		float nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		float x0 = x, y0 = y;
		x = x0 + min_tx * dx + nx * PUSHBACK;
		y = y0 + min_ty * dy + ny * PUSHBACK;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny < 0)
					vy = 0;
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
						vx = -vx;
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->nx != 0 || e->ny != 0) {
					if (koopas->GetState() == KOOPAS_STATE_SPINNING) {
						SetState(BOOMERANG_BROTHER_STATE_DIE);
						mario->AddScore(x, y, 100);
					}

				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}
void CBoomerangBrother::Render()
{
	int ani = 0;
	if (nx > 0)
	{
		if (aimTimer.IsStarted())
			ani = BOOMERANG_BROTHER_ANI_AIM_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_THROW_RIGHT;
	}
	else
	{
		if (aimTimer.IsStarted())
			ani = BOOMERANG_BROTHER_ANI_AIM_LEFT;
		else
			ani = BOOMERANG_BROTHER_ANI_THROW_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CBoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_BROTHER_STATE_FORWARD:
		vx = BOOMERANG_BROTHER_SPEED;
		break;
	case BOOMERANG_BROTHER_STATE_DIE:
		vy = -BOOMERANG_BROTHER_DEFLECT_SPEED;
		type = IGNORE_DEFINE;
		DemolishBoomerang();
		break;
	}
}