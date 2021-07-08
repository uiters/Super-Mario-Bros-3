#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "Portal.h"
#include "Tanooki.h"
#include "Fire.h"
#include "Utils.h"

#define MARIO_WALKING_SPEED_START	0.0001f 
#define MARIO_WALKING_SPEED_MAX		0.15f
#define MARIO_RUNNING_SPEED_MAX		0.2f
#define MARIO_SPEED_MAX				0.25f
#define MARIO_ACCELERATION			0.0003f
#define MARIO_FRICTION				0.0005f	
#define MARIO_WALKING_SPEED_MIN		0.05f

#define MARIO_ACCELERATION_JUMP		0.0005f
#define MARIO_JUMP_SPEED_MAX		0.43f
#define MARIO_JUMP_SPEED_MIN		0.4f
#define MARIO_SLOW_FALLING_SPEED	0.05f
#define MARIO_FLY_SPEED				0.135f
#define MARIO_JUMP_DEFLECT_SPEED	0.3f
#define MARIO_JUMP_DEFLECT_INTRO	0.35f
#define MARIO_GRAVITY				0.002f
#define MARIO_DIE_DEFLECT_SPEED		0.5f
#define MARIO_RUN_SPEED_MAX			0.3f
#define MARIO_BULLET_MAX			20

#define MARIO_UNTOUCHABLE_TIME		1000
#define MARIO_TURNING_TAIL_TIME		350
#define MARIO_TURNING_STATE_TIME	70
#define MARIO_SHOOTING_TIME			150
#define MARIO_KICKING_TIME			200	
#define MARIO_FLAPPING_TIME			200	
#define MARIO_RUNNING_STACK_TIME	200
#define MARIO_SLOW_TIME				1000
#define MARIO_SLOW_STACK_TIME		250
#define MARIO_RELOAD_BULLET_TIME	500
#define MARIO_TAIL_FLYING_TIME		250
#define MARIO_FLYING_TIME			1750
#define MARIO_TRANSFORMING_TIME		500
#define MARIO_KILLSTREAK_TIME		1000
#define MARIO_PIPE_TIME				1000
#define MARIO_GAMEDONE_TIME			3000


#define MARIO_RUNNING_STACKS		7
#define MARIO_WALKING_FAST_STACKS	4
#define MARIO_TURNING_STACKS		5

#define MARIO_DEFAULT_LIFES			4

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	111
#define MARIO_STATE_JUMPING			200
#define MARIO_STATE_SITTING			201
#define MARIO_STATE_HOLDING			444
#define MARIO_STATE_KICK			411
#define MARIO_STATE_SHOOTING		422
#define MARIO_STATE_TURNING			433

#define MARIO_STATE_DIE				900

// SMALL
#define MARIO_ANI_SMALL_IDLE_RIGHT			0
#define MARIO_ANI_SMALL_WALKING_RIGHT		1
#define MARIO_ANI_SMALL_WALKING_FAST_RIGHT	2
#define MARIO_ANI_SMALL_RUNNING_RIGHT		3
#define MARIO_ANI_SMALL_JUMPINGUP_RIGHT		4
#define MARIO_ANI_SMALL_JUMPINGDOWN_RIGHT	5
#define MARIO_ANI_SMALL_BRAKING_RIGHT		6

#define MARIO_ANI_SMALL_IDLE_LEFT			7
#define MARIO_ANI_SMALL_WALKING_LEFT		8
#define MARIO_ANI_SMALL_WALKING_FAST_LEFT	9
#define MARIO_ANI_SMALL_RUNNING_LEFT		10
#define MARIO_ANI_SMALL_JUMPINGUP_LEFT		11
#define MARIO_ANI_SMALL_JUMPINGDOWN_LEFT	12
#define MARIO_ANI_SMALL_BRAKING_LEFT		13

