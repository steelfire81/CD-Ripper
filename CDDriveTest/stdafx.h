// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// My includes
#include <Windows.h>
#include <Ntddcdrm.h>
#include <math.h>
#include "../include/lame.h"
#include "ID3Frame.h"
#include "ID3CommentFrame.h"
#include "ID3Block.h"
#include "MP3Tags.h"
#include "CDDrive.h"

// Constants
#define SYNCHSAFE_MASK 127 // 01111111

// Macro Functions
#define STRLEN_INT(x) (x == 0 ? 1 : ((int) (log10(x) + 1)))

// Method declarations
LPCWSTR cStringToLPCWSTR(const char * cString);
char * getFilePath(char * dir, char * filename, char * ext);
long reverseLongBytewiseEndian(long l);
long longToSynchsafe(long l);
long formatLongForID3Tag(long l);
