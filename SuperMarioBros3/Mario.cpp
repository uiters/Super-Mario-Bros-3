#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Brick.h"
#include "Block.h"
#include "BreakableBrick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Plant.h"
#include "Switch.h"
#include "Tanooki.h"
#include "FirePlant.h"
#include "Score.h"
#include "MusicalBrick.h"
#include "Card.h"
#include "Abyss.h"
CMario::CMario(float x, float y) : CGameObject()
{
	Transform(Mode::Small);
	SetState(MARIO_STATE_IDLE);
	isGround = true;
	dead = false;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	tail = new CTanooki(x, y);
}

CMario::~CMario()
{
}

void CMario::Attacked()
{
	untouchableTimer.Start();
	if (GetMode() == Mode::Small)
	{
		CGame* game = CGame::GetInstance();
		SetState(MARIO_STATE_DIE);
		GameDoneTimer.Reset();
		((CPlayScene*)game->GetCurrentScene())->isGameDone3 = true;
		return;
	}
	if (GetMode() == Mode::Tanooki || GetMode() == Mode::Fire)
		Transform(Mode::Super);
	else if (GetMode() == Mode::Super)
		Transform(Mode::Small);

}

void CMario::Transform(Mode m) {

	switch (m)
	{
	case CMario::Mode::Small:
		if (this->mode != Mode::Small) {
			SetPosition(x, y + 8);
			SetSpeed(0, 0);
		}
		break;
	case CMario::Mode::Super:
	case CMario::Mode::Fire:
	case CMario::Mode::Tanooki:
		if (this->mode == Mode::Small) {
			SetPosition(x, y - 16);
			SetSpeed(0, 0);
		}
		break;
	default:
		break;
	}
	prevMode = mode;
	mode = m;
	if (prevMode != mode) {
		transformTimer.Start();
	}
}

void CMario::RunTimer() {

	if (GameDoneTimer.ElapsedTime() > MARIO_GAMEDONE_TIME && GameDoneTimer.IsStarted())
	{
		GameDoneTimer.Reset();
		CGame::GetInstance()->SwitchScene(0);
		return;
	}
	if (transformTimer.ElapsedTime() >= MARIO_TRANSFORMING_TIME && transformTimer.IsStarted())
	{
		transformTimer.Reset();
	}
	if (untouchableTimer.ElapsedTime() >= MARIO_UNTOUCHABLE_TIME && untouchableTimer.IsStarted()) {
		untouchableTimer.Reset();
	}
	if (pipeDownTimer.ElapsedTime() >= MARIO_PIPE_TIME && pipeDownTimer.IsStarted())
	{
		pipeDownTimer.Reset();
		isSitting = false;
		//DebugOut(L"[INFO] portal-scene:: %d", portal->GetSceneId());
		if (wannaTele)
			CGame::GetInstance()->SwitchExtraScene(portal->GetSceneId(), portal->start_x, portal->start_y, portal->pipeUp);
		else
		{
			vx = vy = 0;
			ay = MARIO_GRAVITY;
		}
	}
	if (pipeUpTimer.ElapsedTime() >= MARIO_PIPE_TIME && pipeUpTimer.IsStarted())
	{
		pipeUpTimer.Reset();
		isSitting = false;
		//DebugOut(L"[INFO] portal-scene:: %d", portal->GetSceneId());
		if (wannaTele)
			CGame::GetInstance()->SwitchBackScene(portal->GetSceneId(), portal->start_x, portal->start_y);
		else
		{
			vx = vy = 0;
			ay = MARIO_GRAVITY;
		}
	}
	if (tailStateTimer.ElapsedTime() >= MARIO_TURNING_STATE_TIME && tailTimer.IsStarted())
	{
		tailStateTimer.Start();
		tailState++;
	}
	if (tailTimer.ElapsedTime() > MARIO_TURNING_TAIL_TIME && tailTimer.IsStarted())
	{
		tailTimer.Reset();
		tailStateTimer.Reset();
		tailState = 0;
		tail->hit_times = 0;
	}
	if (shootingTimer.ElapsedTime() > MARIO_SHOOTING_TIME && shootingTimer.IsStarted())
	{
		shootingTimer.Reset();
	}
	if (kickTimer.ElapsedTime() > MARIO_KICKING_TIME && kickTimer.IsStarted())
	{
		kickTimer.Reset();
	}
	if (runningTimer.ElapsedTime() >= MARIO_RUNNING_STACK_TIME && runningTimer.IsStarted() && vx != 0 && isReadyToRun)
	{
		runningTimer.Start();
		RunningStacks++;
		if (RunningStacks > MARIO_RUNNING_STACKS)
		{
			RunningStacks = MARIO_RUNNING_STACKS;

			vx = nx * MARIO_RUNNING_SPEED_MAX;
		}
	}
	if (flyTimer.ElapsedTime() >= MARIO_FLYING_TIME && flyTimer.IsStarted())
	{
		flyTimer.Reset();
	}

	if (stoppingTimer.ElapsedTime() >= MARIO_SLOW_STACK_TIME && stoppingTimer.IsStarted() && !runningTimer.IsStarted())
	{
		stoppingTimer.Start();
		RunningStacks--;
		if (RunningStacks < 0)
		{
			RunningStacks = 0;
			flyTimer.Reset();
		}
		//DebugOut(L"STOP\n");
	}

}

