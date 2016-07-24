#pragma once

#include "stdafx.h"

// Constants
#define DRIVE_FIRST 'A'
#define DRIVE_HANDLE_PREFIX "\\\\.\\"
#define DRIVE_NUM 26
#define DRIVE_SUFFIX ":\\"
#define FRAMES_OFFSET 150
#define FRAMES_PER_SECOND 75

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
CDROM_TOC CDDriveRetrieveTOC(HANDLE cdDrive);

// CD Functions
CD_TRACK * CDGetTracksFromTOC(CDROM_TOC toc);
