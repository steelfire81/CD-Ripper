#include "stdafx.h"

// Static Variables
static lame_global_flags * lameFlags;

//*** INITIALIZATION FUNCTIONS ***//
// AudioConverterInit
// Initialize all necessary audio converters
BOOL AudioConverterInit()
{
	lameFlags = lame_init();
	if (lameFlags == NULL)
		return FALSE;

	return TRUE;
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
	{

	}
	if (tags.artist != NULL)
	{
		
	}
	if (tags.comment != NULL)
	{

	}
	if (tags.compilation)
	{

	}
	if (tags.composer != NULL)
	{

	}
	if ((tags.diskNumber > 0) || (tags.diskNumberTotal > 0))
	{
		
	}
	if (tags.encodedBy != NULL)
	{

	}
	if (tags.genre != NULL)
	{

	}
	if (tags.title != NULL)
	{

	}
	if ((tags.trackNumber > 0) || (tags.trackNumberTotal > 0))
	{

	}
	if (tags.year > 0)
	{

	}

	free(path);
	return FALSE; // Cannot create MP3 file as of right now
}

// AudioConverterInitializeLAME
// Initialize the LAME MP3 encoder with desired settings
BOOL AudioConverterInitializeLAME(int numChannels, int sampleRate, int bitrate, MPEG_mode_e mode, int quality)
{
	lame_set_num_channels(lameFlags, numChannels);
	lame_set_in_samplerate(lameFlags, sampleRate);
	lame_set_brate(lameFlags, bitrate);
	lame_set_mode(lameFlags, mode);
	lame_set_quality(lameFlags, quality);

	int result = lame_init_params(lameFlags);
	return (result >= 0);
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