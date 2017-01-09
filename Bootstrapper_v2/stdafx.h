// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include <atlstr.h>
#include <atltrace.h>
#include <Msi.h>
#pragma comment(lib, "msi.lib")
#include <KnownFolders.h>
#include <ShlObj.h>
#include <Shlwapi.h>
//#include <MsXml6.h>
#import <msxml6.dll>
