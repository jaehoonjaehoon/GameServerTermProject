#pragma once

extern HWND main_window_handle;
extern POINT scroll;

const int WINDOW_WIDTH	= 800;
const int WINDOW_HEIGHT = 800;

const BYTE BACKGROUND_COLOR_R = 0;
const BYTE BACKGROUND_COLOR_G = 0;
const BYTE BACKGROUND_COLOR_B = 0;

const int TILE_WIDTH	= 400;
const int TILE_HEIGHT	= 400;
const int TILE_SIZE     = 30;

const int WM_SOCKET = WM_USER + 1;

#pragma pack (push, 1)

typedef struct Tile
{
	int		x, y;
	bool	option;

}TILE;

#pragma pack (pop)