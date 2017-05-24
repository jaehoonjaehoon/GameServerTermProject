#pragma once

const int NUM_WORKER = 6;

const int VIEW_RADIUS = 4;

const int OP_SEND = 1;
const int OP_RECV = 2;

const int NPC_MOVE = 1;

typedef struct Npc_Event
{
	UINT id;
	int	 type;
	UINT time;

}NPC_EVENT;

class Comparison
{
public:
	bool operator() (const NPC_EVENT& e1, const NPC_EVENT& e2)
	{
		return (e1.time > e2.time);
	}
};
