#pragma once

#include "Sector.h"

const int SECTOR_WIDTH	= 10;
const int SECTOR_HEIGHT = 10;

const int ZONE_WIDTH = BOARD_WIDTH / SECTOR_WIDTH;
const int ZONE_HEIGHT = BOARD_HEIGHT / SECTOR_HEIGHT;

class Zone
{
	Sector sectors[ZONE_WIDTH][ZONE_HEIGHT];

public:
	void UpdateSector(const UINT id);

	Sector* GetSectorWithPoint(const WORD x, const WORD y);

public:
	Zone();
	~Zone();
};

