#include "stdafx.h"
#include "Sector.h"

Sector::Sector()
{
}

Sector::~Sector()
{
}

void Sector::Insert(const UINT id)
{
	Lock sync(this);
	
	auto iter = find(inner_players.begin(), inner_players.end(), id);

	// 추가 대상의 아이디가 이미 존재함
	if (iter != inner_players.end())
	{
		Leave();
		return;
	}

	inner_players.push_back(id);
}

void Sector::Remove(const UINT id)
{
	Lock sync(this);

	auto iter = find(inner_players.begin(), inner_players.end(), id);

	// 삭제 대상을 찾지 못함
	if (iter == inner_players.end())
	{
		Leave();
		return;
	}

	inner_players.erase(iter);
}