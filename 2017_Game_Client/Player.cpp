#include "stdafx.h"
#include "Player.h"

Player::Player()
	: is_my_player(false)
{
	Initialize();
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
}

void Player::Progress()
{
}

void Player::Render(const HDC& hdc)
{
	if (is_using == false)
		return;

	HBRUSH brush, old_brush;

	if (is_my_player)
		brush = CreateSolidBrush(RGB(0, 255, 0));
	else
		brush = CreateSolidBrush(RGB(255, 0, 0));

	old_brush = (HBRUSH)SelectObject(hdc, brush);

	int size = (TILE_SIZE >> 1);

	Ellipse(hdc,
		tile[x][y].x - size - scroll.x,
		tile[x][y].y - size - scroll.y,
		tile[x][y].x + size - scroll.x,
		tile[x][y].y + size - scroll.y);

	SelectObject(hdc, old_brush);
	DeleteObject(brush);
}

void Player::Release()
{

}

void Player::Move(const WORD pos_x, const WORD pos_y)
{
	int old_x = x;
	int old_y = y;

	x = pos_x;
	y = pos_y;

	if (is_my_player == false)
		return;

	int a = (WINDOW_WIDTH / TILE_SIZE) / 2;
	int b = (WINDOW_HEIGHT / TILE_SIZE) / 2;

	if (a <= x && x < TILE_WIDTH && old_x > pos_x)
		scroll.x -= TILE_SIZE;

	else if (a < x && x <= TILE_WIDTH && old_x < pos_x)
		scroll.x += TILE_SIZE;

	else if (b <= y && y < TILE_HEIGHT && old_y > pos_y)
		scroll.y -= TILE_SIZE;

	else if (b < y && y <= TILE_HEIGHT && old_y < pos_y)
		scroll.y += TILE_SIZE;
}