void CMario::LimitSpeed() {
	//slow down if change direction when running
	if (vx * ax < 0 && abs(vx) > MARIO_WALKING_SPEED_MAX
		&& (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_WALKING_RIGHT))
	{
		vx = -nx * MARIO_WALKING_SPEED_MAX;
		isChangeDirection = true;
		if (RunningStacks < 0)
			RunningStacks = 0;
	}
	vx += ax * dt + RunningStacks * ax;
	vy += ay * dt;
	//limit the speed of mario 
	if (abs(vx) >= MARIO_WALKING_SPEED_MAX && !isReadyToRun)
	{
		vx = nx * MARIO_WALKING_SPEED_MAX;
	}
	if (abs(vx) >= MARIO_RUNNING_SPEED_MAX && isReadyToRun) {
		vx = nx * MARIO_RUNNING_SPEED_MAX;
	}
	// falling
	if (vy > MARIO_JUMP_SPEED_MAX)
	{
		vy = MARIO_JUMP_SPEED_MAX;
		ay = MARIO_GRAVITY;
	}
	// jump
	if (vy <= -MARIO_JUMP_SPEED_MAX && !IsLostControl())
	{
		vy = -MARIO_JUMP_SPEED_MAX;
		ay = MARIO_GRAVITY;
		isGround = false;
	}
	if (flyTimer.IsStarted())
	{
		vy = -MARIO_FLY_SPEED;
		ay = -MARIO_GRAVITY;
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	Camera* cam = Camera::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);


	if (GameDoneTimer.ElapsedTime() > MARIO_GAMEDONE_TIME && GameDoneTimer.IsStarted())
	{
		GameDoneTimer.Reset();
		((CPlayScene*)game->GetCurrentScene())->isGameDone3 = true;
		return;
	}
	if (((CPlayScene*)game->GetCurrentScene())->isGameDone2 == true)
		return;
	if (y > ABYSS_HIGH)
	{
		GameDoneTimer.Reset();
		((CPlayScene*)game->GetCurrentScene())->isGameDone3 = true;
		life--;
		return;
	}


	//DebugOut(L"x %f y %f\n", x, y);
	if (!runningTimer.IsStarted() && isReadyToRun)
	{
		runningTimer.Start();
		stoppingTimer.Reset();
	}
	// nếu chuyển hướng chạy hoặc thả nút boôst thì sẽ giảm tốc từ từ
	else if (!stoppingTimer.IsStarted() && !isReadyToRun)
	{
		stoppingTimer.Start();
		runningTimer.Reset();
	}

	RunTimer();
	LimitSpeed();


	//handle for sitting when jump
	if (state == MARIO_STATE_SITTING && vy < 0)
		vy -= MARIO_ACCELERATION_JUMP * dt;
	//cant jump again until touch the ground
	if (vy < 0)
		isGround = false;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		//camera move to end map
		x += dx;
		y += dy;
		return;
	}
	if (isPipe())
	{
		if (pipeDownTimer.IsStarted())
			y += 0.5f;
		else if (pipeUpTimer.IsStarted())
		{
			if (GetMode() == Mode::Small)
				y += -0.3f; // perfect position which is not overloap
			else
				y += -0.5f;
		}
	}
	else
	{
		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
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
			float oLeft, oTop, oRight, oBottom;			// object Collision
			float mLeft, mTop, mRight, mBottom;			//mario Collision
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->ny != 0 && !(dynamic_cast<CBlock*>(e->obj) && ny > 0))
				{
					if (!(dynamic_cast<CBrick*>(e->obj) && e->obj->tag == PIPE && flyTimer.IsStarted()))
					{
						isGround = true;
						isJumping = false;
						flyTimer.Reset();
					}
				}
				GetBoundingBox(mLeft, mTop, mRight, mBottom);
				e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				//brick
				if (dynamic_cast<CBrick*>(e->obj)) {
					if (GameDoneTimer.IsStarted())
					{
						SetState(MARIO_STATE_WALKING_RIGHT);
						vx = MARIO_WALKING_SPEED_MAX;
						this->nx = 1;
					}
					if (!(tag == PIPE))
					{
						if (e->ny < 0) // mario collide from top to bottom
						{
							vy = 0;
						}
						if (e->ny > 0)// mario collide from bottom to top
						{
							vy = 0;
							ay = MARIO_GRAVITY;
						}
						if (e->nx != 0)
						{
							if (ceil(mBottom) != oTop)
							{
								vx = 0;
							}
						}
					}
				}
				//questionbrick
				if (dynamic_cast<CQuestionBrick*>(e->obj) && e->ny > 0)
				{
					if (e->obj->state != QUESTIONBRICK_STATE_EMPTY)
					{
						if (e->obj->type == QUESTIONBRICK_TYPE_FLASH)
							e->obj->SetState(QUESTBRICK_STATE_FLASH);

						else
						{
							e->obj->SetState(QUESTIONBRICK_STATE_HIT);
						}

					}
				}
				//breakablebrick
				if (dynamic_cast<CBreakableBrick*>(e->obj) && e->ny > 0)
				{
					AddScore(e->obj->x, e->obj->y, 10, false, false);
					((CBreakableBrick*)e->obj)->Break();
				}
				//block
				else if (dynamic_cast<CBlock*>(e->obj)) {
					if (e->nx != 0 && ceil(mBottom) != oTop)
						x = x0 + dx;
					if (e->ny < 0)
					{
						vy = 0;
					}
					if (e->ny > 0 && vy < 0)
						y = y0 + dy;
				}
				// music brick
				else if (dynamic_cast<CMusicalBrick*>(e->obj)) {
					CMusicalBrick* msBrick = dynamic_cast<CMusicalBrick*>(e->obj);
					if (e->nx != 0) {
						vx = 0;
					}
					if (e->ny < 0) {
						msBrick->SetState(MUSIC_BRICK_STATE_HIT_FROM_TOP);
						ay = -MARIO_GRAVITY;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					if (e->ny > 0) {
						msBrick->SetState(MUSIC_BRICK_STATE_HIT_FROM_DOWN);
						vy = 0;
						ay = MARIO_GRAVITY;
					}
				}
				//goomba
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							AddScore(goomba->x, goomba->y, 100, true);
							if (goomba->tag != GOOMBA_RED)
							{
								goomba->SetState(GOOMBA_STATE_DIE);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
							}
							else
							{
								goomba->SetTag(GOOMBA_RED_NORMAL);
								goomba->SetState(GOOMBA_STATE_WALKING);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
							}
						}
						else
							vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						y = y0;
						if (!untouchableTimer.IsStarted())
							Attacked();
						else
						{
							x = x0 + dx;
							y = y0;
							if (e->ny > 0 && vy < 0)
								y = y0 + dy;
							if (e->nx != 0 && tailTimer.IsStarted())
							{
								AddScore(goomba->x, goomba->y, 100, true);
								goomba->SetState(GOOMBA_STATE_DIE_BY_MARIO);
							}
						}
					}
				}
				//bomerang

				//koopas
				else if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas 
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					if (e->ny < 0)
					{
						AddScore(koopas->x, koopas->y, 100, true);
						vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
						if (this->nx > 0)
						{
							if (vx < MARIO_WALKING_SPEED_MIN * 2)
								vx = MARIO_WALKING_SPEED_MIN * 2;
						}
						else
						{
							if (vx > -MARIO_WALKING_SPEED_MIN * 2)
								vx = -MARIO_WALKING_SPEED_MIN * 2;
						}
						if (koopas->tag == KOOPAS_GREEN_PARA)
							koopas->tag = KOOPAS_GREEN;
						else if (koopas->tag == KOOPAS_RED_PARA)
							koopas->tag = KOOPAS_RED;
						else if (koopas->GetState() != KOOPAS_STATE_IN_SHELL && koopas->GetState() != KOOPAS_STATE_SHELL_UP)
							koopas->SetState(KOOPAS_STATE_IN_SHELL);
						else
							koopas->SetState(KOOPAS_STATE_SPINNING);
					}
					else
					{
						if (e->ny > 0)
							y = y0;
						if (untouchableTimer.IsStarted())
						{
							x = x0 + dx;
							if (e->ny > 0 && vy < 0)
								y = y0 + dy;
						}
						if (koopas->GetState() == KOOPAS_STATE_IN_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_UP)
						{
							if (isReadyToHold)
							{
								isHold = true;
								koopas->SetIsHold(true);
							}
							else
							{
								kickTimer.Start();
								koopas->nx = this->nx;
								koopas->SetState(KOOPAS_STATE_SPINNING);
							}
						}
						else if (!untouchableTimer.IsStarted() && (koopas->GetState() == KOOPAS_STATE_SPINNING || koopas->GetState() == KOOPAS_STATE_WALKING))
							Attacked();
					}
				}
				//piranhaPlant
				else if (dynamic_cast<CPlant*>(e->obj) || dynamic_cast<CFirePlant*>(e->obj))
				{
					if (!untouchableTimer.IsStarted())
					{
						Attacked();
					}
					else {
						x = x0 + dx;
						if (e->ny < 0)
						{
							y = y0 + dy;
						}
					}
				}
				//switch
				else if (dynamic_cast<CSwitch*>(e->obj))
				{
					CSwitch* sw = dynamic_cast<CSwitch*>(e->obj);
					if (e->ny > 0)
					{
						//vy = -MARIO_JUMP_SPEED_MAX;
						ay = MARIO_GRAVITY;
						//isReadyToJump = false;
					}
					else if (e->ny < 0)
					{
						vy = 0;
						if (sw->state != SWITCH_STATE_PRESSED)
							sw->SetState(SWITCH_STATE_PRESSED);
					}
					if (e->nx != 0)
					{
						if (ceil(mBottom) != oTop)
							vx = 0;
					}
				}
				//mario

				//card
				else if (dynamic_cast<CCard*>(e->obj))
				{
					srand(time(NULL));
					int id = rand() % 3 + 1;
					e->obj->vy = -CARD_SPEED;
					e->obj->SetState(id);
					GameDoneTimer.Start();
					cards.push_back(id);
					vy = 0;
					ay = MARIO_GRAVITY;
					isJumping = false;
				}
				//port
				else if (dynamic_cast<CPortal*>(e->obj))
				{
				}
			}
		}
	}
	//! Limit y when fly
	if (flyTimer.IsStarted() && cam->GetCameraPosition().y == 0 && y < 40) {
		y = 30;
	}

	tail->Update(dt);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CMario::RenderBasicMoving(int& ani, int ani_jump_down_right, int ani_jump_down_left,
	int ani_idle_right, int ani_idle_left,
	int ani_baking_right, int ani_baking_left, int ani_walking_right, int ani_walking_left, int ani_kicking_right, int ani_kicking_left)
{
	if (vx == 0)
	{
		if (isGround)
		{
			if (nx > 0) ani = ani_idle_right;
			else ani = ani_idle_left;
			if (kickTimer.IsStarted())
				if (nx > 0) ani = ani_kicking_right;
				else ani = ani_kicking_left;
		}
		else
		{
			if (nx > 0) ani = ani_jump_down_right;
			else ani = ani_jump_down_left;

		}
	}
	else {
		if (vx > 0 && nx < 0) {
			ani = ani_baking_right;
		}
		else if (vx < 0 && nx >0) {
			ani = ani_baking_left;
		}
		else if (vx > 0 && nx > 0) {
			ani = ani_walking_right;
		}
		else if (vx < 0 && nx < 0) {
			ani = ani_walking_left;
		}
		if (kickTimer.IsStarted())
		{
			if (vx > 0)
				ani = ani_kicking_right;
			else if (vx < 0)
				ani = ani_kicking_left;
			//DebugOut(L"[KICK]\n");
		}
	}
	if (shootingTimer.IsStarted())
	{
		if (nx > 0)
			ani = MARIO_ANI_SHOOTING_RIGHT;
		else if (nx < 0)
			ani = MARIO_ANI_SHOOTING_LEFT;
	}
	if (vx != 0 && vy > 0 && !isGround)
	{
		if (nx > 0) ani = ani_jump_down_right;
		else ani = ani_jump_down_left;
	}
}

