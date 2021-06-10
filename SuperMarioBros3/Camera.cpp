#include "Camera.h"
#include "Game.h"
#include "PlayScene.h"
#include "define.h"
#include "Utils.h"

Camera* Camera::__instance = NULL;

Camera::Camera()
{
	cameraPosition = D3DXVECTOR3(0, 0, 0);
	Setwidthheight(CAMERA_WIDTH, CAMERA_HEIGHT);
}

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

D3DXVECTOR3 Camera::GetPositionInCamera(D3DXVECTOR3 position)
{
	D3DXVECTOR3 pos;
	pos.x = position.x - this->cameraPosition.x;
	pos.y = position.y - this->cameraPosition.y;
	return D3DXVECTOR3(pos.x, pos.y, 0);
}

void Camera::SetCameraPosition(float x, float y)
{
	if (!block)
	{
		this->cameraPosition.x = x;
		this->cameraPosition.y = y;
	}
}

void Camera::Setwidthheight(int x, int y)
{
	this->width = x;
	this->height = y;
}

D3DXVECTOR3 Camera::GetCameraPosition()
{
	return this->cameraPosition;
}

void Camera::Update(DWORD dt, int typeCamera, float& countx)
{
	CGame* game = CGame::GetInstance();
	// Update camera to follow mario
	float cx, cy, mapHeight, mapWidth;
	CMap* currentMap = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetMap();
	CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mapHeight = currentMap->GetMapHeight();
	mapWidth = currentMap->GetMapWidth();
	player->GetPosition(cx, cy);
	if (false)
	{
		if (player->nx > 0)
			cx -= 8;
		else if (player->nx < 0)
			cx += 8;
	}
	D3DXVECTOR3 pos = GetCameraPosition();

	if (typeCamera)
	{
		if (pos.x > mapWidth - SCREEN_WIDTH - 1)
			return;
		countx = countx + CAMERA_SPEED_X * dt;
		cx = countx;
		cy = 0;
		SetCameraPosition((int)countx, 0);
		return;
	}
	else
	{
		if (mapWidth > SCREEN_WIDTH) {
			if (cx + 5 < SCREEN_WIDTH / 2) {
				cx = pos.x;
			}
			else if (cx + SCREEN_WIDTH / 2 > mapWidth - 1) {
				cx = mapWidth - SCREEN_WIDTH;
			}
			else {
				cx = cx + 5 + SCREEN_WIDTH / 2 - SCREEN_WIDTH;
			}
		}
		else {
			cx = 0;
		}

		if (mapHeight > SCREEN_HEIGHT)
		{
			if (cy < SCREEN_HEIGHT - 32)
			{
				cy = 0;
			}
			else if (cy > mapHeight - SCREEN_HEIGHT)
			{
				cy = mapHeight - SCREEN_HEIGHT + 32;
			}
			else //if (cy < mapHeight - SCREEN_HEIGHT)
			{
				cy = cy - SCREEN_HEIGHT / 2 + 32;
			}
		}
		else
		{
			cy = mapHeight - SCREEN_HEIGHT;
		}
		if (cy < 0) cy = 0;
		//cy -= SCREEN_HEIGHT / 2;
	}
	SetCameraPosition((int)cx, (int)cy);
}

void Camera::Unload()
{
	if (__instance == NULL) return;
	else
	{
		__instance = NULL;
	}
}

Camera::~Camera()
{

}




