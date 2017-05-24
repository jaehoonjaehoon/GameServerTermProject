#pragma once
#include "Object.h"

class NPC
	: public Object
{
	set<UINT> view;

public:
	void HeartBeat();

public:
	NPC();
	virtual ~NPC();
};

