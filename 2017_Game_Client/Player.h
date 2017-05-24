#pragma once
#include "Object.h"

class Player
	: public Object
{
	bool	is_my_player;

public:
	void SetIsMyPlayer(const bool mine) { is_my_player = mine; }

public:
	virtual void Initialize();
	virtual void Progress();
	virtual void Render(const HDC& hdc);
	virtual void Release();

public:
	virtual void Move(const WORD pos_x, const WORD pos_y);

public:
	Player();
	virtual ~Player();
};

