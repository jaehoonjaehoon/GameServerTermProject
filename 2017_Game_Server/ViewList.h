#pragma once
#include "SyncObject.h"

class Player;

const int view_size = 64;

class ViewList
	: public SyncObject
{
	Player* owner;

	vector<pair<UINT, bool>> view;
	vector<pair<UINT, bool>> temp;

public:
	void Update(vector<UINT>& near_list);

	bool IsInViewList(const UINT id);

	void InsertInTemp(const UINT id);
	void InsertInView(const UINT id);
	void RemoveInView(const UINT id);

public:
	void PutMessage(const UINT id);
	void MoveMessage(const UINT id);
	void RemoveMessage(const UINT id);

public:
	void Clear();

public:
	ViewList(Player* player);
	virtual ~ViewList();
};

