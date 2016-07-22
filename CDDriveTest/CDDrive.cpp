#include "stdafx.h"

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