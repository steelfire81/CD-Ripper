// CDDriveTest - Scratchpad-like thing to test CD rip stuff

#include "stdafx.h"

// main
int main()
{
	/*
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
	*/

	AudioConverterInit();
	AudioConverterInitializeLAME(2, 44100, 320, JOINT_STEREO, LAME_QUALITY_HIGH);

	return 0;
}

