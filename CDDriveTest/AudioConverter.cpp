#include "stdafx.h"

//*** INITIALIZATION FUNCTIONS ***//
// AudioConverterInit
// Initialize all necessary audio converters
BOOL AudioConverterInit()
{
	return FALSE;
}

//*** MP3 FUNCTIONS ***//
// AudioConverterWAVToMP3
// Convert a WAV file to an MP3 file
BOOL AudioConverterWAVToMP3(HANDLE wavFile, BOOL del, char * dir, char * filename, MP3_TAGS tags)
{
	char * path = getFilePath(dir, filename, EXT_MP3);
	HANDLE outFile = CreateFile(cStringToLPCWSTR(path), GENERIC_WRITE, 0, NULL, CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_EXISTS)
	{
		fprintf_s(stderr, "ERROR: FILE EXISTS AT %s\n", path);
		return FALSE;
	}
	if (outFile == INVALID_HANDLE_VALUE)
	{
		fprintf_s(stderr, "ERROR: COULD NOT CREATE %s\n", path);
		return FALSE;
	}
	
	DWORD bytesWritten = 0;

	// ID3v2.3.0 INFORMATION AT END OF FILE
	// ID3v2.3.0 Header
	char majorVersion = ID3_MAJOR_VERSION;
	char minorVersion = ID3_MINOR_VERSION;
	char flags = ID3_FLAGS;

	WriteFile(outFile, ID3_IDENTIFIER, 3, &bytesWritten, NULL);
	WriteFile(outFile, &majorVersion, 1, &bytesWritten, NULL);
	WriteFile(outFile, &minorVersion, 1, &bytesWritten, NULL);
	WriteFile(outFile, &flags, 1, &bytesWritten, NULL);

	// ID3v2.3.0 Frames
	if (tags.album != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_ALBUM, tags.album, FALSE);
	if (tags.albumArtist != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_ALBUM_ARTIST, tags.albumArtist, FALSE);
	if (tags.artist != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_ARTIST, tags.artist, FALSE);
	if (tags.comment != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_COMMENT, tags.comment, FALSE);
	if (tags.compilation)
		AudioConverterWriteID3Frame(outFile, ID3_COMPILATION, "1", FALSE);
	if (tags.composer != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_COMPOSER, tags.composer, FALSE);
	if (tags.diskNumber > 0)
	{
		char * diskNumberField;
		if (tags.diskNumberTotal > 0)
		{
			diskNumberField = (char *)calloc(STRLEN_INT(tags.diskNumber) + STRLEN_INT(tags.diskNumberTotal)
				+ 2, sizeof(char));
			sprintf_s(diskNumberField, sizeof(char) * (STRLEN_INT(tags.diskNumber) + STRLEN_INT(tags.diskNumberTotal)
				+ 2), "%d/%d", tags.diskNumber, tags.diskNumberTotal);
		}
		else // Just print disk number
		{
			diskNumberField = (char *)calloc(STRLEN_INT(tags.diskNumber) + 1, sizeof(char));
			sprintf_s(diskNumberField, sizeof(char) * (STRLEN_INT(tags.diskNumber) + 1), "%d", tags.diskNumber);
		}

		AudioConverterWriteID3Frame(outFile, ID3_DISK_NUMBER, diskNumberField, FALSE);
		free(diskNumberField);
	}
	if (tags.encodedBy != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_ENCODED_BY, tags.encodedBy, TRUE);
	if (tags.genre != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_GENRE, tags.genre, FALSE);
	if (tags.title != NULL)
		AudioConverterWriteID3Frame(outFile, ID3_TITLE, tags.title, FALSE);
	if (tags.trackNumber > 0)
	{
		char * trackNumberField;
		if (tags.trackNumberTotal > 0)
		{
			trackNumberField = (char *)calloc(STRLEN_INT(tags.trackNumber) + STRLEN_INT(tags.trackNumberTotal)
				+ 2, sizeof(char));
			sprintf_s(trackNumberField, sizeof(char) * (STRLEN_INT(tags.trackNumber) + STRLEN_INT(tags.trackNumberTotal)
				+ 2), "%d/%d", tags.trackNumber, tags.trackNumberTotal);
		}
		else // Just print track number
		{
			trackNumberField = (char *)calloc(STRLEN_INT(tags.trackNumber) + 1, sizeof(char));
			sprintf_s(trackNumberField, sizeof(char) * (STRLEN_INT(tags.trackNumber) + 1), "%d", tags.trackNumber);
		}

		AudioConverterWriteID3Frame(outFile, ID3_DISK_NUMBER, trackNumberField, FALSE);
		free(trackNumberField);
	}
	if ((tags.year > 0) && (tags.year < ID3_YEAR_MAX))
	{
		// Year must always be a 4 character numeric string
		char * yearField = (char *) calloc(5, sizeof(char));
		sprintf_s(yearField, sizeof(char) * 5, "%04d", tags.year);
		AudioConverterWriteID3Frame(outFile, ID3_YEAR, yearField, FALSE);
		free(yearField);
	}

	free(path);
	return FALSE; // Cannot create MP3 file as of right now
}

// AudioConverterInitializeLAME
// Initialize the LAME MP3 encoder with desired settings
BOOL AudioConverterInitializeLAME(int numChannels, int sampleRate, int bitrate, int mode, int quality)
{
	return FALSE;
}

// AudioConverterWriteID3Frame
// Writes an ID3 tag frame to an MP3 file
BOOL AudioConverterWriteID3Frame(HANDLE mp3File, char * frameID, char * data, BOOL readonly)
{
	// TODO: Make size synchsafe
	DWORD bytesWritten = 0;
	ULONG size = strlen(data);
	char flag1;
	if (readonly)
		flag1 = ID3_FRAME_FLAGS_1_READONLY;
	else
		flag1 = ID3_FRAME_FLAGS_1;
	char flag2 = ID3_FRAME_FLAGS_2;

	WriteFile(mp3File, frameID, 4, &bytesWritten, NULL);
	WriteFile(mp3File, &size, 4, &bytesWritten, NULL);
	WriteFile(mp3File, &flag1, 1, &bytesWritten, NULL);
	WriteFile(mp3File, &flag2, 1, &bytesWritten, NULL);
	WriteFile(mp3File, &data, strlen(data), &bytesWritten, NULL);

	return TRUE;
}