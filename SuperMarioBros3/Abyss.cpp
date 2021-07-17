#include "Abyss.h"
#include "Koopas.h"
#include "PlayScene.h"
CAbyss::CAbyss()
{
	type = IGNORE_DEFINE;
}
void CAbyss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float mLeft, mTop, mRight, mBottom;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT object = coObjects->at(i);
		if (dynamic_cast<CKoopas*>(object))
		{
			object->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && object->x >= x - ABYSS_ACTIVE_RANGE)
			{
				if (object->state == KOOPAS_STATE_SPINNING)
				{
					object->x = x + PUSHBACK;
					object->y++;
				}
				else if (object->state == KOOPAS_STATE_WALKING)
				{
					object->x -= KOOPAS_BBOX_WIDTH;
					object->nx = -1;
					object->vx = -KOOPAS_WALKING_SPEED;
				}
			}
		}
	}
};
void CAbyss::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ABYSS_BBOX_WIDTH;
	b = y + ABYSS_BBOX_HEIGHT;
};