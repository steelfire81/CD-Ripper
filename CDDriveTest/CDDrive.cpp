#include "stdafx.h"

//*** INITIALIZATION FUNCTIONS **//
// CDDriveGetHandles
// Retrieves a list of handles on CD drives
HANDLE * CDDriveGetHandles()
{
	HANDLE * cdDriveHandles = (HANDLE *) calloc(DRIVE_NUM + 1, sizeof(HANDLE));
	DWORD drives = GetLogicalDrives();
	int numCDDrives = 0;
	char currDriveLetter = DRIVE_FIRST;

	for (int i = 0; i < DRIVE_NUM; i++)
	{
		char * driveName = (char *)calloc(strlen(DRIVE_SUFFIX) + 2, sizeof(char));
		sprintf_s(driveName, sizeof(driveName), "%c%s", currDriveLetter, DRIVE_SUFFIX);
		if (GetDriveType(cStringToLPCWSTR(driveName)) == DRIVE_CDROM)
		{
			char * driveHandleName = (char *) calloc(strlen(DRIVE_HANDLE_PREFIX) + 3, sizeof(char));
			sprintf_s(driveHandleName, sizeof(char) * (strlen(DRIVE_HANDLE_PREFIX) + 3),
				"%s%c:", DRIVE_HANDLE_PREFIX, currDriveLetter);
			cdDriveHandles[numCDDrives] = CreateFile(cStringToLPCWSTR(driveHandleName), GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				NULL);

			if ((cdDriveHandles[numCDDrives] == INVALID_HANDLE_VALUE) || (GetLastError() != NO_ERROR))
				fprintf_s(stderr, "ERROR GETTING HANDLE ON %s\n", driveName);
			else
				numCDDrives++;

			free(driveHandleName);
		}

		free(driveName);
		currDriveLetter++;
	}

	realloc(cdDriveHandles, sizeof(HANDLE) * (numCDDrives));
	return cdDriveHandles;
}

