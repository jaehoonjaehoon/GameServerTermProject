#include "stdafx.h"
#include "Board.h"

#include "ObjMgr.h"
#include "Object.h"

Board::Board()
{
	Initialize();
}

Board::~Board()
{
}

void Board::Initialize()
{
}

void Board::Render(const HDC& hdc)
{
	HBRUSH brush, old_brush;

	int size = (TILE_SIZE >> 1);

	int cull_range = 10;

	for (int i = 0; i < TILE_WIDTH; ++i)
	{	
		int p_x = player->GetX();

		if (i < p_x - cull_range || p_x + cull_range < i)
			continue;

		for (int j = 0; j < TILE_HEIGHT; ++j)
		{
			int p_y = player->GetY();

			if (j < p_y - cull_range || p_y + cull_range < j)
				continue;

			if ((i >> 2) % 2 == (j >> 2) % 2)
				brush = CreateSolidBrush(RGB(255, 255, 255));
			else
				brush = CreateSolidBrush(RGB(150, 150, 150));

			old_brush = (HBRUSH)SelectObject(hdc, brush);

			Rectangle(hdc,
				tile[i][j].x - size - scroll.x,
				tile[i][j].y - size - scroll.y,
				tile[i][j].x + size - scroll.x,
				tile[i][j].y + size - scroll.y);

			SelectObject(hdc, old_brush);
			DeleteObject(brush);
		}
	}
}