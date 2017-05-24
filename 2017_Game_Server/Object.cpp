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
	// 시야범위의 각 꼭지점을 구함
	RECT rc;
	rc.left	  = x - VIEW_RADIUS / 2;
	rc.right  = x + VIEW_RADIUS / 2 + 1;
	rc.top	  = y - VIEW_RADIUS / 2;
	rc.bottom = y + VIEW_RADIUS / 2 + 1;

	// 0 혹은 TILE_WIDTH(최대값)을 넘기지 않도록 조정
	ClampRect(rc);

	near_sector.clear();

	// 주변 섹터를 중복없이 near_sectors에 추가
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.left, rc.top));
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.right, rc.top));
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.left, rc.bottom));
	InsertNearSector(near_sector, zone.GetSectorWithPoint(rc.right, rc.bottom));
	
	near_list.clear();

	// near_sectors에 존재하는 모든 오브젝트 중 시야 내에 있는 오브젝트만 near_list에 추가
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

	// dist를 sqrt 하는 것보다 VIEW_RADIUS를 제곱하는 것이 빠름
	return (dist <= VIEW_RADIUS * VIEW_RADIUS);
}