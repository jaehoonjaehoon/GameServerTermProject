#include "stdafx.h"
#include "Zone.h"

#include "Object.h"
#include "Sector.h"

Zone::Zone()
{
}

Zone::~Zone()
{
}

void Zone::UpdateSector(const UINT id)
{
	Object* object = objects[id];

	Sector* curr_sector = object->GetCurrSector();
	Sector* new_sector = GetSectorWithPoint(object->GetX(), object->GetY());

	if (curr_sector != new_sector)
	{
		if(curr_sector)
			curr_sector->Remove(id);
		new_sector->Insert(id);

		object->SetSector(new_sector);
	}
}

Sector* Zone::GetSectorWithPoint(const WORD x, const WORD y)
{
	if (y / SECTOR_HEIGHT >= ZONE_HEIGHT || x / SECTOR_WIDTH >= ZONE_WIDTH || x < 0 || y < 0)
	{
		cout << " [error] sector 접근 오류 : " << x << ", " << y << endl;
		return nullptr;
	}

	return &sectors[y / SECTOR_HEIGHT][x / SECTOR_WIDTH];
}