#pragma once
class Object
{
protected:
	UINT	id;
	bool	is_using;

	POINT	tile[TILE_WIDTH][TILE_HEIGHT];
	WORD	x, y;

public:
	void SetID(const UINT n) { id = n; }
	void SetUsing(const bool use) { is_using = use; }
	void SetX(const WORD n) { x = n; }
	void SetY(const WORD n) { y = n; }

	void SetTile(const POINT t[TILE_WIDTH][TILE_HEIGHT])
	{
		for (int i = 0; i < TILE_WIDTH; ++i)
		{
			for (int j = 0; j < TILE_HEIGHT; ++j)
			{
				tile[i][j] = t[i][j];
			}
		}
	}

public:
	const UINT GetID() const { return id; }
	const WORD GetX() const { return x; }
	const WORD GetY() const { return y; }

public:
	virtual void Initialize() = 0;
	virtual void Progress() = 0;
	virtual void Render(const HDC& hdc) = 0;
	virtual void Release() = 0;

public:
	virtual void Move(const WORD pos_x, const WORD pos_y) = 0;

public:
	Object();
	virtual ~Object();
};