//*** INDIVIDUAL DRIVE FUNCTIONS ***//
// CDDriveCheckTray
// Checks if anything is in the CD tray
BOOL CDDriveCheckTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_STORAGE_CHECK_VERIFY, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// CDDriveCloseTray
// Closes the tray of the given CD drive
BOOL CDDriveCloseTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// CDDriveExtractTrack
// Extracts a track from a CD drive and saves it as a WAV
BOOL CDDriveExtractTrackToWAV(HANDLE cdDrive, CD_TRACK track, char * dir, char * filename)
{
	char * path = getFilePath(dir, filename, EXT_WAV);
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

	// RIFF Chunk
	DWORD bytesWritten = 0;
	ULONG chunkSize = (track.duration * BYTES_PER_SECTOR) + WAV_CHUNK_SIZE_OFFSET;

	WriteFile(outFile, WAV_CHUNKID, 4, &bytesWritten, NULL);
	WriteFile(outFile, &chunkSize, 4, &bytesWritten, NULL);
	WriteFile(outFile, WAV_FORMAT, 4, &bytesWritten, NULL);

	// Format Chunk
	ULONG subchunk1Size = WAV_SUBCHUNK_1_SIZE;
	USHORT audioFormat = WAVE_FORMAT_PCM;
	USHORT numChannels = WAV_CHANNELS_STEREO;
	ULONG sampleRate = WAV_CD_SAMPLE_RATE;
	USHORT bitsPerSample = WAV_BITS_PER_SAMPLE;
	ULONG byteRate = sampleRate * numChannels * (bitsPerSample / 8);
	USHORT blockAlign = numChannels * (bitsPerSample / 8);

	WriteFile(outFile, WAV_SUBCHUNK_1_ID, 4, &bytesWritten, NULL);
	WriteFile(outFile, &subchunk1Size, 4, &bytesWritten, NULL);
	WriteFile(outFile, &audioFormat, 2, &bytesWritten, NULL);
	WriteFile(outFile, &numChannels, 2, &bytesWritten, NULL);
	WriteFile(outFile, &sampleRate, 4, &bytesWritten, NULL);
	WriteFile(outFile, &byteRate, 4, &bytesWritten, NULL);
	WriteFile(outFile, &blockAlign, 2, &bytesWritten, NULL);
	WriteFile(outFile, &bitsPerSample, 2, &bytesWritten, NULL);

	// Data Chunk
	ULONG subchunk2Size = track.duration * BYTES_PER_SECTOR;

	WriteFile(outFile, WAV_SUBCHUNK_2_ID, 4, &bytesWritten, NULL);
	WriteFile(outFile, &subchunk2Size, 4, &bytesWritten, NULL);

	// Read & Write Data
	unsigned char * data = (unsigned char *)calloc(SECTORS_PER_READ * BYTES_PER_SECTOR, sizeof(unsigned char));
	DWORD bytesReturned = 0;

	RAW_READ_INFO readRequest;
	readRequest.SectorCount = SECTORS_PER_READ;
	readRequest.TrackMode = CDDA;

	long i;
	for (i = 0; i < (track.duration / SECTORS_PER_READ); i++)
	{
		readRequest.DiskOffset.QuadPart = (track.startAddress + (i * SECTORS_PER_READ)) * DISK_OFFSET_MULTIPLIER;
		BOOL result = DeviceIoControl(cdDrive, IOCTL_CDROM_RAW_READ, &readRequest,
			sizeof(RAW_READ_INFO), data, SECTORS_PER_READ * BYTES_PER_SECTOR,
			&bytesReturned, NULL);
		if (!result)
			return FALSE;

		WriteFile(outFile, (LPCVOID)&data[0], SECTORS_PER_READ * BYTES_PER_SECTOR, &bytesWritten, NULL);
	}

	ULONG leftoverSectors = track.duration % SECTORS_PER_READ;
	if (leftoverSectors > 0)
	{
		readRequest.SectorCount = leftoverSectors;
		readRequest.DiskOffset.QuadPart = (track.startAddress + (i * SECTORS_PER_READ)) * DISK_OFFSET_MULTIPLIER;
		BOOL result = DeviceIoControl(cdDrive, IOCTL_CDROM_RAW_READ, &readRequest,
			sizeof(RAW_READ_INFO), data, leftoverSectors * BYTES_PER_SECTOR,
			&bytesReturned, NULL);
		if (!result)
			return FALSE;
		WriteFile(outFile, (LPCVOID)&data[0], leftoverSectors * BYTES_PER_SECTOR, &bytesWritten, NULL);
	}

	free(path);
	free(data);

	return CloseHandle(outFile);
}

// CDDriveOpenTray
// Opens the tray of the given CD drive
BOOL CDDriveOpenTray(HANDLE cdDrive)
{
	DWORD bytesReturned = 0;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0,
		&bytesReturned, NULL);
	return result;
}

