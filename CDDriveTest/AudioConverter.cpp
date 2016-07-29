#include "stdafx.h"

// Static Variables
static lame_global_flags * lameFlags;

//*** MP3 FUNCTIONS ***//

// AudioConverterInitializeLAME
// Initialize the LAME MP3 encoder with desired settings
BOOL AudioConverterInitializeLAME(int numChannels, int sampleRate, int bitrate, MPEG_mode mode, int quality)
{
	lameFlags = lame_init();
	if (lameFlags == NULL)
	{
		fprintf_s(stderr, "ERROR INITIALIZING LAME FLAGS\n");
		return FALSE;
	}

	lame_set_num_channels(lameFlags, numChannels);
	lame_set_in_samplerate(lameFlags, sampleRate);
	lame_set_brate(lameFlags, bitrate);
	lame_set_mode(lameFlags, mode);
	lame_set_quality(lameFlags, quality);

	int result = lame_init_params(lameFlags);
	if (result >= 0)
		return TRUE;
	else
	{
		fprintf_s(stderr, "ERROR INITIALIZING LAME PARAMS\n");
		return FALSE;
	}
}

// AudioConverterWAVToMP3
// Convert a WAV file to an MP3 file
BOOL AudioConverterWAVToMP3(char * wavdir, char * wavfilename, BOOL del, char * dir, char * filename, MP3_TAGS tags, int bitrate, MPEG_mode mode, int quality)
{
	char * wavPath = getFilePath(wavdir, wavfilename, EXT_WAV);
	HANDLE wavFile = CreateFile(cStringToLPCWSTR(wavPath), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (wavFile == INVALID_HANDLE_VALUE)
	{
		fprintf_s(stderr, "ERROR: COULD NOT LOAD WAV\n");
		return FALSE;
	}

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

	DWORD bytesRead = 0;
	DWORD bytesWritten = 0;

	// Extract info from WAV header
	short format;
	short numChannels;
	long sampleRate;
	long songSize;
	unsigned char * buff = (unsigned char *) calloc(20, sizeof(unsigned char));
	ReadFile(wavFile, buff, 20, &bytesRead, NULL); // First 20 bytes not needed
	ReadFile(wavFile, &format, 2, &bytesRead, NULL); // Format (MUST == WAVE_FORMAT_PCM)
	if (format != WAVE_FORMAT_PCM)
	{
		fprintf_s(stderr, "ERROR: WAV FORMAT NOT PCM; CAN'T CONVERT\n");
		return FALSE;
	}
	ReadFile(wavFile, &numChannels, 2, &bytesRead, NULL);
	ReadFile(wavFile, &sampleRate, 4, &bytesRead, NULL);
	ReadFile(wavFile, buff, 12, &bytesRead, NULL); // Next 12 bytes not needed
	ReadFile(wavFile, &songSize, 4, &bytesRead, NULL);
	free(buff);

	// DEBUG
	printf("%d %d %ld %ld\n", format, numChannels, sampleRate, songSize);

	// Initialize lame
	lame_global_flags * lameFlags = lame_init();
	if (lameFlags == NULL)
	{
		fprintf_s(stderr, "ERROR INITIALIZING LAME FLAGS\n");
		return FALSE;
	}
	lame_set_num_channels(lameFlags, numChannels);
	lame_set_in_samplerate(lameFlags, sampleRate);
	lame_set_brate(lameFlags, bitrate);
	lame_set_mode(lameFlags, mode);
	lame_set_quality(lameFlags, quality);
	int result = lame_init_params(lameFlags);
	if (result < 0)
	{
		fprintf_s(stderr, "ERROR INITIALIZING WITH LAME FLAGS\n");
		return FALSE;
	}

	// Encode file piece by piece
	int totalSamples = songSize / (WAV_BITS_PER_SAMPLE / 8);
	int mp3Buffsize = 1.25 * (SAMPLES_PER_READ * 2) + 7200; // from LAME documentation
	unsigned char * mp3Buffer = (unsigned char *) calloc(mp3Buffsize, sizeof(unsigned char));
	char * wavBuffer = (char *) calloc(SAMPLES_PER_READ * (WAV_BITS_PER_SAMPLE / 2), sizeof(char));
	long i;
	for (i = 0; i < (totalSamples / SAMPLES_PER_READ); i++)
	{
		BOOL encodeResult = ReadFile(wavFile, wavBuffer, SAMPLES_PER_READ * (WAV_BITS_PER_SAMPLE / 8), &bytesRead, NULL);
		if (!encodeResult)
		{
			fprintf_s(stderr, "ERROR READING FROM WAV TO CONVERSION BUFFER\n");
			return FALSE;
		}

		int lameResult = lame_encode_buffer(lameFlags, (short *) wavBuffer, (short *) wavBuffer,
			SAMPLES_PER_READ, mp3Buffer, mp3Buffsize);
		if (lameResult < 0)
		{
			fprintf_s(stderr, "ERROR ENCODING WAV BUFFER TO LAME MP3 BUFFER (%d)\n", lameResult);
			return FALSE;
		}

		encodeResult = WriteFile(outFile, mp3Buffer, lameResult, &bytesWritten, NULL);
		if (!encodeResult)
		{
			fprintf_s(stderr, "ERROR WRITING FROM WAV BUFFER TO MP3 FILE\n");
			return FALSE;
		}
	}
	free(mp3Buffer);
	free(wavBuffer);

	long leftoverSamples = totalSamples % SAMPLES_PER_READ;
	if (leftoverSamples > 0)
	{
		int mp3LeftoverBuffsize = 1.25 * leftoverSamples + 7200;
		unsigned char * mp3LeftoverBuffer = (unsigned char *) calloc(mp3LeftoverBuffsize, sizeof(unsigned char));
		short * wavLeftoverBuffer = (short *) calloc(leftoverSamples, sizeof(short));

		BOOL encodeResult = ReadFile(wavFile, wavLeftoverBuffer, leftoverSamples * (WAV_BITS_PER_SAMPLE / 8), &bytesRead, NULL);
		if (!encodeResult)
		{
			fprintf_s(stderr, "ERROR READING LEFTOVER SECTORS FROM WAV\n");
			return FALSE;
		}

		int lameResult = lame_encode_buffer(lameFlags, wavLeftoverBuffer, wavLeftoverBuffer,
			leftoverSamples, mp3LeftoverBuffer, mp3LeftoverBuffsize);
		if (lameResult < 0)
		{
			fprintf_s(stderr, "ERROR ENCODING LEFTOVER WAV BUFFER TO LAME MP3 BUFFER (%d)\n",
				lameResult);
			return FALSE;
		}

		encodeResult = WriteFile(outFile, mp3LeftoverBuffer, lameResult, &bytesWritten, NULL);
		if (!encodeResult)
		{
			fprintf_s(stderr, "ERROR WRITING LEFTOVER WAV BUFFER TO MP3 FILE\n");
			return FALSE;
		}

		free(mp3LeftoverBuffer);
		free(wavLeftoverBuffer);
	}

	printf("Finished extracting WAV data, tagging...\n"); // DEBUG

	/*
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
		char * yearField = (char *)calloc(5, sizeof(char));
		sprintf_s(yearField, sizeof(char) * 5, "%04d", tags.year);
		AudioConverterWriteID3Frame(outFile, ID3_YEAR, yearField, FALSE);
		free(yearField);
	}
	*/

	free(path);
	return CloseHandle(outFile); // Cannot create MP3 file as of right now
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