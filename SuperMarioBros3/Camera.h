#pragma once
#include <d3dx9.h>
#include <vector>
#include "GameObject.h"

#define CAMERA_WIDTH 280
#define CAMERA_HEIGHT 160

#define CAMERA_SPEED_X	0.05f

class Camera
{
private:
	D3DXVECTOR3 cameraPosition;
	int width, height;
	static Camera* __instance;
public:
	bool block = false;
	Camera();
	static Camera* GetInstance();
	D3DXVECTOR3 GetPositionInCamera(D3DXVECTOR3 position);
	void SetCameraPosition(float x, float y);
	void Setwidthheight(int x, int y);
	D3DXVECTOR3 GetCameraPosition();
	void Update(DWORD dt, int typeCamera, float& countx);
	void Unload();
	~Camera();
};