void CMario::RenderJumping(int& ani, int ani_jump_up_right, int ani_jump_up_left, int ani_jump_down_right, int ani_jump_down_left)
{
	if (!isGround)
	{
		if (nx > 0 && vy < 0)
			ani = ani_jump_up_right;
		else if (nx < 0 && vy < 0)
			ani = ani_jump_up_left;
		else if (nx > 0 && vy >= 0)
			ani = ani_jump_down_right;
		else if (nx < 0 && vy >= 0)
			ani = ani_jump_down_left;
	}
	if (shootingTimer.IsStarted())
	{
		if (nx > 0)
			ani = MARIO_ANI_SHOOTING_JUMP_RIGHT;
		else
			ani = MARIO_ANI_SHOOTING_JUMP_LEFT;
	}
}

void CMario::TransformAni(int& ani) {
	if (prevMode == Mode::Small && GetMode() == Mode::Super || GetMode() == Mode::Fire || GetMode() == Mode::Tanooki)
		ani = MARIO_ANI_TRANSFORM_BANG;
	else
	{
		if (nx > 0)
			ani = MARIO_ANI_TRANSFORM_SMALL_RIGHT;
		else if (nx < 0)
			ani = MARIO_ANI_TRANSFORM_SMALL_LEFT;
	}
}

