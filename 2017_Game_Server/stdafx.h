// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#define WIN32_LEAN_AND_MEAN
#define INITGUID

#include <time.h>
#include <conio.h>
#include <windows.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <mutex>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;

#include "Protocol.h"
#include "MyHeader.h"
#include "MyNamespace.h"