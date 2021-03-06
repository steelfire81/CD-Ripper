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

// reverseLongBytewiseEndian
// Reverses the order of bytes in a long
long reverseLongBytewiseEndian(long l)
{
	long result = 0;

	char * originalPointer = (char *) &l;
	char * resultPointer = (char *) &result;

	for (int i = 0; i < 4; i++)
		resultPointer[i] = originalPointer[3 - i];

	return result;
}

// longToSynchsafe
// Convert a long to a synchsafe long
long longToSynchsafe(long l)
{
	long result = 0;
	long mask = SYNCHSAFE_MASK;

	for (int i = 0; i < 4; i++)
	{
		result = l & ~mask;
		result <<= 1;
		result |= l & mask;

		mask = ((mask + 1) << 8) - 1;
		l = result;
	}

	return result;
}

// formatLongForID3Tag
// Convert a long for ID3 tagging by reversing endian and then making synchsafe
long formatLongForID3Tag(long l)
{
	return reverseLongBytewiseEndian(longToSynchsafe(l));
}
