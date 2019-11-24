// CDDriveTest - Scratchpad-like thing to test CD rip stuff

#include "stdafx.h"

// getTrackTags
// Retrieve tags for a specified CD track
static MP3Tags * getTrackTags(int trackNumber)
{
	MP3Tags * tags = new MP3Tags();

	char * title;
	
	// TODO: Retrieve these from a UI instead of using fixed values
	switch (trackNumber)
	{
		case 0:
			title = "Cherub Rock";
			break;
		case 1:
			title = "Pissant";
			break;
		case 2:
			title = "French Movie Theme";
			break;
		default:
			title = "This Track Doesn't Exist";
			break;
	}

	tags->setTitle(title);

	return tags;
}

// freeTrackTags
// Free space allocated to a set of tags
static void freeTrackTags(MP3Tags * tags)
{
	delete tags;
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

	CDPLBasic * listener = new CDPLBasic();

	int numTracks = toc.LastTrack - toc.FirstTrack + 1;
	for (int i = 0; i < numTracks; i++)
	{
		printf("Track %d:\n", i + 1);
		printf("\tStart: %ld frames\n", tracks[i].startAddress);
		printf("\tDuration: %ld frames\n", tracks[i].duration);
		
		// Set track-specific tags
		MP3Tags * tags = getTrackTags(i);

		// Set tags dependent on entire album
		// TODO: Allow these to be customizable per-track as well
		// TODO: Retrieve these from UI as well
		tags->setArtist("Smashing Pumpkins");
		tags->setAlbum("Cherub Rock - Single");
		tags->setTrack(i + 1, numTracks);
		tags->setYear(1993);
		tags->setGenre("Alternative Rock");
		tags->setDisk(1, 1);

		// Format filename
		// TODO: Extract this to separate function
		// TODO: Use tags to build filename in format specifiable by user
		char * filename = (char *)calloc(strlen("Track") + STRLEN_INT(i + 1) + 1, sizeof(char));
		sprintf_s(filename, sizeof(char) * (strlen("Track") + STRLEN_INT(i + 1) + 1), "%s%d", "Track", i + 1);

		BOOL result = CDDriveExtractTrackToMP3(cdDriveHandles[selectedDrive], tracks[i], ".", filename, tags, listener);
		if (result)
			printf("\tExtracted track to MP3\n");
		else
			printf("\tCould not extract track %d\n", i + 1);

		free(filename);
		delete tags;
	}
	printf("ALL TASKS COMPLETED\n");

	return 0;
}

