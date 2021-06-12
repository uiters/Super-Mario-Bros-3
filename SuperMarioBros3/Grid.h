#pragma once
// Reference : https://gameprogrammingpatterns.com/spatial-partition.html#intent
#include "GameObject.h"
#include <unordered_set>
#include "Camera.h"

#define CELL_WIDTH		136
#define CELL_HEIGHT		128
class Unit
{
	friend class Grid;
private:
	float x;
	float y;

	Grid* grid;
	LPGAMEOBJECT obj;

	Unit* prev;
	Unit* next;
public:
	Unit(Grid* grid, LPGAMEOBJECT obj, float x, float y);
	Unit(Grid* grid, LPGAMEOBJECT obj, int gridRow, int gridCol);

	void Add(LPGAMEOBJECT obj);
	void Move(float x, float y);
	LPGAMEOBJECT GetObj() { return this->obj; }

};

class Grid
{
	int mapWidth;
	int mapHeight;


	int numCols;
	int numRows;

	vector <vector<Unit*>> cells;
public:
	Grid(int mapWidth, int mapHeight, int cellWidth, int cellHeight);
	Grid(int gridCols, int gridRows);
	~Grid();

	void Add(Unit* unit);
	void Add(Unit* unit, int gridRow, int gridCol);
	void Move(Unit* unit, float x, float y);
	void Get(Camera* cam, vector<Unit*>& listUnits);
	void UpdateGrid(vector<Unit*> &units);
	void Out();
	void ClearAll();
};
