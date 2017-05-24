#include "stdafx.h"
#include "MainGame.h"

#include "ObjMgr.h"
#include "NetworkMgr.h"

#include "Player.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::Initialize()
{
	BuildFrameBuffer();

	// global scroll variable
	scroll.x = 0;
	scroll.y = 0;

	send_wsa_buf.buf = reinterpret_cast<char*>(send_buf);
	send_wsa_buf.len = MAX_BUF_SIZE;

	ObjMgr::GetInstance()->Initialize();
	NetworkMgr::GetInstance()->Initialize();
}

void MainGame::Progress()
{
	ObjMgr::GetInstance()->Progress();
}

void MainGame::Render()
{
	ClearFrameBuffer(0x00FFFFFF);

	ObjMgr::GetInstance()->Render(memory_dc);

	PresentFrameBuffer();
}

void MainGame::Release()
{
	ReleaseDC(main_window_handle, memory_dc);

	ObjMgr::GetInstance()->DestroyInstance();
	NetworkMgr::GetInstance()->DestroyInstance();

}

void MainGame::BuildFrameBuffer()
{
	HDC main_dc = GetDC(main_window_handle);

	if (!memory_dc)
		memory_dc = CreateCompatibleDC(main_dc);

	if (buffer_bitmap_handle)
	{
		SelectObject(memory_dc, nullptr);
		DeleteObject(buffer_bitmap_handle);

		buffer_bitmap_handle = nullptr;
	}

	buffer_bitmap_handle = CreateCompatibleBitmap(main_dc, WINDOW_WIDTH, WINDOW_HEIGHT);

	SelectObject(memory_dc, buffer_bitmap_handle);

	ReleaseDC(main_window_handle, main_dc);
	SetBkMode(memory_dc, TRANSPARENT);
}

void MainGame::ClearFrameBuffer(DWORD color)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(memory_dc, hBrush);

	Rectangle(memory_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	SelectObject(memory_dc, hOldBrush);
	DeleteObject(hBrush);
}

void MainGame::PresentFrameBuffer()
{
	HDC main_dc = GetDC(main_window_handle);

	BitBlt(main_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memory_dc, 0, 0, SRCCOPY);

	ReleaseDC(main_window_handle, main_dc);
}

void MainGame::ProcessWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			break;
		}

		Object* player = ObjMgr::GetInstance()->GetPlayer();

		BYTE dir = 0;

		if (wParam == VK_UP)
		{
			if (player->GetY() > 0)
				dir = CS_UP;

			else break;
		}

		else if (wParam == VK_DOWN)
		{
			if(player->GetY() < TILE_HEIGHT - 1)
				dir = CS_DOWN;

			else break;
		}

		else if (wParam == VK_LEFT)
		{
			if (player->GetX() > 0)
				dir = CS_LEFT;

			else break;
		}

		else if (wParam == VK_RIGHT)
		{
			if(player->GetX() < TILE_WIDTH - 1)
				dir = CS_RIGHT;

			else break;
		}

		else break;

		cs_packet_up* move_packet = reinterpret_cast<cs_packet_up*>(&send_buf);
		move_packet->size = sizeof(cs_packet_up);
		move_packet->type = dir;

		send_wsa_buf.len = sizeof(cs_packet_up);

		DWORD io_byte;

		int ret_val = WSASend(NetworkMgr::GetInstance()->GetSock(), &send_wsa_buf, 1, &io_byte, 0, NULL, NULL);
		if (ret_val == SOCKET_ERROR)
			cout << " [error] WSASend() " << endl;

		break;
	}

	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam))
		{
			closesocket((SOCKET)wParam);
			exit(-1);
		}

		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
			NetworkMgr::GetInstance()->ReadPacket((SOCKET)wParam);
			break;

		case FD_CLOSE:
			closesocket((SOCKET)wParam);
			exit(-1);
			break;
		}
	}

	default:
		break;
	}


}