void CMario::TransformSmallAni(int& ani) {
	if (vx == 0)
	{
		if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
		else ani = MARIO_ANI_SMALL_IDLE_LEFT;
	}
	else if (isJumping)
	{
		if (isHold)
		{
			RenderJumping(ani,
				MARIO_ANI_SMALL_HOLD_JUMPINGUP_RIGHT,
				MARIO_ANI_SMALL_HOLD_JUMPINGUP_LEFT,
				MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_RIGHT,
				MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_LEFT);
		}
		else {
			if (flyTimer.IsStarted())
			{
				RenderJumping(ani,
					MARIO_ANI_SMALL_FLY_RIGHT,
					MARIO_ANI_SMALL_FLY_LEFT,
					MARIO_ANI_SMALL_FLY_RIGHT,
					MARIO_ANI_SMALL_FLY_LEFT);
			}
			else
				RenderJumping(ani,
					MARIO_ANI_SMALL_JUMPINGUP_RIGHT,
					MARIO_ANI_SMALL_JUMPINGUP_LEFT,
					MARIO_ANI_SMALL_JUMPINGDOWN_RIGHT,
					MARIO_ANI_SMALL_JUMPINGDOWN_LEFT);
		}
	}
	else {
		if (isHold)
		{
			RenderBasicMoving(ani,
				MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_RIGHT, MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_LEFT,
				MARIO_ANI_SMALL_HOLD_IDLE_RIGHT, MARIO_ANI_SMALL_HOLD_IDLE_LEFT,
				MARIO_ANI_SMALL_HOLD_BRAKING_RIGHT, MARIO_ANI_SMALL_HOLD_BRAKING_LEFT,
				MARIO_ANI_SMALL_HOLD_WALKING_RIGHT, MARIO_ANI_SMALL_HOLD_WALKING_LEFT, MARIO_ANI_SMALL_KICKING_RIGHT, MARIO_ANI_SMALL_KICKING_LEFT);

		}
		else {
			int ani_go_right = MARIO_ANI_SMALL_WALKING_RIGHT;
			int ani_go_left = MARIO_ANI_SMALL_WALKING_LEFT;
			if (runningTimer.IsStarted())
				if (abs(vx) >= MARIO_RUNNING_SPEED_MAX)
				{
					ani_go_right = MARIO_ANI_SMALL_WALKING_FAST_RIGHT;
					ani_go_left = MARIO_ANI_SMALL_WALKING_FAST_LEFT;
				}
			if (RunningStacks == MARIO_RUNNING_STACKS)
			{
				ani_go_right = MARIO_ANI_SMALL_RUNNING_RIGHT;
				ani_go_left = MARIO_ANI_SMALL_RUNNING_LEFT;
			}
			RenderBasicMoving(ani,
				MARIO_ANI_SMALL_JUMPINGDOWN_RIGHT, MARIO_ANI_SMALL_JUMPINGDOWN_LEFT,
				MARIO_ANI_SMALL_IDLE_RIGHT, MARIO_ANI_SMALL_IDLE_LEFT,
				MARIO_ANI_SMALL_BRAKING_RIGHT, MARIO_ANI_SMALL_BRAKING_LEFT,
				ani_go_right, ani_go_left, MARIO_ANI_SMALL_KICKING_RIGHT, MARIO_ANI_SMALL_KICKING_LEFT);

		}
	}
}

