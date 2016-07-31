#pragma once

#include "stdafx.h"

// Constants - ID3
#define ID3_FLAGS 0 // 00000000
#define ID3_FRAME_FLAGS_1 0 // 00000000
#define ID3_FRAME_FLAGS_2 0 // 00000000
#define ID3_IDENTIFIER "ID3"
#define ID3_MAJOR_VERSION 3
#define ID3_MINOR_VERSION 0
#define ID3_YEAR_MAX 10000 // non-inclusive

// Constants - ID3 Tags
#define ID3_ALBUM "TALB"
#define ID3_ALBUM_ARTIST "TPE2"
#define ID3_ARTIST "TPE1"
#define ID3_COMMENT "COMM"
#define ID3_COMPILATION "TCMP"
#define ID3_COMPOSER "TCOM"
#define ID3_DISK_NUMBER "TPOS"
#define ID3_ENCODED_BY "TENC"
#define ID3_GENRE "TCON"
#define ID3_TITLE "TIT2"
#define ID3_TRACK_NUMBER "TRCK"
#define ID3_YEAR "TYER"

// Constants - ID3 Text Encoding
#define ID3_TEXT_ISO 0
#define ID3_TEXT_UNICODE 1

// Structs
typedef struct MP3_TAGS {
	char * artist;
	char * title;
} MP3_TAGS;

// Functions
int ID3TagFile(HANDLE file, MP3_TAGS tags);
int ID3WriteTextFrame(HANDLE file, char * frameID, char * data, BOOL readonly);