//BIG
#define MARIO_ANI_BIG_IDLE_RIGHT			14
#define MARIO_ANI_BIG_WALKING_RIGHT			15
#define MARIO_ANI_BIG_WALKING_FAST_RIGHT	16
#define MARIO_ANI_BIG_RUNNING_RIGHT			17
#define MARIO_ANI_BIG_JUMPINGUP_RIGHT		18
#define MARIO_ANI_BIG_JUMPINGDOWN_RIGHT		19
#define MARIO_ANI_BIG_BRAKING_RIGHT			20
#define MARIO_ANI_BIG_SITTING_RIGHT			21

#define MARIO_ANI_BIG_IDLE_LEFT				22
#define MARIO_ANI_BIG_WALKING_LEFT			23
#define MARIO_ANI_BIG_WALKING_FAST_LEFT		24
#define MARIO_ANI_BIG_RUNNING_LEFT			25
#define MARIO_ANI_BIG_JUMPINGUP_LEFT		26
#define MARIO_ANI_BIG_JUMPINGDOWN_LEFT		27
#define MARIO_ANI_BIG_BRAKING_LEFT			28
#define MARIO_ANI_BIG_SITTING_LEFT			29

//TAIL
#define MARIO_ANI_TAIL_IDLE_RIGHT			30
#define MARIO_ANI_TAIL_WALKING_RIGHT		31
#define MARIO_ANI_TAIL_WALKING_FAST_RIGHT	32
#define MARIO_ANI_TAIL_RUNNING_RIGHT		33
#define MARIO_ANI_TAIL_JUMPINGUP_RIGHT		34
#define MARIO_ANI_TAIL_JUMPINGDOWN_RIGHT	35
#define MARIO_ANI_TAIL_BRAKING_RIGHT		36
#define MARIO_ANI_TAIL_SITTING_RIGHT		37

#define MARIO_ANI_TAIL_IDLE_LEFT			38
#define MARIO_ANI_TAIL_WALKING_LEFT			39
#define MARIO_ANI_TAIL_WALKING_FAST_LEFT	40
#define MARIO_ANI_TAIL_RUNNING_LEFT			41
#define MARIO_ANI_TAIL_JUMPINGUP_LEFT		42
#define MARIO_ANI_TAIL_JUMPINGDOWN_LEFT		43
#define MARIO_ANI_TAIL_BRAKING_LEFT			44
#define MARIO_ANI_TAIL_SITTING_LEFT			45

//FIRE
#define MARIO_ANI_FIRE_IDLE_RIGHT			46
#define MARIO_ANI_FIRE_WALKING_RIGHT		47
#define MARIO_ANI_FIRE_WALKING_FAST_RIGHT	48
#define MARIO_ANI_FIRE_RUNNING_RIGHT		49
#define MARIO_ANI_FIRE_JUMPINGUP_RIGHT		50
#define MARIO_ANI_FIRE_JUMPINGDOWN_RIGHT	51
#define MARIO_ANI_FIRE_BRAKING_RIGHT		52
#define MARIO_ANI_FIRE_SITTING_RIGHT		53

#define MARIO_ANI_FIRE_IDLE_LEFT			54
#define MARIO_ANI_FIRE_WALKING_LEFT			55
#define MARIO_ANI_FIRE_WALKING_FAST_LEFT	56
#define MARIO_ANI_FIRE_RUNNING_LEFT			57
#define MARIO_ANI_FIRE_JUMPINGUP_LEFT		58
#define MARIO_ANI_FIRE_JUMPINGDOWN_LEFT		59
#define MARIO_ANI_FIRE_BRAKING_LEFT			60
#define MARIO_ANI_FIRE_SITTING_LEFT			61


