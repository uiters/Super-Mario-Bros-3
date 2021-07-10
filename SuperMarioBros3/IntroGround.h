#pragma once
#include "Brick.h"

#define GROUND_BBOX_WIDTH  300
#define GROUND_BBOX_HEIGHT 16

class CIntroGround :
    public CBrick
{
    void GetBoundingBox(float& l, float& t, float& r, float& b);
};

