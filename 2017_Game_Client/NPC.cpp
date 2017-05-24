#include "stdafx.h"
#include "NPC.h"

NPC::NPC()
{
	Initialize();
}

NPC::~NPC()
{
	Release();
}

void NPC::Initialize()
{

}

void NPC::Progress()
{

}

void NPC::Render(const HDC& hdc)
{
	if (is_using == false)
		return;

	HBRUSH brush, old_brush;

	brush = CreateSolidBrush(RGB(0, 0, 255));
	old_brush = (HBRUSH)SelectObject(hdc, brush);

	int size = (TILE_SIZE >> 2);

	Rectangle(hdc,
		tile[x][y].x - size - scroll.x,
		tile[x][y].y - size - scroll.y,
		tile[x][y].x + size - scroll.x,
		tile[x][y].y + size - scroll.y);

	SelectObject(hdc, old_brush);
	DeleteObject(brush);
}

void NPC::Release()
{

}

void NPC::Move(const WORD pos_x, const WORD pos_y)
{

}