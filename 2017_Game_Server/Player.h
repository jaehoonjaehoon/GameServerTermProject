#pragma once
#include "Object.h"

#include "ViewList.h"

class Player
	: public Object
{
	SOCKET sock;

	// OVERLAPPED_EX ����ü�� �־������ ��Ŷ�� ���� ���� ���� NPC�� ���
	// ���� �޸𸮸� ��ƸԱ� ������ Player Ŭ������ ���� ������
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

