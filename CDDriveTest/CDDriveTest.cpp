// CDDriveTest - Scratchpad-like thing to test CD rip stuff

#include "stdafx.h"

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
	HANDLE * cdDriveHandles = CDDriveGetHandles();
	int selectedDrive = 0; // TODO: Ask user
	
	BOOL hasCD = CDDriveCheckTray(cdDriveHandles[selectedDrive]);
	if (hasCD)
		printf("CD Found\n");
	else
	{
		printf("No CD Found\n");
		return 0;
	}

	CDROM_TOC toc = CDDriveRetrieveTOC(cdDriveHandles[selectedDrive]);
	CD_TRACK * tracks = CDGetTracksFromTOC(toc);

	int numTracks = toc.LastTrack - toc.FirstTrack + 1;
	for (int i = 0; i < numTracks; i++)
	{
		printf("Track %d:\n", i + 1);
		printf("\tStart: %ld frames\n", tracks[i].startAddress);
		printf("\tDuration: %ld frames\n", tracks[i].duration);
		
		char * filename = (char *) calloc(strlen("Track") + 4, sizeof(char));
		sprintf_s(filename, sizeof(char) * (strlen("Track") + 4), "%s%d", "Track", i + 1);
		BOOL result = CDDriveExtractTrackToWAV(cdDriveHandles[selectedDrive], tracks[i], ".", filename);
		if (result)
			printf("\tSUCCESS!\n");
		else
			printf("\tCould not extract track %d\n", i + 1);

		free(filename);
	}
	printf("ALL TASKS COMPLETED\n");

	return 0;
}