void CMario::TransformBigAni(int& ani) {

	if (state == MARIO_STATE_SITTING)
	{
		if (nx > 0) ani = MARIO_ANI_BIG_SITTING_RIGHT;
		else ani = MARIO_ANI_BIG_SITTING_LEFT;
	}
	else if (isJumping)
	{
		if (isHold)
		{
			RenderJumping(ani,
				MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT,
				MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT,
				MARIO_ANI_BIG_HOLD_JUMPINGDOWN_RIGHT,
				MARIO_ANI_BIG_HOLD_JUMPINGDOWN_LEFT);
		}
		else {
			if (flyTimer.IsStarted())
			{
				RenderJumping(ani,
					MARIO_ANI_BIG_FLY_RIGHT,
					MARIO_ANI_BIG_FLY_LEFT,
					MARIO_ANI_BIG_FLY_RIGHT,
					MARIO_ANI_BIG_FLY_LEFT);
			}
			else
				RenderJumping(ani,
					MARIO_ANI_BIG_JUMPINGUP_RIGHT,
					MARIO_ANI_BIG_JUMPINGUP_LEFT,
					MARIO_ANI_BIG_JUMPINGDOWN_RIGHT,
					MARIO_ANI_BIG_JUMPINGDOWN_LEFT);
		}
	}
	else {
		if (isHold) {
			RenderBasicMoving(ani,
				MARIO_ANI_BIG_HOLD_JUMPINGDOWN_RIGHT, MARIO_ANI_BIG_HOLD_JUMPINGDOWN_LEFT,
				MARIO_ANI_BIG_HOLD_IDLE_RIGHT, MARIO_ANI_BIG_HOLD_IDLE_LEFT,
				MARIO_ANI_BIG_HOLD_BRAKING_RIGHT, MARIO_ANI_BIG_HOLD_BRAKING_LEFT,
				MARIO_ANI_BIG_HOLD_WALKING_RIGHT, MARIO_ANI_BIG_HOLD_WALKING_LEFT, MARIO_ANI_BIG_KICKING_RIGHT, MARIO_ANI_BIG_KICKING_LEFT);

		}
		else {
			int ani_go_right = MARIO_ANI_BIG_WALKING_RIGHT;
			int ani_go_left = MARIO_ANI_BIG_WALKING_LEFT;
			if (runningTimer.IsStarted())
				if (abs(vx) >= MARIO_RUNNING_SPEED_MAX)
				{
					ani_go_right = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
					ani_go_left = MARIO_ANI_BIG_WALKING_FAST_LEFT;
				}
			if (abs(vx) >= MARIO_SPEED_MAX)
			{
				ani_go_right = MARIO_ANI_BIG_RUNNING_RIGHT;
				ani_go_left = MARIO_ANI_BIG_RUNNING_LEFT;
			}
			RenderBasicMoving(ani,
				MARIO_ANI_BIG_JUMPINGDOWN_RIGHT, MARIO_ANI_BIG_JUMPINGDOWN_LEFT,
				MARIO_ANI_BIG_IDLE_RIGHT, MARIO_ANI_BIG_IDLE_LEFT,
				MARIO_ANI_BIG_BRAKING_RIGHT, MARIO_ANI_BIG_BRAKING_LEFT,
				ani_go_right, ani_go_left, MARIO_ANI_BIG_KICKING_RIGHT, MARIO_ANI_BIG_KICKING_LEFT);

		}
	}
}