#define MARIO_ANI_DIE	62
//SMALL
#define MARIO_ANI_SMALL_HOLD_IDLE_RIGHT			63
#define MARIO_ANI_SMALL_HOLD_WALKING_RIGHT		64
#define MARIO_ANI_SMALL_HOLD_WALKING_FAST_RIGHT	64
#define MARIO_ANI_SMALL_HOLD_RUNNING_RIGHT		64
#define MARIO_ANI_SMALL_HOLD_JUMPINGUP_RIGHT	65
#define MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_RIGHT	65
#define MARIO_ANI_SMALL_HOLD_BRAKING_RIGHT		64
#define MARIO_ANI_SMALL_KICKING_RIGHT			66

#define MARIO_ANI_SMALL_HOLD_IDLE_LEFT			67
#define MARIO_ANI_SMALL_HOLD_WALKING_LEFT		68
#define MARIO_ANI_SMALL_HOLD_WALKING_FAST_LEFT	68
#define MARIO_ANI_SMALL_HOLD_RUNNING_LEFT		68
#define MARIO_ANI_SMALL_HOLD_JUMPINGUP_LEFT		69
#define MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_LEFT	69
#define MARIO_ANI_SMALL_HOLD_BRAKING_LEFT		68
#define MARIO_ANI_SMALL_KICKING_LEFT			70

//BIG
#define MARIO_ANI_BIG_HOLD_IDLE_RIGHT			71
#define MARIO_ANI_BIG_HOLD_WALKING_RIGHT		72
#define MARIO_ANI_BIG_HOLD_WALKING_FAST_RIGHT	72
#define MARIO_ANI_BIG_HOLD_RUNNING_RIGHT		72
#define MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT		73
#define MARIO_ANI_BIG_HOLD_JUMPINGDOWN_RIGHT	73
#define MARIO_ANI_BIG_HOLD_BRAKING_RIGHT		72
#define MARIO_ANI_BIG_KICKING_RIGHT				74

#define MARIO_ANI_BIG_HOLD_IDLE_LEFT			75
#define MARIO_ANI_BIG_HOLD_WALKING_LEFT			76
#define MARIO_ANI_BIG_HOLD_WALKING_FAST_LEFT	76
#define MARIO_ANI_BIG_HOLD_RUNNING_LEFT			76
#define MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT		77
#define MARIO_ANI_BIG_HOLD_JUMPINGDOWN_LEFT		77
#define MARIO_ANI_BIG_HOLD_BRAKING_LEFT			76
#define MARIO_ANI_BIG_KICKING_LEFT				78

//TAIL
#define MARIO_ANI_TAIL_HOLD_IDLE_RIGHT			79
#define MARIO_ANI_TAIL_HOLD_WALKING_RIGHT		80
#define MARIO_ANI_TAIL_HOLD_WALKING_FAST_RIGHT	80
#define MARIO_ANI_TAIL_HOLD_RUNNING_RIGHT		80
#define MARIO_ANI_TAIL_HOLD_JUMPINGUP_RIGHT		81
#define MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_RIGHT	81
#define MARIO_ANI_TAIL_HOLD_BRAKING_RIGHT		80
#define MARIO_ANI_TAIL_KICKING_RIGHT			82

#define MARIO_ANI_TAIL_HOLD_IDLE_LEFT			83
#define MARIO_ANI_TAIL_HOLD_WALKING_LEFT		84
#define MARIO_ANI_TAIL_HOLD_WALKING_FAST_LEFT	84
#define MARIO_ANI_TAIL_HOLD_RUNNING_LEFT		84
#define MARIO_ANI_TAIL_HOLD_JUMPINGUP_LEFT		85
#define MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_LEFT	85
#define MARIO_ANI_TAIL_HOLD_BRAKING_LEFT		84
#define MARIO_ANI_TAIL_KICKING_LEFT				86

