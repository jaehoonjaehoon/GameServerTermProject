#pragma once

#include "Object.h"
#include "Board.h"

class ObjMgr
{
	POINT	tile[TILE_WIDTH][TILE_HEIGHT];

	Board*	board;
	
	Object* player;

	vector<Object*> players;
	vector<Object*> npc;

public:
	Object* GetPlayer() { return player; }
	const vector<Object*>& GetAllPlayers() const { return players; }
	const vector<Object*>& GetNpc() const { return npc; }

public:
	void Initialize();
	void Progress();
	void Render(const HDC& hdc);
	void Release();

private:
	static ObjMgr* instance;

public:
	static ObjMgr* GetInstance()
	{
		if (instance == nullptr)
			instance = new ObjMgr();

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
	ObjMgr();
public:
	~ObjMgr();
};