void CMario::TransformTanookiAni(int& ani) {
	if (state == MARIO_STATE_SITTING)
	{
		if (nx > 0) ani = MARIO_ANI_TAIL_SITTING_RIGHT;
		else ani = MARIO_ANI_TAIL_SITTING_LEFT;
	}
	else if (isJumping)
	{
		if (isHold)
		{
			RenderJumping(ani,
				MARIO_ANI_TAIL_HOLD_JUMPINGUP_RIGHT,
				MARIO_ANI_TAIL_HOLD_JUMPINGUP_LEFT,
				MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_RIGHT,
				MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_LEFT);
		}
		else {
			if (flyTimer.IsStarted())
			{
				RenderJumping(ani,
					MARIO_ANI_TAIL_FLY_UP_RIGHT,
					MARIO_ANI_TAIL_FLY_UP_LEFT,
					MARIO_ANI_TAIL_FLY_DOWN_RIGHT,
					MARIO_ANI_TAIL_FLY_DOWN_LEFT);
			}
			else
				RenderJumping(ani,
					MARIO_ANI_TAIL_FLY_UP_RIGHT,
					MARIO_ANI_TAIL_FLY_UP_LEFT,
					MARIO_ANI_TAIL_FLY_DOWN_RIGHT,
					MARIO_ANI_TAIL_FLY_DOWN_LEFT);
		}
	}
	else {
		if (isHold) {
			RenderBasicMoving(ani,
				MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_RIGHT, MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_LEFT,
				MARIO_ANI_TAIL_HOLD_IDLE_RIGHT, MARIO_ANI_TAIL_HOLD_IDLE_LEFT,
				MARIO_ANI_TAIL_HOLD_BRAKING_RIGHT, MARIO_ANI_TAIL_HOLD_BRAKING_LEFT,
				MARIO_ANI_TAIL_HOLD_WALKING_RIGHT, MARIO_ANI_TAIL_HOLD_WALKING_LEFT, MARIO_ANI_TAIL_KICKING_RIGHT, MARIO_ANI_TAIL_KICKING_LEFT);

		}
		else {
			int ani_go_right = MARIO_ANI_TAIL_WALKING_RIGHT;
			int ani_go_left = MARIO_ANI_TAIL_WALKING_LEFT;
			if (runningTimer.IsStarted())
				if (abs(vx) >= MARIO_RUNNING_SPEED_MAX)
				{
					ani_go_right = MARIO_ANI_TAIL_WALKING_FAST_RIGHT;
					ani_go_left = MARIO_ANI_TAIL_WALKING_FAST_LEFT;
				}
			if (abs(vx) >= MARIO_SPEED_MAX)
			{
				ani_go_right = MARIO_ANI_TAIL_RUNNING_RIGHT;
				ani_go_left = MARIO_ANI_TAIL_RUNNING_LEFT;
			}
			RenderBasicMoving(ani,
				MARIO_ANI_TAIL_JUMPINGDOWN_RIGHT, MARIO_ANI_TAIL_JUMPINGDOWN_LEFT,
				MARIO_ANI_TAIL_IDLE_RIGHT, MARIO_ANI_TAIL_IDLE_LEFT,
				MARIO_ANI_TAIL_BRAKING_RIGHT, MARIO_ANI_TAIL_BRAKING_LEFT,
				ani_go_right, ani_go_left, MARIO_ANI_TAIL_KICKING_RIGHT, MARIO_ANI_TAIL_KICKING_LEFT);

		}
	}

	if (tailTimer.IsStarted())
	{
		if (nx > 0)
			ani = MARIO_ANI_TAIL_TURNING_RIGHT;
		else if (nx < 0)
			ani = MARIO_ANI_TAIL_TURNING_LEFT;
	}
}

