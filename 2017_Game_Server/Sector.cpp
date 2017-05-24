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

	// �߰� ����� ���̵� �̹� ������
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

	// ���� ����� ã�� ����
	if (iter == inner_players.end())
	{
		Leave();
		return;
	}

	inner_players.erase(iter);
}