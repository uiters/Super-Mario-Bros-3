#pragma once
#include "GameObject.h"
#define ABYSS_BBOX_WIDTH    16
#define ABYSS_BBOX_HEIGHT   32
#define ABYSS_ACTIVE_RANGE  2
class CAbyss :
    public CGameObject
{
public:
    CAbyss::CAbyss();
    void Render() { RenderBoundingBox(255); };
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
};