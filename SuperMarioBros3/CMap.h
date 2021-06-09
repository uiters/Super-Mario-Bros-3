#pragma once
#include "Sprites.h"
#include <d3d9.h>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
class CMap
{
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
