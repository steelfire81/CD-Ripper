// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// My includes
#include <Windows.h>
#include <Ntddcdrm.h>
#include <ctype.h>
#include "lame.h"
#include "AudioConverter.h"
#include "CDDrive.h"

// Method declarations
LPCWSTR cStringToLPCWSTR(const char * cString);
char * getFilePath(char * dir, char * filename, char * ext);
