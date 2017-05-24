#pragma once

#include "Overlapped_Ex.h"
#include "Sector.h"

class Object
{
protected:
	bool is_using;
	UINT id;

	OVERLAPPED_EX over_ex;

	WORD x, y;

	Sector*			curr_sector;
	vector<Sector*> near_sector;

	vector<UINT>	near_list;

public:
	const bool GetUsing() const { return is_using; }
	const UINT GetID() const { return id; }
	const WORD GetX() const { return x; }
	const WORD GetY() const { return y; }
	OVERLAPPED_EX& GetOverEx() { return over_ex; }
	Sector* GetCurrSector() { return curr_sector; }
	const vector<UINT>& GetNearList() const { return near_list; }

public:
	void SetUsing(const bool used) { is_using = used; }
	void SetID(const UINT n) { id = n; }
	void SetX(const WORD n) { x = n; }
	void SetY(const WORD n) { y = n; }
	void SetSector(Sector* sector) { curr_sector = sector; }

public:
	void UpdateNearList();

	void ClampRect(RECT& rc);
	void ClampRange(long& value, const long min, const long max);

	void InsertNearSector(vector<Sector*>& near_sector, Sector* sector);

	bool IsInViewArea(const UINT i, const UINT j);

public:
	Object();
	virtual ~Object();
};

