#include "stdafx.h"
#include "ObjMgr.h"

#include "Player.h"
#include "NPC.h"

ObjMgr* ObjMgr::instance = nullptr;

ObjMgr::ObjMgr()
{
}

ObjMgr::~ObjMgr()
{
	Release();
}

void ObjMgr::Initialize()
{
	for (int i = 0; i < TILE_WIDTH; ++i)
	{
		for (int j = 0; j < TILE_HEIGHT; ++j)
		{
			POINT pt{ (i + 1) * TILE_SIZE + (TILE_SIZE >> 1),
				      (j + 1) * TILE_SIZE + (TILE_SIZE >> 1) };

			tile[i][j] = pt;
		}
	}
	
	player = new Player();
	player->SetTile(tile);
	dynamic_cast<Player*>(player)->SetIsMyPlayer(true);

	board = new Board();
	board->SetTile(tile);
	board->SetPlayer(player);

	players.reserve(MAX_USER);

	for (int i = 0; i < MAX_USER; ++i)
	{
		players.push_back(new Player());
		players[i]->SetTile(tile);
	}

	int num_npc = MAX_OBJECT - MAX_USER;

	npc.reserve(num_npc);

	for (int i = 0; i < num_npc; ++i)
	{
		npc.push_back(new NPC());
		npc[i]->SetTile(tile);
	}
}

void ObjMgr::Progress()
{
	player->Progress();

	for (auto other : players)
		other->Progress();

	for (auto monster : npc)
		monster->Progress();
}

void ObjMgr::Render(const HDC& hdc)
{
	board->Render(hdc);
	
	for (auto other : players)
		other->Render(hdc);

	for (auto monster : npc)
		monster->Render(hdc);

	player->Render(hdc);
}

void ObjMgr::Release()
{
	delete board;
	board = nullptr;

	delete player;
	player = nullptr;

	for (auto other : players)
	{
		delete other;
		other = nullptr;
	}

	for (auto monster : npc)
	{
		delete monster;
		monster = nullptr;
	}
}
