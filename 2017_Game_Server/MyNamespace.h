#pragma once

#include "Zone.h"
class Object;

extern volatile bool   thread_running;
extern volatile HANDLE hIOCP;

// volatile을 붙이면 objects[i]가 안된다.
// [] operator를 못찾는다고 나옴
extern map<UINT, Object*> objects;

extern Zone zone;