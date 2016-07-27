// stdafx.cpp : source file that includes just the standard includes
// CDDriveTest.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// Methods
// cStringToLPCWSTR
// Converts a char * to a pointer to a wide string
LPCWSTR cStringToLPCWSTR(const char * cString)
{
	wchar_t * result = (wchar_t *)calloc(strlen(cString) + 1, sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, cString, -1, result, strlen(cString) + 1);
	return result;
}

// getFilePath
// Creates a char * representing a path to a file
char * getFilePath(char * dir, char * filename, char * ext)
{
	char * path = (char *)calloc(strlen(dir) + strlen(filename) + strlen(ext) + 2, sizeof(char));
	sprintf_s(path, sizeof(char) * (strlen(dir) + strlen(filename) + strlen(ext) + 2), "%s\\%s%s",
		dir, filename, ext);
	return path;
}