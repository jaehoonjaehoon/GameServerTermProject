#include "stdafx.h"
#include "Server.h"

#include "ThreadFunctions.h"

#include "Player.h"
#include "NPC.h"

#include "Timer.h"

Server::Server()
{
	Initialize();
}

Server::~Server()
{
	Release();
}

void Server::Initialize()
{
	srand(unsigned(time(NULL)));

	thread_running = true;
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		if (i < MAX_USER)
		{
			objects[i] = new Player();
		}

		else if (i < NPC_START + MAX_NPC)
		{
			objects[i] = new NPC();
			objects[i]->SetID(i);
			objects[i]->SetX(rand() % BOARD_WIDTH);
			objects[i]->SetY(rand() % BOARD_HEIGHT);
			objects[i]->SetUsing(true);

			zone.UpdateSector(i);

			Timer::GetInstance()->AddTimer(i, NPC_MOVE, 1000);
		}
	}

	accept_thread = thread{ AcceptThread };

	for (int i = 0; i < NUM_WORKER; ++i)
		worker_threads.push_back(new thread{ WorkerThread });

	timer_thread = thread{ Timer::TimerThread };
}

void Server::Progress()
{
	cout << " 서버 종료 [y/Y] " << endl;

	int key;

	while (true)
	{
		if (_kbhit() != false)
		{
			key = _getch();

			if (key == 'y' || key == 'Y')
			{
				thread_running = false;
				break;
			}
		}
	}

	for (int i = 0; i < NUM_WORKER; ++i)
		PostQueuedCompletionStatus(hIOCP, 0, 0, nullptr);


	while (true)
	{
		SOCKET tmp_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

		SOCKADDR_IN server_addr;
		ZeroMemory(&server_addr, sizeof(SOCKADDR_IN));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(MY_SERVER_PORT);
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		int retval = WSAConnect(tmp_sock, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr), NULL, NULL, NULL, NULL);
		if (SOCKET_ERROR != retval)
			break;
	}
}

void Server::Release()
{
	accept_thread.join();

	for (auto worker : worker_threads)
	{
		worker->join();
		delete worker;
	}
	worker_threads.clear();

	timer_thread.join();

	for (auto object : objects)
	{
		delete object.second;
		object.second = nullptr;
	}
	objects.clear();
}
