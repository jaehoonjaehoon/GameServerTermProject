#pragma once

class SyncObject
{
	CRITICAL_SECTION cs;

public:
	void Enter();
	void Leave();

public:
	SyncObject();
	virtual ~SyncObject();
};


// �������� �̿��� lock ��ü
// ������ ������ �����ϰ� �������� ���������� ���ÿ� ��ü�� �Ҹ�Ǹ鼭
// �ڵ����� unlock �� �� �ֵ��� ����
class Lock
{
	SyncObject* sync_obj;

public:
	Lock(SyncObject* sync) : sync_obj(sync)
	{
		sync_obj->Enter();
	}

	~Lock() { sync_obj->Leave(); }
};