#include "stdafx.h"
#include "Player.h"

#include "ThreadFunctions.h"
#include "Sector.h"

Player::Player()
	: packet_size(0), saved_size(0), view_list(this)
{
	ZeroMemory(recv_buf, sizeof(recv_buf));
	ZeroMemory(&over_ex.overlapped, sizeof(over_ex.overlapped));
	over_ex.wsa_buf.buf = reinterpret_cast<char*>(recv_buf);
	over_ex.wsa_buf.len = sizeof(recv_buf);

	near_list.reserve(100);
	near_sector.reserve(5);
	curr_sector = nullptr;
}

Player::~Player()
{
}

void Player::RecvPacket()
{
	DWORD flags = 0;

	int ret_val = WSARecv(sock, &over_ex.wsa_buf, 1, NULL, &flags, &over_ex.overlapped, NULL);
	if (ret_val != 0)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			cout << " [error] WSARecv() on " << id << " Player::RecvPacket() " << endl;
	}
}

void Player::ReadPacket(const UINT id, const int transferred)
{
	BYTE* ptr = recv_buf;

	int remained = transferred;

	while (0 < remained)
	{
		if (0 == packet_size)
			packet_size = ptr[0];

		int required = packet_size - saved_size;

		if (remained >= required)
		{
			memcpy(packet_buf + saved_size, ptr, required);

			ProcessPacket(id, packet_buf);

			remained -= required;
			ptr += required;

			packet_size = 0;
			saved_size = 0;
		}

		else
		{
			memcpy(packet_buf + saved_size, ptr, remained);

			saved_size += remained;

			remained = 0;
		}
	}
}

void Player::Clear()
{
	view_list.Clear();

	if (curr_sector)
		curr_sector->Remove(id);
}