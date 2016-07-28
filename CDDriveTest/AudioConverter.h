#pragma once

#include "stdafx.h"

// Structs
typedef struct MP3_TAGS {
	char * album; // TALB
	char * albumArtist; // TPE2
	char * artist; // TPE1
	char * comment; // COMM
	BOOL compilation; // TCMP
	char * composer; // TCOM
	int diskNumber; // TPOS
	int diskNumberTotal; // /TPOS
	char * encodedBy; // TENC
	char * genre; // TCON
	char * title; // TIT2
	int trackNumber; // TRCK
	int trackNumberTotal; // /TRCK
	int year; // TYER
} MP3_TAGS;

// Constants - Extensions
#define EXT_MP3 ".mp3"

// Constants - ID3
#define ID3_FLAGS 0 // 00000000
#define ID3_FRAME_FLAGS_1 96 // 01100000
#define ID3_FRAME_FLAGS_1_READONLY 112 // 01110000
#define ID3_FRAME_FLAGS_2 1 // 00000001
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

// Constants - LAME
#define LAME_CHANNELS_MONO 1
#define LAME_CHANNELS_STEREO 2
#define LAME_QUALITY_HIGH 2
#define LAME_QUALITY_DEFAULT 3
#define LAME_QUALITY_NORMAL 5
#define LAME_QUALITY_FAST 7

// Constants - MP3 Encoding
#define SAMPLES_PER_READ 20

// LAME / MP3 Functions
BOOL AudioConverterInitializeLAME(int numChannels, int sampleRate, int bitrate, MPEG_mode mode, int quality);
BOOL AudioConverterWAVToMP3(char * wavdir, char * wavfilename, BOOL del, char * dir, char * filename, MP3_TAGS tags, int bitrate, MPEG_mode mode, int quality);
BOOL AudioConverterWriteID3Frame(HANDLE mp3File, char * frameID, char * data, BOOL readonly);