//FIRE
#define MARIO_ANI_FIRE_HOLD_IDLE_RIGHT			87
#define MARIO_ANI_FIRE_HOLD_WALKING_RIGHT		88
#define MARIO_ANI_FIRE_HOLD_WALKING_FAST_RIGHT	88
#define MARIO_ANI_FIRE_HOLD_RUNNING_RIGHT		88
#define MARIO_ANI_FIRE_HOLD_JUMPINGUP_RIGHT		89
#define MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_RIGHT	89
#define MARIO_ANI_FIRE_HOLD_BRAKING_RIGHT		88
#define MARIO_ANI_FIRE_KICKING_RIGHT			90

#define MARIO_ANI_FIRE_HOLD_IDLE_LEFT			91
#define MARIO_ANI_FIRE_HOLD_WALKING_LEFT		92
#define MARIO_ANI_FIRE_HOLD_WALKING_FAST_LEFT	92
#define MARIO_ANI_FIRE_HOLD_RUNNING_LEFT		92
#define MARIO_ANI_FIRE_HOLD_JUMPINGUP_LEFT		93
#define MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_LEFT	93
#define MARIO_ANI_FIRE_HOLD_BRAKING_LEFT		92
#define MARIO_ANI_FIRE_KICKING_LEFT				94

//TAIL
#define MARIO_ANI_TAIL_TURNING_RIGHT			95
#define MARIO_ANI_TAIL_TURNING_LEFT				96
#define MARIO_ANI_TAIL_FLAPPING_RIGHT			97
#define MARIO_ANI_TAIL_FLAPPING_LEFT			98
//FIRE
#define MARIO_ANI_SHOOTING_RIGHT				99
#define MARIO_ANI_SHOOTING_LEFT					100
#define MARIO_ANI_SHOOTING_JUMP_RIGHT			101
#define MARIO_ANI_SHOOTING_JUMP_LEFT			102
//FLY
#define MARIO_ANI_SMALL_FLY_RIGHT				103
#define MARIO_ANI_SMALL_FLY_LEFT				104
#define MARIO_ANI_BIG_FLY_RIGHT					105
#define MARIO_ANI_BIG_FLY_LEFT					106
#define MARIO_ANI_TAIL_FLY_UP_RIGHT				107
#define MARIO_ANI_TAIL_FLY_DOWN_RIGHT			108
#define MARIO_ANI_TAIL_FLY_FLAPPING_RIGHT		109
#define MARIO_ANI_TAIL_FLY_UP_LEFT				110
#define MARIO_ANI_TAIL_FLY_DOWN_LEFT			111
#define MARIO_ANI_TAIL_FLY_FLAPPING_LEFT		112
#define MARIO_ANI_FIRE_FLY_RIGHT				113
#define MARIO_ANI_FIRE_FLY_LEFT					114

#define MARIO_ANI_TRANSFORM_SMALL_RIGHT			115
#define MARIO_ANI_TRANSFORM_SMALL_LEFT			116
#define MARIO_ANI_TRANSFORM_BANG				117

#define MARIO_SPRITE_PIPE_SMALL					10900
#define MARIO_SPRITE_PIPE_BIG					11900
#define MARIO_SPRITE_PIPE_TAIL					12900
#define MARIO_SPRITE_PIPE_FIRE					13900

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_TAIL	3
#define	MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH			15
#define MARIO_BIG_BBOX_HEIGHT			27
#define MARIO_BIG_BBOX_SITTING_HEIGHT	18
#define MARIO_SMALL_BBOX_WIDTH			13
#define MARIO_SMALL_BBOX_HEIGHT			16

// whack tail
#define MARIO_SPRITE_WHACK_LEFT_1_ID	12813
#define MARIO_SPRITE_WHACK_LEFT_2_ID	12814
#define MARIO_SPRITE_WHACK_LEFT_3_ID	12815
#define MARIO_SPRITE_WHACK_LEFT_4_ID	12816

#define MARIO_SPRITE_WHACK_RIGHT_1_ID	12803
#define MARIO_SPRITE_WHACK_RIGHT_2_ID	12804
#define MARIO_SPRITE_WHACK_RIGHT_3_ID	12805
#define MARIO_SPRITE_WHACK_RIGHT_4_ID	12806

