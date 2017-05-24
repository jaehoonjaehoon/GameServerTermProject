#include "stdafx.h"
#include "NetworkMgr.h"

#include "ObjMgr.h"

NetworkMgr* NetworkMgr::instance = nullptr;

NetworkMgr::NetworkMgr()
{
}

NetworkMgr::~NetworkMgr()
{
}


void NetworkMgr::Initialize()
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	my_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	while (true)
	{
		char ip[128] = { 0 };

		cout << " 서버 IP 입력 : ";
		cin >> ip;

		SOCKADDR_IN serverAddr;
		ZeroMemory(&serverAddr, sizeof(SOCKADDR_IN));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(MY_SERVER_PORT);
		serverAddr.sin_addr.s_addr = inet_addr(ip);

		int retval = WSAConnect(my_sock, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr), NULL, NULL, NULL, NULL);
		if (retval != SOCKET_ERROR)
			break;
	}

	WSAAsyncSelect(my_sock, main_window_handle, WM_SOCKET, FD_CLOSE | FD_READ);

	recv_wsa_buf.buf = reinterpret_cast<char*>(recv_buf);
	recv_wsa_buf.len = MAX_BUF_SIZE;

}

void NetworkMgr::Release()
{
	closesocket(my_sock);
	WSACleanup();
}

void NetworkMgr::ReadPacket(const SOCKET& sock)
{
	DWORD io_byte, io_flag = 0;

	int retval = WSARecv(sock, &recv_wsa_buf, 1, &io_byte, &io_flag, NULL, NULL);
	if (retval == SOCKET_ERROR)
	{
		cout << " RecvPacket() : WSARecv() -> SOCKET_ERROR " << endl;
		return;
	}

	BYTE* ptr = recv_buf;


	while (io_byte != 0)
	{
		if (received_size == 0)
			received_size = ptr[0];

		if (io_byte + saved_size >= received_size)
		{
			memcpy(packet_buf + saved_size, ptr, received_size - saved_size);

			ProcessPacket(packet_buf);

			ptr += received_size - saved_size;
			io_byte -= received_size - saved_size;

			received_size = 0;
			saved_size = 0;
		}

		else
		{
			memcpy(packet_buf + saved_size, ptr, io_byte);

			saved_size += io_byte;
			io_byte = 0;
		}
	}
}

void NetworkMgr::ProcessPacket(BYTE* packet)
{
	static bool first = true;

	switch (packet[1]) {
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player* put_packet = reinterpret_cast<sc_packet_put_player*>(packet);

		int id = put_packet->id;

		Object* player = ObjMgr::GetInstance()->GetPlayer();

		if (first)
		{
			first = false;
			player->SetID(id);
			player->SetUsing(true);
		}

		if (id == player->GetID())
		{
			// player
			player->SetX(put_packet->x);
			player->SetY(put_packet->y);
		}

		else if (id < NPC_START)
		{
			// other player
			player = ObjMgr::GetInstance()->GetAllPlayers()[id];
			player->SetUsing(true);
			player->SetX(put_packet->x);
			player->SetY(put_packet->y);
		}

		else
		{
			// npc
			Object* npc = ObjMgr::GetInstance()->GetNpc()[id - NPC_START];
			npc->SetUsing(true);
			npc->SetX(put_packet->x);
			npc->SetY(put_packet->y);
		}

		break;
	}

	case SC_POS:
	{
		sc_packet_pos* pos_packet = reinterpret_cast<sc_packet_pos*>(packet);

		int id = pos_packet->id;

		Object* player = ObjMgr::GetInstance()->GetPlayer();

		if (id == player->GetID())
		{
			player->Move(pos_packet->x, pos_packet->y);
		}

		else if (id < NPC_START)
		{
			player = ObjMgr::GetInstance()->GetAllPlayers()[id];
			player->Move(pos_packet->x, pos_packet->y);
		}

		else
		{
			// npc
			Object* npc = ObjMgr::GetInstance()->GetNpc()[id - NPC_START];
			npc->SetX(pos_packet->x);
			npc->SetY(pos_packet->y);
		}

		break;
	}

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player* remove_packet = reinterpret_cast<sc_packet_remove_player*>(packet);

		int id = remove_packet->id;

		Object* player = ObjMgr::GetInstance()->GetPlayer();

		if (id == player->GetID())
			player->SetUsing(false);

		else if (id < NPC_START)
		{
			player = ObjMgr::GetInstance()->GetAllPlayers()[id];

			player->SetUsing(false);
		}

		else
		{
			// npc
			Object* npc = ObjMgr::GetInstance()->GetNpc()[id - NPC_START];
			npc->SetUsing(false);
		}

		break;
	}

	default:
		break;
	}
}