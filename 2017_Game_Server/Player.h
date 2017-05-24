#pragma once
#include "Object.h"

#include "ViewList.h"

class Player
	: public Object
{
	SOCKET sock;

	// OVERLAPPED_EX 구조체에 넣어놓으면 패킷을 받을 일이 없는 NPC의 경우
	// 괜히 메모리만 잡아먹기 때문에 Player 클래스에 따로 빼놓음
	BYTE recv_buf[MAX_BUF_SIZE];

	BYTE packet_buf[MAX_PACKET_SIZE];
	BYTE packet_size;
	BYTE saved_size;

	ViewList view_list;

public:
	const SOCKET& GetSock() const { return sock; }
	ViewList& GetViewList() { return view_list; }

public:
	void SetSock(const SOCKET& s) { sock = s; }

public:
	void RecvPacket();
	void ReadPacket(const UINT id, const int transferred);

public:
	void UpdateViewList() { view_list.Update(near_list); }

public:
	void Clear();

public:
	Player();
	virtual ~Player();
};

