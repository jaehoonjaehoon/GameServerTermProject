#include "stdafx.h"
#include "ThreadFunctions.h"

#include "Overlapped_Ex.h"

#include "Object.h"
#include "Player.h"
#include "NPC.h"

#include "Zone.h"
#include "Timer.h"

void AcceptThread()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listen_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(MY_SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	::bind(listen_sock, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));

	::listen(listen_sock, 10);

	UINT new_id = 0;

	while (true)
	{
		SOCKET		client_sock;
		SOCKADDR_IN client_addr;
		int	addr_len = sizeof(client_addr);

		client_sock = WSAAccept(listen_sock, reinterpret_cast<SOCKADDR*>(&client_addr), &addr_len, NULL, NULL);

		if (client_sock == SOCKET_ERROR)
			break;

		if (thread_running == false)
		{
			closesocket(client_sock);
			break;
		}

		cout << new_id << " client enter " << endl;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), hIOCP, new_id, 0);

		
		Player* player = dynamic_cast<Player*>(objects[new_id]);
		player->SetSock(client_sock);
		player->SetID(new_id);


		sc_packet_put_player put_packet;
		put_packet.size = sizeof(put_packet);
		put_packet.type = SC_PUT_PLAYER;
		put_packet.id = new_id;
		put_packet.x = objects[new_id]->GetX();
		put_packet.y = objects[new_id]->GetY();

		SendPacket(new_id, reinterpret_cast<BYTE*>(&put_packet));

		UpdatePlayerView(new_id);

		player->SetUsing(true);
		player->RecvPacket();

		++new_id;
	}

	closesocket(listen_sock);
	WSACleanup();

	cout << " exit accept thread " << endl;
}

void WorkerThread()
{
	DWORD		transferred;
	ULONGLONG	id;

	OVERLAPPED_EX* over_ex = nullptr;

	while (true)
	{
		BOOL ret_val = GetQueuedCompletionStatus(hIOCP, &transferred, (PULONG_PTR)&id, reinterpret_cast<LPOVERLAPPED*>(&over_ex), INFINITE);

		if (ret_val == false)
		{
			// error 贸府
		}

		if (thread_running == false)
			break;

		if (transferred == 0)
		{
			// 立加 辆丰 贸府
			Player* player = dynamic_cast<Player*>(objects[id]);

			if (player->GetUsing())
			{
				closesocket(player->GetSock());
				player->Clear();
				player->SetUsing(false);

				cout << id << " client exit " << endl;
			} 

			continue;
		}

		if (over_ex->command == NPC_MOVE)
		{
			NPC* npc = dynamic_cast<NPC*>(objects[id]);
			
			if (npc->GetUsing() == false)
				continue;

			zone.UpdateSector(id);

			npc->HeartBeat();

			Timer::GetInstance()->AddTimer(id, NPC_MOVE, 1000);

			continue;
		}

		if (over_ex->operation == OP_SEND)
		{
			delete over_ex;
			over_ex = nullptr;

			cout << "send 俊堪" << endl;
		}

		else if (over_ex->operation == OP_RECV)
		{
			AfterRecv(id, transferred);
			cout << "recv 俊堪" << endl;
		}

		else
		{
			cout << " [error] unknown operation detected on worker thread " << endl;
		}
	}

	cout << " exit worker thread " << endl;
}

void AfterRecv(const UINT id, const int transferred)
{
	Player* player = dynamic_cast<Player*>(objects[id]);

	player->ReadPacket(id, transferred);
	player->RecvPacket();
}

void SendPacket(const UINT id, BYTE* packet)
{
	OVERLAPPED_EX* over_ex = new OVERLAPPED_EX();
	ZeroMemory(over_ex, sizeof(OVERLAPPED_EX));
	over_ex->wsa_buf.buf = reinterpret_cast<char*>(over_ex->packet_buf);
	over_ex->wsa_buf.len = packet[0];
	over_ex->operation = OP_SEND;

	memcpy(over_ex->packet_buf, packet, packet[0]);

	Player* player = dynamic_cast<Player*>(objects[id]);

	int ret_val = WSASend(player->GetSock(), &over_ex->wsa_buf, 1, NULL, 0, &over_ex->overlapped, NULL);
	if (ret_val != 0)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			cout << " [error] WSASend() on SendPacket function " << endl;
	}
	cout << "send 啊堪" << endl;

}

void SendPacket(Player* player, BYTE* packet)
{
	OVERLAPPED_EX* over_ex = new OVERLAPPED_EX();
	ZeroMemory(over_ex, sizeof(OVERLAPPED_EX));
	over_ex->wsa_buf.buf = reinterpret_cast<char*>(over_ex->packet_buf);
	over_ex->wsa_buf.len = packet[0];
	over_ex->operation = OP_SEND;

	memcpy(over_ex->packet_buf, packet, packet[0]);

	int ret_val = WSASend(player->GetSock(), &over_ex->wsa_buf, 1, NULL, 0, &over_ex->overlapped, NULL);
	if (ret_val != 0)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			cout << " [error] WSASend() on SendPacket function " << endl;
	}

}

void ProcessPacket(const UINT id, BYTE* packet)
{
	WORD old_x = objects[id]->GetX();
	WORD old_y = objects[id]->GetY();

	switch (packet[1]) {
	case CS_UP:		objects[id]->SetY(old_y - 1); break;
	case CS_DOWN:	objects[id]->SetY(old_y + 1); break;
	case CS_LEFT:	objects[id]->SetX(old_x - 1); break;
	case CS_RIGHT:	objects[id]->SetX(old_x + 1); break;

	default:
		cout << " [error] unknown packet type detected on ProcessPacket functions " << endl;
		break;
	}

	sc_packet_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SC_POS;
	pos_packet.id = id;
	pos_packet.x = objects[id]->GetX();
	pos_packet.y = objects[id]->GetY();

	SendPacket(id, reinterpret_cast<BYTE*>(&pos_packet));

	UpdatePlayerView(id);
}

void ProcessEvent(const NPC_EVENT& e)
{
	Object* object = objects[e.id];
	ZeroMemory(&object->GetOverEx(), sizeof(object->GetOverEx()));

	switch (e.type) {
	case NPC_MOVE:
	{
		object->GetOverEx().command = NPC_MOVE;
		break;
	}

	default:
		break;
	}

	PostQueuedCompletionStatus(hIOCP, 1, object->GetID(), (LPOVERLAPPED)&object->GetOverEx());
}

void UpdatePlayerView(const UINT id)
{
	zone.UpdateSector(id);

	Player* player = dynamic_cast<Player*>(objects[id]);
	player->UpdateNearList();
	player->UpdateViewList();
}