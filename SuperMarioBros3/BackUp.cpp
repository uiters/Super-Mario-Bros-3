#include "BackUp.h"
#include "PlayScene.h"


CBackUp* CBackUp::__instance = NULL;
CBackUp* CBackUp::GetInstance()
{
	if (__instance == NULL) __instance = new CBackUp();
	return __instance;
}

void CBackUp::BackUpMario(CMario* mario)
{
	if (mario != NULL)
	{
		life = mario->life;
		score = mario->score;
		mode = mario->GetMode();
		money = mario->money;
		scene = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (scene == 2 || scene == 5)
			scene--;
	}

}

void CBackUp::LoadBackUp(CMario* mario)
{
	if (mario != NULL)
	{
		mario->life = life;
		mario->score = score;
		mario->Transform(mode);
		mario->money = money;
	}

}