#define MARIO_WHACK_STATE_1		1
#define MARIO_WHACK_STATE_2		2
#define MARIO_WHACK_STATE_3		3
#define MARIO_WHACK_STATE_4		4
#define MARIO_WHACK_STATE_5		5

#define MARIO_WHACK_RENDER_DIFF_2	2 
#define MARIO_WHACK_RENDER_DIFF_7	7 
//others
#define MARIO_RENDER_ALPHA				255
#define MARIO_FIRE_BULLETS				2
#define MARIO_SITTING_RENDER_DIFF_5		5 

#define MARIO	0
#define LUIGI	1

class CMario : public CGameObject
{
public:
	enum class Mode
	{
		Small,
		Super,
		Fire,
		Tanooki,
	};
private:
	Mode mode;
	Mode prevMode;
	Timer transformTimer;

	//Boost speed and fly
	Timer flyTimer;
	Timer runningTimer;
	Timer stoppingTimer;

	Timer dieTimer;

	// killing
	Timer countKillTimer;
	int countKill;

	//skill
	CTanooki* tail;
public:
	Timer untouchableTimer;
	float start_x;			// initial position of Mario at scene
	float start_y;

	bool dead;
	bool isSitting;
	bool isGround;
	bool isJumping;
	bool isJumpMusicBrick;
	//boost speed
	bool isReadyToRun;
	bool isChangeDirection = false;
	//action with koopas
	Timer kickTimer;
	bool isHold;
	bool isReadyToHold;
	//game done
	bool isGameDone = false;
	// HUD
	int RunningStacks = 0;
	int money = 0;
	int life = MARIO_DEFAULT_LIFES;
	int score = 0;
	//  tail
	int tailState = 0;
	Timer tailTimer;
	Timer tailStateTimer;
	//shoot
	Timer shootingTimer;
	int ShootTimes = 0;
	bool isReadyToShoot;
	// swap map
	bool wannaTele = false;
	Timer pipeUpTimer;
	Timer pipeDownTimer;
	CPortal* portal = NULL;

	CMario(float x = 0.0f, float y = 0.0f);
	~CMario();

	bool IsDead() { return dead; }
	bool IsLostControl() { return transformTimer.IsStarted(); }
	bool isPipe() { return pipeUpTimer.IsStarted() || pipeDownTimer.IsStarted(); }
	//
	void RunTimer();
	// transform 
	Mode GetMode() { return mode; }
	void Transform(Mode form);
	//render
	void Attacked();
	void LimitSpeed();
	void RenderBasicMoving(int& ani,
		int ani_idle_right, int ani_idle_left, int ani_jump_down_right, int ani_jump_down_left,
		int ani_baking_right, int ani_baking_left, int ani_walking_right, int ani_walking_left, int ani_kicking_right, int ani_kicking_left);
	void RenderJumping(int& ani, int ani_jump_up_right, int ani_jump_up_left, int ani_jump_down_right, int ani_jump_down_left);
	void TransformAni(int& ani);
	void TransformSmallAni(int& ani);
	void TransformBigAni(int& ani);
	void TransformTanookiAni(int& ani);
	void TransformFireAni(int& ani);
	void RenderTanookiAni(int ani, int alpha);
	void Render();

	//set state
	void Stop();
	void Jump();
	void Sit();
	void Die();
	void SlowSpeed();
	void Reset();
	void Attack();
	void SetState(int vState);
	void SetIsJumpOnMusicBrick(bool t);
	void Update(DWORD dt, std::vector<LPGAMEOBJECT>* objects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	CTanooki* getTail() { return tail; }
	void AddLife(int l = 1) { this->life += l; }
	void AddMoney(int m = 1) { this->money += m; }
	void AddScore(int ox, int oy, int s = 100, bool isEnemy = false, bool showScore = true);
	void TelePort();
};