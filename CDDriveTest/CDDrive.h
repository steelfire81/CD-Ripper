#pragma once

#include "stdafx.h"

// Constants - Drive information
#define DRIVE_FIRST 'A'
#define DRIVE_HANDLE_PREFIX "\\\\.\\"
#define DRIVE_NUM 26
#define DRIVE_SUFFIX ":\\"

// Initialization Functions
HANDLE * CDDriveGetHandles();

// Individual Drive Functions
BOOL CDDriveCheckTray(HANDLE cdDrive);
int CDDriveCloseTray(HANDLE cdDrive);
int CDDriveOpenTray(HANDLE cdDrive);
CDROM_TOC CDDriveRetrieveTOC(HANDLE cdDrive);
