#pragma once

class NetworkMgr
{
	SOCKET	my_sock;

	BYTE	recv_buf[MAX_BUF_SIZE];
	WSABUF	recv_wsa_buf;

	BYTE	packet_buf[MAX_BUF_SIZE];

	int		received_size;
	int		saved_size;

public:
	const SOCKET& GetSock() const { return my_sock; }

public:
	void Initialize();
	void Release();

public:
	void ReadPacket(const SOCKET& sock);
	void ProcessPacket(BYTE* packet);

private:
	static NetworkMgr* instance;

public:
	static NetworkMgr* GetInstance()
	{
		if (instance == nullptr)
			instance = new NetworkMgr();

		return instance;
	}

	void DestroyInstance()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

private:
	NetworkMgr();
public:
	~NetworkMgr();
};

