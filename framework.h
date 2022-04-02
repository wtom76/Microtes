// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#pragma warning (push)
#pragma warning (disable: 4458)
#include <objidl.h>
#include <Gdiplus.h>
#pragma warning (pop)
#pragma comment (lib,"Gdiplus.lib")

#include <cassert>
#include <random>

using namespace Gdiplus;
using namespace std;

#undef min
#undef max

#include "util.h"
