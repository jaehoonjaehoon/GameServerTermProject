#pragma once

class Object;

class Board
{
	POINT tile[TILE_WIDTH][TILE_HEIGHT];

	Object* player;

public:
	void SetPlayer(Object* p) { player = p; }

	void SetTile(const POINT t[TILE_WIDTH][TILE_HEIGHT])
	{
		for (int i = 0; i < TILE_WIDTH; ++i)
		{
			for (int j = 0; j < TILE_HEIGHT; ++j)
			{
				tile[i][j] = t[i][j];
			}
		}
	}

public:
	void Initialize();
	void Render(const HDC& hdc);

public:
	Board();
	~Board();
};

