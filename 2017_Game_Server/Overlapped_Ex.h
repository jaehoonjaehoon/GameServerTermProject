#pragma once

typedef struct Overlapped_Ex
{
	WSAOVERLAPPED	overlapped;
	WSABUF			wsa_buf;

	BYTE packet_buf[MAX_PACKET_SIZE];

	int	command;
	int operation;

}OVERLAPPED_EX;