#pragma once
#include <d3d9.h>
#include"Textures.h"
#include "Sprites.h"
#include "Utils.h"
#include "Camera.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
class CMap
{
	Camera* cam;
	int TotalRowsOfMap, TotalColumnsOfMap;
	int TotalRowsOfTileSet, TotalColumnsOfTileSet;
	int TotalTiles;	//total tiles of tile set
	LPDIRECT3DTEXTURE9 TileSet; //sceneX_bank.png
	vector<LPSPRITE> Tiles;
	int** TileMap;
public:
	CMap(int TileSetID, int TotalRowsOfMap, int TotalColumnsOfMap, int TotalRowsOfTileSet, int  TotalColumnsOfTileSet, int TotalTiles);
	~CMap();
	void Render();
	void SetTileMapData(int** TileMapData);
	void ExtractTileFromTileSet();
	int GetMapWidth();
	int GetMapHeight();
};

