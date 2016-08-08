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
		
		MP3Tags * tags = new MP3Tags();
		tags->setAlbum("Album");
		tags->setAlbumArtist("AlbumArtist");
		tags->setArtist("Artist");
		tags->setCompilation(TRUE);
		tags->setComposer("Composer");
		tags->setDisk(1, 1);
		tags->setGenre("Genre");
		tags->setTitle("Title");
		tags->setTrack(i + 1, numTracks);
		tags->setYear(1993);

		tags->setAlbumSort("AlbumSort");
		tags->setAlbumArtistSort("AlbumArtistSort");
		tags->setArtistSort("ArtistSort");
		tags->setBPM(120);
		tags->setComposerSort("ComposerSort");
		tags->setTitleSort("TitleSort");

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