// CDDriveReadRawTrack
// Reads raw data from a CD track
unsigned char * CDDriveReadRawTrack(HANDLE cdDrive, CD_TRACK track)
{
	unsigned char * data = (unsigned char *)calloc(track.duration * BYTES_PER_SECTOR,
		sizeof(unsigned char));
	DWORD bytesReturned = 0;
	
	RAW_READ_INFO readRequest;
	readRequest.SectorCount = SECTORS_PER_READ;
	readRequest.TrackMode = CDDA;

	long i;
	for (i = 0; i < (track.duration / SECTORS_PER_READ); i++)
	{
		readRequest.DiskOffset.QuadPart = (track.startAddress + (i * SECTORS_PER_READ)) * DISK_OFFSET_MULTIPLIER;
		BOOL result = DeviceIoControl(cdDrive, IOCTL_CDROM_RAW_READ, &readRequest,
			sizeof(RAW_READ_INFO), data + (i * SECTORS_PER_READ * BYTES_PER_SECTOR),
			SECTORS_PER_READ * BYTES_PER_SECTOR, &bytesReturned, NULL);
		if (!result)
		{
			fprintf_s(stderr, "ERROR READING TRACK RAW DATA\n");
			return NULL;
		}
	}

	// Read leftover sectors
	ULONG leftoverSectors = track.duration % SECTORS_PER_READ;
	readRequest.SectorCount = leftoverSectors;
	readRequest.DiskOffset.QuadPart = (track.startAddress + (i * SECTORS_PER_READ)) * DISK_OFFSET_MULTIPLIER;
	BOOL result = DeviceIoControl(cdDrive, IOCTL_CDROM_RAW_READ, &readRequest,
		sizeof(RAW_READ_INFO), data + (i * SECTORS_PER_READ * BYTES_PER_SECTOR),
		leftoverSectors * BYTES_PER_SECTOR, &bytesReturned, NULL);
	if (!result)
	{
		fprintf_s(stderr, "ERROR READING TRACK RAW DATA (END)\n");
		return NULL;
	}

	printf("\t%ld bytes\n", track.duration * BYTES_PER_SECTOR); // DEBUG

	return data;
}

// CDDriveRetrieveTOC
// Returns the table of contents of the CD in the given drive
CDROM_TOC CDDriveRetrieveTOC(HANDLE cdDrive)
{
	CDROM_READ_TOC_EX cdTOCRequest;
	cdTOCRequest.Format = CDROM_READ_TOC_EX_FORMAT_TOC;
	cdTOCRequest.Reserved1 = 0;
	cdTOCRequest.Reserved2 = 0;
	cdTOCRequest.Reserved3 = 0;
	cdTOCRequest.SessionTrack = 0;
	cdTOCRequest.Msf = true;

	CDROM_TOC tableOfContents;
	DWORD bytesReturned = 0;

	BOOL result = DeviceIoControl(cdDrive, IOCTL_CDROM_READ_TOC_EX, &cdTOCRequest,
		sizeof(CDROM_READ_TOC_EX), &tableOfContents, sizeof(CDROM_TOC),
			&bytesReturned, NULL);
	if (!result)
		fprintf_s(stderr, "ERROR RETRIEVING CD TOC\n");

	return tableOfContents;
}

//*** CD FUNCTIONS ***//
// CDGetTracksFromTOC
// Returns a list of CD_TRACKs from a given table of contents
CD_TRACK * CDGetTracksFromTOC(CDROM_TOC toc)
{
	int numTracks = toc.LastTrack - toc.FirstTrack + 1;
	CD_TRACK * tracks = (CD_TRACK *) calloc(numTracks, sizeof(CD_TRACK));

	for (int i = 0; i < numTracks; i++)
	{
		TRACK_DATA td = toc.TrackData[i];
		tracks[i].startAddress = td.Address[0] * 60 * 60 * FRAMES_PER_SECOND +
			td.Address[1] * 60 * FRAMES_PER_SECOND + td.Address[2] * FRAMES_PER_SECOND +
			td.Address[3] - FRAMES_OFFSET;
	}

	for (int i = 0; i < numTracks - 1; i++)
		tracks[i].duration = tracks[i + 1].startAddress - tracks[i].startAddress;

	// For last track, find start of one beyond final track
	TRACK_DATA tdEnd = toc.TrackData[numTracks];
	ULONG cdEnd = tdEnd.Address[0] * 60 * 60 * FRAMES_PER_SECOND +
		tdEnd.Address[1] * 60 * FRAMES_PER_SECOND + tdEnd.Address[2] * FRAMES_PER_SECOND +
		tdEnd.Address[3] - FRAMES_OFFSET;
	tracks[numTracks - 1].duration = cdEnd - tracks[numTracks - 1].startAddress;

	return tracks;
}
