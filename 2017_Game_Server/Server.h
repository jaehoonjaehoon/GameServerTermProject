#pragma once

class Server
{
	thread			accept_thread;
	vector<thread*> worker_threads;
	thread			timer_thread;

public:
	void Initialize();
	void Progress();
	void Release();

public:
	Server();
	~Server();
};

