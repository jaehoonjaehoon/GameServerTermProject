#pragma once

const int MY_SERVER_PORT = 4000;

const int MAX_BUF_SIZE		= 4000;
const int MAX_PACKET_SIZE	= 255;

const int MAX_STR_SIZE = 100;

const int BOARD_WIDTH	= 400;
const int BOARD_HEIGHT	= 400;

const int MAX_USER	= 10;

const int NPC_START = MAX_USER;
const int MAX_NPC	= 1000;

const int MAX_OBJECT = MAX_USER + MAX_NPC;


const int CS_UP = 1;
const int CS_DOWN = 2;
const int CS_LEFT = 3;
const int CS_RIGHT = 4;
const int CS_CHAT = 5;

const int SC_POS = 1;
const int SC_PUT_PLAYER = 2;
const int SC_REMOVE_PLAYER = 3;
const int SC_CHAT = 4;

#pragma pack (push, 1)

struct cs_packet_up {
	BYTE size;
	BYTE type;
};

struct cs_packet_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_left {
	BYTE size;
	BYTE type;
};

struct cs_packet_right {
	BYTE size;
	BYTE type;
};

struct cs_packet_chat {
	BYTE size;
	BYTE type;
	WCHAR message[MAX_STR_SIZE];
};

struct sc_packet_pos {
	BYTE size;
	BYTE type;
	WORD id;
	WORD x;
	WORD y;
};

struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;
	WORD x;
	WORD y;
};
struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_chat {
	BYTE size;
	BYTE type;
	WORD id;
	WCHAR message[MAX_STR_SIZE];
};

#pragma pack (pop)