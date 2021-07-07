#pragma once
#include"Mario.h"
#include "Define.h"
class CBackUp
{
public:
	static CBackUp* __instance;
	int life = MARIO_DEFAULT_LIFES;
	int money = 0;
	DWORD time = 0;
	int score = 0;
	CMario::Mode mode = CMario::Mode::Small;
	int scene = 0;
	vector<int>cards;
	static CBackUp* GetInstance();
	void BackUpMario(CMario* mario);
	void LoadBackUp(CMario* mario);
};
