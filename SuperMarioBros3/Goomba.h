#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Timer.h"

#define GOOMBA_WALKING_SPEED		0.035f
#define GOOMBA_GRAVITY				0.002f
#define GOOMBA_DIE_DEFLECT_SPEED	0.35f
#define GOOMBA_JUMP_SPEED			0.125f
#define GOOMBA_HIGHJUMP_SPEED		0.25f


#define GOOMBA_RED_TIME_CHASING			7000
#define GOOMBA_RED_TIME_WALKING			500
#define GOOMBA_RED_TIME_HIGHJUMPING		500
#define GOOMBA_RED_TIME_JUMPING			125
#define GOOMBA_TIME_DIYING				250
#define GOOMBA_TIME_DIYING_BY_TAIL		1000
// SUPER GOOMBA
#define GOOMBA_BIG_TIME_WALKING		1000
#define GOOMBA_BIG_TIME_READY_FLY	500
#define GOOMBA_BIG_TIME_DROP	1000

#define GOOMBA_RED_JUMPING_STACKS	3

#define GOOMBA_RED_RANGE_CHASING		50

#define GOOMBA_NORMAL_BBOX_WIDTH		16
#define GOOMBA_RED_BBOX_WIDTH			20
#define GOOMBA_NORMAL_BBOX_HEIGHT		15
#define GOOMBA_RED_BBOX_HEIGHT			17
#define GOOMBA_RED_BBOX_WINGS_HEIGHT	22
// SUPER GOOMBA
#define GOOMBA_BIG_BBOX_WIDTH			20
#define GOOMBA_BIG_BBOX_HEIGHT			19
#define GOOMBA_BIG_BBOX_WINGS_HEIGHT	24

#define GOOMBA_BBOX_HEIGHT_DIE			9

#define GOOMBA_STATE_WALKING			100
#define GOOMBA_STATE_DIE				200
#define GOOMBA_STATE_DIE_BY_MARIO		300
#define GOOMBA_STATE_RED_JUMPING		400
#define GOOMBA_STATE_RED_HIGHJUMPING	450
#define GOOMBA_STATE_RED_WINGSWALKING	500
#define GOOMBA_STATE_YELLOW_WINGSWALKING 600
// SUPER GOOMBA
#define GOOMBA_SUPER_STATE_FLYING		550
#define GOOMBA_SUPER_STATE_WALKING		600
#define GOOMBA_STATE_WINGSJUMPING		650

#define GOOMBA_NORMAL_ANI_WALKING		0
#define GOOMBA_NORMAL_ANI_DIE			1
#define GOOMBA_RED_ANI_JUMPING			2
#define GOOMBA_RED_ANI_WINGSWALKING		3
#define GOOMBA_RED_ANI_WALKING			4
#define GOOMBA_RED_ANI_DIE				5
// SUPER GOOMBA
#define GOOMBA_ANI_FLY	6
#define GOOMBA_ANI_WALK	7
#define GOOMBA_ANI_WING_WALK	8

#define GOOMBA_ANISET_ID	2
#define MAX_TINY_GOOMBA		4

#define GOOMBA_NORMAL			0
#define GOOMBA_RED				1
#define GOOMBA_RED_NORMAL		2
#define GOOMBA_SUPER			3

class CGoomba : public CGameObject
{
	Timer dyingTimer;
	Timer chasingTimer;
	Timer walkingTimer;

	bool isDying = false;
	bool isWhackedDying = false;
	bool isJumping = false;
	bool isHighJumping = false;
	int jumpingStacks = 0;
	// super goomba
	Timer flyingTimer;
	Timer goombaWalkingTimer;
	Timer droppingTimer;
	int createTinyGoomba = 0;

public:
	CGoomba(int tag);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void StartDying(bool l = false) { dyingTimer.Start(); if (l) isWhackedDying = true; else isDying = true; }
	void HandleBigGoomba();
	void SetSpeedDirection(float b);
	void CreatePoopGoomba();
};