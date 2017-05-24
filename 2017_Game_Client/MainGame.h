#pragma once

class MainGame
{
	HDC		memory_dc;
	HBITMAP buffer_bitmap_handle;

	BYTE	send_buf[MAX_BUF_SIZE];
	WSABUF	send_wsa_buf;

public:
	void Initialize();
	void Progress();
	void Render();
	void Release();

	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD color);
	void PresentFrameBuffer();

	void ProcessWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainGame();
	~MainGame();
};