void CMario::TransformFireAni(int& ani) {
	if (state == MARIO_STATE_SITTING)
	{
		if (nx > 0) ani = MARIO_ANI_FIRE_SITTING_RIGHT;
		else ani = MARIO_ANI_FIRE_SITTING_LEFT;
	}
	else if (isJumping)
	{
		if (isHold) {
			RenderJumping(ani,
				MARIO_ANI_FIRE_HOLD_JUMPINGUP_RIGHT,
				MARIO_ANI_FIRE_HOLD_JUMPINGUP_LEFT,
				MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_RIGHT,
				MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_LEFT);
		}
		else {
			if (flyTimer.IsStarted())
			{
				RenderJumping(ani,
					MARIO_ANI_FIRE_FLY_RIGHT,
					MARIO_ANI_FIRE_FLY_LEFT,
					MARIO_ANI_FIRE_FLY_RIGHT,
					MARIO_ANI_FIRE_FLY_LEFT);
			}
			else
				RenderJumping(ani,
					MARIO_ANI_FIRE_JUMPINGUP_RIGHT,
					MARIO_ANI_FIRE_JUMPINGUP_LEFT,
					MARIO_ANI_FIRE_JUMPINGDOWN_RIGHT,
					MARIO_ANI_FIRE_JUMPINGDOWN_LEFT);
		}
	}
	else {
		if (isHold)
		{
			RenderBasicMoving(ani,
				MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_RIGHT, MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_LEFT,
				MARIO_ANI_FIRE_HOLD_IDLE_RIGHT, MARIO_ANI_FIRE_HOLD_IDLE_LEFT,
				MARIO_ANI_FIRE_HOLD_BRAKING_RIGHT, MARIO_ANI_FIRE_HOLD_BRAKING_LEFT,
				MARIO_ANI_FIRE_HOLD_WALKING_RIGHT, MARIO_ANI_FIRE_HOLD_WALKING_LEFT, MARIO_ANI_FIRE_KICKING_RIGHT, MARIO_ANI_FIRE_KICKING_LEFT);

		}
		else {
			int ani_go_right = MARIO_ANI_FIRE_WALKING_RIGHT;
			int ani_go_left = MARIO_ANI_FIRE_WALKING_LEFT;
			if (runningTimer.IsStarted())
				if (abs(vx) >= MARIO_RUNNING_SPEED_MAX)
				{
					ani_go_right = MARIO_ANI_FIRE_WALKING_FAST_RIGHT;
					ani_go_left = MARIO_ANI_FIRE_WALKING_FAST_LEFT;
				}
			if (abs(vx) >= MARIO_SPEED_MAX)
			{
				ani_go_right = MARIO_ANI_FIRE_RUNNING_RIGHT;
				ani_go_left = MARIO_ANI_FIRE_RUNNING_LEFT;
			}
			RenderBasicMoving(ani,
				MARIO_ANI_FIRE_JUMPINGDOWN_RIGHT, MARIO_ANI_FIRE_JUMPINGDOWN_LEFT,
				MARIO_ANI_FIRE_IDLE_RIGHT, MARIO_ANI_FIRE_IDLE_LEFT,
				MARIO_ANI_FIRE_BRAKING_RIGHT, MARIO_ANI_FIRE_BRAKING_LEFT,
				ani_go_right, ani_go_left, MARIO_ANI_FIRE_KICKING_RIGHT, MARIO_ANI_FIRE_KICKING_LEFT);

		}
	}
}

void CMario::RenderTanookiAni(int ani, int alpha)
{
	if (tailTimer.IsStarted())
	{
		if (nx > 0)
		{
			if (tailState == MARIO_WHACK_STATE_1 || tailState == MARIO_WHACK_STATE_5)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_RIGHT_1_ID]->Draw(x - MARIO_WHACK_RENDER_DIFF_7, y - HUD_HEIGHT, alpha);
			if (tailState == MARIO_WHACK_STATE_2)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_RIGHT_2_ID]->Draw(x, y - HUD_HEIGHT, alpha);
			if (tailState == MARIO_WHACK_STATE_3)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_RIGHT_3_ID]->Draw(x, y - HUD_HEIGHT, alpha);
			if (tailState == MARIO_WHACK_STATE_4)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_RIGHT_4_ID]->Draw(x, y - HUD_HEIGHT, alpha);
		}
		else if (nx < 0)
		{
			if (tailState == MARIO_WHACK_STATE_1 || tailState == MARIO_WHACK_STATE_5)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_LEFT_1_ID]->Draw(x + MARIO_WHACK_RENDER_DIFF_2, y - HUD_HEIGHT, alpha);
			if (tailState == MARIO_WHACK_STATE_2)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_LEFT_2_ID]->Draw(x, y - HUD_HEIGHT, alpha);
			if (tailState == MARIO_WHACK_STATE_3)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_LEFT_3_ID]->Draw(x - MARIO_WHACK_RENDER_DIFF_7, y - HUD_HEIGHT, alpha);
			if (tailState == MARIO_WHACK_STATE_4)
				CSprites::GetInstance()->sprites[MARIO_SPRITE_WHACK_LEFT_4_ID]->Draw(x, y - HUD_HEIGHT, alpha);
		}
	}
	else
	{
		animation_set->at(ani)->Render(x, y, alpha);
	}

}

void CMario::Render()
{
	int ani = -1;
	int alpha = MARIO_RENDER_ALPHA;

	if (transformTimer.IsStarted())
	{
		TransformAni(ani);
	}
	else
	{
		if (IsDead())
			ani = MARIO_ANI_DIE;
		else if (GetMode() == Mode::Small)
			TransformSmallAni(ani);
		else if (GetMode() == Mode::Super)
			TransformBigAni(ani);
		else if (GetMode() == Mode::Tanooki)
			TransformTanookiAni(ani);
		else if (GetMode() == Mode::Fire)
			TransformFireAni(ani);
	}

	if (untouchableTimer.IsStarted()) alpha = MARIO_RENDER_ALPHA / 2;

	if (pipeDownTimer.IsStarted() == true || pipeUpTimer.IsStarted() == true)
	{
		int sprite_id = MARIO_SPRITE_PIPE_SMALL;
		if (GetMode() == Mode::Super)
			sprite_id = MARIO_SPRITE_PIPE_BIG;
		if (GetMode() == Mode::Tanooki)
			sprite_id = MARIO_SPRITE_PIPE_TAIL;
		if (GetMode() == Mode::Fire)
			sprite_id = MARIO_SPRITE_PIPE_FIRE;
		CSprites::GetInstance()->sprites[sprite_id]->Draw(x, y - HUD_HEIGHT, alpha);
	}
	else if (GetMode() == Mode::Tanooki)
	{
		RenderTanookiAni(ani, alpha);
	}
	else
	{
		animation_set->at(ani)->Render(x, y, alpha);
	}
	RenderBoundingBox(100);
	if (tailTimer.IsStarted())
		tail->Render();
}

