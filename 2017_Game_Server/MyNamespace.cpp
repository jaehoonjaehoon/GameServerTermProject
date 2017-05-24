#include "stdafx.h"
#include "MyNamespace.h"

volatile bool	thread_running	= false;
volatile HANDLE hIOCP			= NULL;

map<UINT, Object*> objects;
Zone zone;