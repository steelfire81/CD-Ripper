// CDDriveTest - Scratchpad-like thing to test CD rip stuff

#include "stdafx.h"

// Constants
#define DRIVE_SUFFIX ":\\"

// cStringToLPCWSTR - converts a char * to a LPCWSTR
LPCWSTR cStringToLPCWSTR(const char * cString)
{
	wchar_t * result = (wchar_t *) calloc(strlen(cString) + 1, sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, cString, -1, result, strlen(cString) + 1);
	return result;
}

// driveTypeAsString - takes an int containing drive type and converts to string for debug
char * driveTypeAsString(int type)
{
	if (type == DRIVE_UNKNOWN)
		return "UNKNOWN DRIVE TYPE";
	else if (type == DRIVE_NO_ROOT_DIR)
		return "NO VOLUME MOUNTED";
	else if (type == DRIVE_REMOVABLE)
		return "REMOVABLE MEDIA DRIVE";
	else if (type == DRIVE_FIXED)
		return "FIXED MEDIA DRIVE";
	else if (type == DRIVE_REMOTE)
		return "NETWORK DRIVE";
	else if (type == DRIVE_CDROM)
		return "CD DRIVE";
	else if (type == DRIVE_RAMDISK)
		return "RAM DISK";
	else
		return "INVALID DRIVE TYPE";
}

// main
int main()
{
	// Initialize and find which drives are CD drives
	DWORD drives = GetLogicalDrives();
	char curr = 'A';
	for (int i = 0; i < 26; i++)
	{
		// Generate drive name
		std::string driveName = "";
		driveName += curr;
		driveName += DRIVE_SUFFIX;
		const char * driveNameCString = driveName.c_str();
		printf("%s: ", driveNameCString);

		// Determine drive type
		int type = GetDriveType(cStringToLPCWSTR(driveNameCString));
		printf("%s (%d)\n", driveTypeAsString(type), type);

		// Increment drive letter
		curr++;
	}

	return 0;
}

