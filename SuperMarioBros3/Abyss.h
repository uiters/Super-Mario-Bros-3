#include "GameObject.h"

class CAbyss :
    public CGameObject
{
public:
    CAbyss::CAbyss() {};
    void Render() { RenderBoundingBox(255); };
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
    void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};