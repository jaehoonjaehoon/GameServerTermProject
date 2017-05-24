#include "stdafx.h"
#include "Object.h"

#include "Zone.h"

Object::Object()
	: is_using(false), id(-1), x(4), y(4)
{
	over_ex.operation = OP_RECV;
}

Object::~Object()
{
}

void Object::UpdateNearList()
{
	// �þ߹����� �� �������� ����
	RECT rc;
	rc.left	  = x - VIEW_RADIUS / 2;
	rc.right  = x + VIEW_RADIUS / 2 + 1;
	rc.top	  = y - VIEW_RADIUS / 2;
	rc.bottom = y + VIEW_RADIUS / 2 + 1;

	// 0 Ȥ�� TILE_WIDTH(�ִ밪)�� �ѱ��� �ʵ��� ����
	ClampRect(rc);

	near_sector.clear();

	// �ֺ� ���͸� �ߺ����� near_sectors�� �߰�
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.left, rc.top));
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.right, rc.top));
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.left, rc.bottom));
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.right, rc.bottom));
	
	near_list.clear();

	// near_sectors�� �����ϴ� ��� ������Ʈ �� �þ� ���� �ִ� ������Ʈ�� near_list�� �߰�
	for (auto sector : near_sector)
	{
		sector->Enter();
		vector<UINT> v = sector->GetInnerPlayers();
		sector->Leave();

		for (auto near_id : v)
		{
			if (near_id == id)
				continue;

			if (IsInViewArea(near_id, id) == true)
				near_list.push_back(near_id);
		}
	}
}

void Object::ClampRect(RECT& rc)
{
	ClampRange(rc.left, 0, 400 - 1);
	ClampRange(rc.right, 0, 400 - 1);
	ClampRange(rc.top, 0, 400 - 1);
	ClampRange(rc.bottom, 0, 400 - 1);
}

void Object::ClampRange(long& value, const long min, const long max)
{
	if (value < min)
		value = min;
	if (value >= max)
		value = max;
}

void Object::InsertNearSector(vector<Sector*>& near_sector, Sector* sector)
{
	auto iter = find(near_sector.begin(), near_sector.end(), sector);

	if (iter == near_sector.end())
		near_sector.push_back(sector);
}

bool Object::IsInViewArea(const UINT i, const UINT j)
{
	int x = objects[i]->GetX() - objects[j]->GetX();
	int y = objects[i]->GetY() - objects[j]->GetY();

	int dist = x * x + y * y;

	// dist�� sqrt �ϴ� �ͺ��� VIEW_RADIUS�� �����ϴ� ���� ����
	return (dist <= VIEW_RADIUS * VIEW_RADIUS);
}