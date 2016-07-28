// CDDriveTest - Scratchpad-like thing to test CD rip stuff

#include "stdafx.h"

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
		
		char * filename = (char *) calloc(strlen("Track") + STRLEN_INT(i + 1) + 1, sizeof(char));
		sprintf_s(filename, sizeof(char) * (strlen("Track") + STRLEN_INT(i + 1) + 1), "%s%d", "Track", i + 1);
		BOOL result = CDDriveExtractTrackToWAV(cdDriveHandles[selectedDrive], tracks[i], ".", filename);
		if (result)
			printf("\tExtracted track to WAV\n");
		else
			printf("\tCould not extract track %d\n", i + 1);

		printf("\tConverting to MP3\n");

		MP3_TAGS myTags;
		myTags.diskNumber = 1;
		myTags.trackNumber = 1;
		myTags.year = 1995;

		result = AudioConverterWAVToMP3(".", filename, FALSE, ".", filename, myTags, 320, JOINT_STEREO,
			LAME_QUALITY_HIGH);
		if (result)
			printf("\tConverted track to MP3\n");
		else
			printf("\tCould not convert %d\n", i + 1);

		free(filename);
	}
	printf("ALL TASKS COMPLETED\n");

	return 0;
}

