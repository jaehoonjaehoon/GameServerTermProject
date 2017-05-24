#pragma once
#include "Object.h"

class NPC
	: public Object
{
public:
	virtual void Initialize();
	virtual void Progress();
	virtual void Render(const HDC& hdc);
	virtual void Release();

public:
	virtual void Move(const WORD pos_x, const WORD pos_y);

public:
	NPC();
	virtual ~NPC();
};

