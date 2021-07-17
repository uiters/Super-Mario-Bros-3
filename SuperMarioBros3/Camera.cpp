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
	int sceneId = game->GetCurrentScene()->GetId();
	HUD* hud = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetHUD();
	CMap* currentMap = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetMap();
	CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mapHeight = currentMap->GetMapHeight();
	mapWidth = currentMap->GetMapWidth();
	player->GetPosition(cx, cy);
	D3DXVECTOR3 pos = GetCameraPosition();

	bool isDoneGame = ((CPlayScene*)game->GetCurrentScene())->isGameDone3;

	// camera moving
	if (!isDoneGame)
	{
		//limit X
		if (cx >= mapWidth - MARIO_BIG_BBOX_WIDTH)//Right edge
			player->x = mapWidth - MARIO_BIG_BBOX_WIDTH;
		else if (cx <= 0)//Left edge
			player->x = 0;
		//limit Y
		if (cy >= mapHeight)
			player->y = mapHeight + 100;
		else if (cy <= 20)
			player->y = 20;

		if (sceneId == EXTRA_MAP1_3_1)
		{
			if (cx - 1 <= pos.x)
			{
				player->SetState(MARIO_STATE_WALKING_RIGHT);
				player->vx = 0.1f;
				player->nx = 1;
			}
			if (cx >= pos.x + SCREEN_WIDTH - 32)
			{
				player->x = pos.x + SCREEN_WIDTH - 32;
			}
		}
	}

	if (typeCamera && sceneId == EXTRA_MAP1_3_1)
	{
		if (pos.x > mapWidth - SCREEN_WIDTH - 1)
			return;
		countx = countx + CAMERA_SPEED_X * dt;
		cx = countx;
		/*cy = 200;*/
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
		SetCameraPosition((int)countx, cy);
		hud->SetPosition(int(cx), (int)cy + 187);
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
			if (cy < SCREEN_HEIGHT - 48)
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
	if (sceneId == EXTRA_MAP1_1_1)
	{
		SetCameraPosition((int)cx, (int)cy - 30);
		hud->SetPosition(int(cx), HUD_CY / 2 - 40);
	}
	else if (sceneId == WORLD_SCENE)
	{
		SetCameraPosition(0, -HUD_HEIGHT - 6);
	}
	else
	{
		SetCameraPosition((int)cx, (int)cy);
		hud->SetPosition(int(cx), HUD_CY);
		if (cy == 0)
		{
			hud->SetPosition(int(cx), HUD_CY / 2 - 10);
		}
	}
}

bool Camera::isAreaCamera(float x, float y) {
	CGame* game = CGame::GetInstance();

	bool xInViewPort = x >= GetCameraPosition().x - 32 && x < GetCameraPosition().x + game->GetScreenWidth();
	bool yInViewPort = y >= GetCameraPosition().y - (SCREEN_HEIGHT - game->GetScreenHeight()) && y < GetCameraPosition().y + SCREEN_HEIGHT;
	return xInViewPort && yInViewPort;
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




