#include "stdafx.h"

//*** INITIALIZATION FUNCTIONS **//
// CDDriveGetHandles
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
// CDDriveCheckTray
// Checks if anything is in the CD tray
BOOL CDDriveCheckTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_STORAGE_CHECK_VERIFY, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// CDDriveCloseTray
// Closes the tray of the given CD drive
BOOL CDDriveCloseTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// CDDriveOpenTray
// Opens the tray of the given CD drive
BOOL CDDriveOpenTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// CDDriveRetrieveTOC
// Returns the table of contents of the CD in the given drive
CDROM_TOC CDDriveRetrieveTOC(HANDLE cdDrive)
{
	// Request
	CDROM_READ_TOC_EX cdTOCRequest;
	cdTOCRequest.Format = CDROM_READ_TOC_EX_FORMAT_TOC;
	cdTOCRequest.Reserved1 = 0;
	cdTOCRequest.Reserved2 = 0;
	cdTOCRequest.Reserved3 = 0;
	cdTOCRequest.SessionTrack = 0;
	cdTOCRequest.Msf = true;

	// Output parameters
	CDROM_TOC tableOfContents;
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_CDROM_READ_TOC_EX, &cdTOCRequest,
		sizeof(CDROM_READ_TOC_EX), &tableOfContents, sizeof(CDROM_TOC),
			&bytesReturned, NULL);

	if (!result)
		fprintf_s(stderr, "ERROR RETRIEVING CD TOC\n");

	return tableOfContents;
}

//*** CD FUNCTIONS ***//
// CDGetTracksFromTOC
// Returns a list of CD_TRACKs from a given table of contents
CD_TRACK * CDGetTracksFromTOC(CDROM_TOC toc)
{
	int numTracks = toc.LastTrack - toc.FirstTrack + 1;
	CD_TRACK * tracks = (CD_TRACK *) calloc(numTracks, sizeof(CD_TRACK));

	for (int i = 0; i < numTracks; i++)
	{
		TRACK_DATA td = toc.TrackData[i];
		tracks[i].startAddress = td.Address[0] * 60 * 60 * FRAMES_PER_SECOND +
			td.Address[1] * 60 * FRAMES_PER_SECOND + td.Address[2] * FRAMES_PER_SECOND +
			td.Address[3] - FRAMES_OFFSET;
	}

	for (int i = 0; i < numTracks - 1; i++)
		tracks[i].duration = tracks[i + 1].startAddress - tracks[i].startAddress;

	// For last track, go one beyond final track
	TRACK_DATA tdEnd = toc.TrackData[numTracks];
	ULONG cdEnd = tdEnd.Address[0] * 60 * 60 * FRAMES_PER_SECOND +
		tdEnd.Address[1] * 60 * FRAMES_PER_SECOND + tdEnd.Address[2] * FRAMES_PER_SECOND +
		tdEnd.Address[3] - FRAMES_OFFSET;
	tracks[numTracks - 1].duration = cdEnd - tracks[numTracks - 1].startAddress;

	return tracks;
}
