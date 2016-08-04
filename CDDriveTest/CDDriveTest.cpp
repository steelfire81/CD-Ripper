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
		
		MP3Tags tags = MP3Tags::MP3Tags();
		tags.setAlbum("Cherub Rock");
		tags.setAlbumArtist("Smashing Pumpkins");
		tags.setArtist("Smashing Pumpkins");
		tags.setCompilation(FALSE);
		tags.setComposer("Billy Corgan");
		tags.setDisk(1, 1);
		tags.setEncoder("LAME3.99.5");
		tags.setGenre("Alternative Rock");
		tags.setTitle("Cherub Rock");
		tags.setTrack(i + 1, numTracks);
		tags.setYear(1993);

		char * filename = (char *) calloc(strlen("Track") + STRLEN_INT(i + 1) + 1, sizeof(char));
		sprintf_s(filename, sizeof(char) * (strlen("Track") + STRLEN_INT(i + 1) + 1), "%s%d", "Track", i + 1);
		BOOL result = CDDriveExtractTrackToMP3(cdDriveHandles[selectedDrive], tracks[i], ".", filename, tags);
		if (result)
			printf("\tExtracted track to MP3\n");
		else
			printf("\tCould not extract track %d\n", i + 1);

		free(filename);
	}
	printf("ALL TASKS COMPLETED\n");

	return 0;
}

