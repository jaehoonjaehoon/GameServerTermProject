#pragma once

#include "Zone.h"
class Object;

extern volatile bool   thread_running;
extern volatile HANDLE hIOCP;

// volatile�� ���̸� objects[i]�� �ȵȴ�.
// [] operator�� ��ã�´ٰ� ����
extern map<UINT, Object*> objects;

extern Zone zone;