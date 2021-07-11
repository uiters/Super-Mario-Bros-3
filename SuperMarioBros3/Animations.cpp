#include "Animations.h"
#include "Utils.h"
#include "Game.h"
#include "IntroScene.h"
#include "Define.h"

CAnimationSets* CAnimationSets::__instance = NULL;

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Animation ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
	DebugOut(L"[INFO] Animation ID %d added!\n", spriteId);
}

// NOTE: sometimes Animation object is NULL ??? HOW ??? 
void CAnimation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
				currentFrame = 0;
		}
	}
	if (!dynamic_cast<CIntroScene*>(CGame::GetInstance()->GetCurrentScene()))
		frames[currentFrame]->GetSprite()->Draw(x, y - HUD_HEIGHT, alpha);
	else
		frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}
	animations.clear();
}

CAnimationSets::CAnimationSets()
{

}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}

LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
	DebugOut(L"[INFO] AnimationSet ID %d added!\n", id);
}
