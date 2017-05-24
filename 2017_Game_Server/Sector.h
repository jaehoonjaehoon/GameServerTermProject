#pragma once
#include "SyncObject.h"

class Sector
	: public SyncObject
{
	vector<UINT> inner_players;

public:
	const vector<UINT>& GetInnerPlayers() const { return inner_players; }

public:
	void Insert(const UINT id);
	void Remove(const UINT id);

public:
	Sector();
	virtual ~Sector();
};

