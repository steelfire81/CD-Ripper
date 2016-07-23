// stdafx.cpp : source file that includes just the standard includes
// CDDriveTest.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// Methods
LPCWSTR cStringToLPCWSTR(const char * cString)
{
	wchar_t * result = (wchar_t *)calloc(strlen(cString) + 1, sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, cString, -1, result, strlen(cString) + 1);
	return result;
}