void CMario::Stop() {
	if (vx > 0)
		ax = -MARIO_ACCELERATION;
	if (vx < 0)
		ax = MARIO_ACCELERATION;
	ay = MARIO_GRAVITY;

	if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
	{
		vx = 0;
		ax = 0;
	}
}

void CMario::Jump() {
	isGround = false;
	isJumping = true;
	if (vy > -MARIO_JUMP_SPEED_MIN)
		vy = -MARIO_JUMP_SPEED_MIN;
	if (RunningStacks == MARIO_RUNNING_STACKS)
	{
		flyTimer.Start();
	}
	// increase high when jump
	ay = -MARIO_ACCELERATION_JUMP;
}

void CMario::Sit() {
	ay = MARIO_GRAVITY;
	ax = -nx * MARIO_ACCELERATION;
	if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
	{
		vx = 0;
		ax = 0;
	}
	isSitting = true;
}

void CMario::Die()
{
	vy = -MARIO_DIE_DEFLECT_SPEED;
	vx = -nx * MARIO_DIE_DEFLECT_SPEED;
	dead = true;

}

void CMario::SlowSpeed() {
	isChangeDirection = true;
}

void CMario::Attack() {
	if (GetMode() == Mode::Fire)
	{
		if (ShootTimes < MARIO_FIRE_BULLETS)
		{
			shootingTimer.Start();
			isReadyToShoot = true;
			CFireBullet* bullet = new CFireBullet(x + nx * FIRE_BULLET_BBOX_WIDTH, y + FIRE_BULLET_SHOOT_DIFF * (MARIO_LEVEL_SMALL ? MARIO_SMALL_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT));
			bullet->SetSpeed(nx * FIRE_BULLET_SPEED_X, FIRE_BULLET_SPEED_Y);
			bullet->SetTemHeight(0);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetUnit()->AddUnit(bullet, ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetGrid());
			ShootTimes++;
		}
		DebugOut(L"FIRE\n");
	}
	else if (GetMode() == Mode::Tanooki)
	{
		tailTimer.Start();
		tailStateTimer.Start();
		tailState = 1;
		DebugOut(L"TAIL\n");
	}
}


void CMario::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		ax = MARIO_ACCELERATION;
		if (ax < 0 && vy > 0)
		{
			isChangeDirection = true;
		}
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		ax = -MARIO_ACCELERATION;
		if (ax > 0 && vy > 0)
			isChangeDirection = true;
		nx = -1;
		break;
	case MARIO_STATE_JUMPING:
		Jump();
		break;
	case MARIO_STATE_IDLE:
		Stop();
		break;
	case MARIO_STATE_SITTING:
		Sit();
		break;
	case MARIO_STATE_DIE:
		Die();
		break;
	}
	if (GetMode() != Mode::Small)
	{
		if (state == MARIO_STATE_SITTING && this->state != MARIO_STATE_SITTING)
			y += 9;
		if (state != MARIO_STATE_SITTING && this->state == MARIO_STATE_SITTING)
			y -= 9;
	}
	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (GetMode() != Mode::Small)
	{
		right = left + MARIO_BIG_BBOX_WIDTH;
		bottom = top + MARIO_BIG_BBOX_HEIGHT;
		if (state == MARIO_STATE_SITTING)
		{
			bottom -= MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_BBOX_SITTING_HEIGHT;
		}
		// mario tail
	}

	else
	{
		if (transformTimer.IsStarted())
		{
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else {
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}

	}
}

void CMario::AddScore(int ox, int oy, int s, bool isEnemy, bool showscore)
{
	if (isEnemy)
	{
		if (countKillTimer.ElapsedTime() >= MARIO_KILLSTREAK_TIME)
		{
			countKill++;
			if (countKill > 3)
				countKill = 3;
		}
		else
			countKill = 0;
		countKillTimer.Start();
	}
	else
		countKill = 0;
	s = pow(2, countKill) * s;
	this->score += s;
	if (showscore)
	{
		CScore* cscore = new CScore(s);
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

		cscore->SetPosition(ox, oy);
		scene->GetUnit()->AddUnit(cscore, scene->GetGrid());
	}
}
/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	Transform(Mode::Small);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	dead = false;
}

void CMario::TelePort() {
	if (nx > 0)
		SetPosition(x + 1300, start_y);
	else
		SetPosition(x - 1300, start_y);
}
