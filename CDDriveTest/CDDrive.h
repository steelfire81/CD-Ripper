#pragma once

#include "stdafx.h"

// Constants
#define BYTES_PER_SECTOR 2352
#define DISK_OFFSET_MULTIPLIER 2048
#define DRIVE_FIRST 'A'
#define DRIVE_HANDLE_PREFIX "\\\\.\\"
#define DRIVE_NUM 26
#define DRIVE_SUFFIX ":\\"
#define FRAMES_OFFSET 150
#define FRAMES_PER_SECOND 75
#define SECTORS_PER_READ 10

// Structs
typedef struct CD_TRACK {
	ULONG startAddress;
	ULONG duration;
} CD_TRACK;

// Initialization Functions
HANDLE * CDDriveGetHandles();

// Individual Drive Functions
BOOL CDDriveCheckTray(HANDLE cdDrive);
BOOL CDDriveCloseTray(HANDLE cdDrive);
BOOL CDDriveOpenTray(HANDLE cdDrive);
unsigned char * CDDriveReadRawTrack(HANDLE cdDrive, CD_TRACK track);
CDROM_TOC CDDriveRetrieveTOC(HANDLE cdDrive);

// CD Functions
CD_TRACK * CDGetTracksFromTOC(CDROM_TOC toc);
