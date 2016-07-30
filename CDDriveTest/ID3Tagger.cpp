#include "stdafx.h"

//*** FUNCTIONS ***//
// ID3TagFile
// Tag an MP3 file's start with ID3v2 tags, returning number of bytes written
int ID3TagFile(HANDLE file, MP3_TAGS tags)
{
	int totalBytes = 0;
	DWORD bytesWritten = 0;

	// Header
	char majorVersion = ID3_MAJOR_VERSION;
	char minorVersion = ID3_MINOR_VERSION;
	char flags = ID3_FLAGS;
	WriteFile(file, ID3_IDENTIFIER, 3, &bytesWritten, NULL);
	totalBytes += bytesWritten;
	WriteFile(file, &majorVersion, 1, &bytesWritten, NULL);
	totalBytes += bytesWritten;
	WriteFile(file, &minorVersion, 1, &bytesWritten, NULL);
	totalBytes += bytesWritten;
	WriteFile(file, &flags, 1, &bytesWritten, NULL);
	totalBytes += bytesWritten;

	// Frames (One for each tag provided)
	if (tags.artist != NULL)
		totalBytes += ID3WriteFrame(file, ID3_ARTIST, tags.artist, FALSE);
	if (tags.title != NULL)
		totalBytes += ID3WriteFrame(file, ID3_TITLE, tags.title, FALSE);

	return totalBytes;
}

// ID3WriteFrame
// Write a frame to an MP3 file containing a tag, returning number of bytes written
int ID3WriteFrame(HANDLE file, char * frameID, char * data, BOOL readonly)
{
	int totalBytes = 0;
	DWORD bytesWritten = 0;
	
	ULONG size = strlen(data);
	unsigned char flag1;
	if (readonly)
		flag1 = ID3_FRAME_FLAGS_1_READONLY;
	else
		flag1 = ID3_FRAME_FLAGS_1;
	unsigned char flag2 = ID3_FRAME_FLAGS_2;

	WriteFile(file, frameID, 4, &bytesWritten, NULL);
	totalBytes += bytesWritten;
	WriteFile(file, &size, 4, &bytesWritten, NULL);
	totalBytes += bytesWritten;
	WriteFile(file, &flag1, 1, &bytesWritten, NULL);
	totalBytes += bytesWritten;
	WriteFile(file, &flag2, 1, &bytesWritten, NULL);
	totalBytes += bytesWritten;
	WriteFile(file, &data, strlen(data), &bytesWritten, NULL);
	totalBytes += bytesWritten;

	return totalBytes;
}
