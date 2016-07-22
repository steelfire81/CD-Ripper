// CDDriveTest - Scratchpad-like thing to test CD rip stuff

#include "stdafx.h"

// Constants - Drive information
#define DRIVE_FIRST 'A'
#define DRIVE_HANDLE_PREFIX "\\\\.\\"
#define DRIVE_NUM 26
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
	char ** cdDrives = (char **) calloc(DRIVE_NUM + 1, sizeof(char **));
	int numCDDrives = 0;
	char currDriveLetter = DRIVE_FIRST;
	for (int i = 0; i < DRIVE_NUM; i++)
	{
		// Generate drive name
		char * driveName = (char *) calloc(strlen(DRIVE_SUFFIX) + 2, sizeof(char));
		sprintf_s(driveName, sizeof(driveName), "%c%s", currDriveLetter, DRIVE_SUFFIX);
		printf("%s: ", driveName);

		// Determine drive type
		int type = GetDriveType(cStringToLPCWSTR(driveName));
		printf("%s (%d)\n", driveTypeAsString(type), type);

		// If CD drive, add to list of CD drives
		if (type == DRIVE_CDROM)
		{
			cdDrives[numCDDrives] = (char *) calloc(strlen(DRIVE_HANDLE_PREFIX) + 3, sizeof(char));
			sprintf_s(cdDrives[numCDDrives], sizeof(char) * (strlen(DRIVE_HANDLE_PREFIX) + 3), "%s%c:", DRIVE_HANDLE_PREFIX, currDriveLetter);
			numCDDrives++;
		}

		// Increment drive letter
		currDriveLetter++;
	}
	printf("\n");

	// Get handles on CD drives
	HANDLE * cdDriveHandles = (HANDLE *) calloc(numCDDrives + 1, sizeof(HANDLE));
	for (int i = 0; i < numCDDrives; i++)
	{
		printf("CREATING HANDLE ON %s\n", cdDrives[i]);
		cdDriveHandles[i] = CreateFile(cStringToLPCWSTR(cdDrives[i]), GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

		// Ensure handle worked
		if ((cdDriveHandles[i] == INVALID_HANDLE_VALUE) || (GetLastError() != NO_ERROR))
			printf("ERROR GETTING HANDLE ON %s\n", cdDrives[i]);
	}
	printf("\n");

	// Select drive to work with
	int selectedDrive = 0; // TODO: Ask user
	printf("Automatically selecting %s drive\n\n", cdDrives[selectedDrive]);

	CDDriveOpenTray(cdDriveHandles[selectedDrive]);
	CDDriveCloseTray(cdDriveHandles[selectedDrive]);

	return 0;
}

