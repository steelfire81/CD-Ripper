#include "stdafx.h"

//*** INITIALIZATION FUNCTIONS **//
// Retrieves a list of handles on CD drives
HANDLE * CDDriveGetHandles()
{
	HANDLE * cdDriveHandles = (HANDLE *) calloc(DRIVE_NUM + 1, sizeof(HANDLE));
	DWORD drives = GetLogicalDrives();
	int numCDDrives = 0;
	char currDriveLetter = DRIVE_FIRST;

	for (int i = 0; i < DRIVE_NUM; i++)
	{
		char * driveName = (char *)calloc(strlen(DRIVE_SUFFIX) + 2, sizeof(char));
		sprintf_s(driveName, sizeof(driveName), "%c%s", currDriveLetter, DRIVE_SUFFIX);
		if (GetDriveType(cStringToLPCWSTR(driveName)) == DRIVE_CDROM)
		{
			char * driveHandleName = (char *) calloc(strlen(DRIVE_HANDLE_PREFIX) + 3, sizeof(char));
			sprintf_s(driveHandleName, sizeof(char) * (strlen(DRIVE_HANDLE_PREFIX) + 3),
				"%s%c:", DRIVE_HANDLE_PREFIX, currDriveLetter);
			cdDriveHandles[numCDDrives] = CreateFile(cStringToLPCWSTR(driveHandleName), GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				NULL);

			if ((cdDriveHandles[numCDDrives] == INVALID_HANDLE_VALUE) || (GetLastError() != NO_ERROR))
				fprintf_s(stderr, "ERROR GETTING HANDLE ON %s\n", driveName);
			else
				numCDDrives++;

			free(driveHandleName);
		}

		free(driveName);
		currDriveLetter++;
	}

	realloc(cdDriveHandles, sizeof(HANDLE) * (numCDDrives));
	return cdDriveHandles;
}

//*** INDIVIDUAL DRIVE FUNCTIONS ***//
// Checks if anything is in the CD tray
BOOL CDDriveCheckTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_STORAGE_CHECK_VERIFY, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// Closes the tray of the given CD drive
int CDDriveCloseTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	int result = DeviceIoControl(cdDrive, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// Opens the tray of the given CD drive
int CDDriveOpenTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	int result = DeviceIoControl(cdDrive, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}