#pragma once

#include "stdafx.h"

// Constants - Extensions
#define EXT_MP3 ".mp3"
#define EXT_WAV ".wav"

// Constants
#define BYTES_PER_SECTOR 2352
#define CD_BITS_PER_SAMPLE 16
#define CD_NUM_CHANNELS 2
#define CD_SAMPLE_RATE 44100
#define DISK_OFFSET_MULTIPLIER 2048
#define DRIVE_FIRST 'A'
#define DRIVE_HANDLE_PREFIX "\\\\.\\"
#define DRIVE_NUM 26
#define DRIVE_SUFFIX ":\\"
#define FRAMES_OFFSET 150
#define FRAMES_PER_SECOND 75
#define LAME_LEFTOVER_BUFFSIZE 7200
#define LAME_VBR_FRAME_BUFFSIZE 7200
#define SAMPLES_PER_READ 20
#define SECTORS_PER_READ 10

// Constants - LAME
#define LAME_QUALITY_HIGH 2
#define LAME_QUALITY_DEFAULT 3
#define LAME_QUALITY_NORMAL 5
#define LAME_QUALITY_LOW 7

// Constants - WAV Header
#define WAV_BITS_PER_SAMPLE 16
#define WAV_CD_SAMPLE_RATE 44100
#define WAV_CHANNELS_MONO 1
#define WAV_CHANNELS_STEREO 2
#define WAV_CHUNK_SIZE_OFFSET 36
#define WAV_CHUNKID "RIFF"
#define WAV_FORMAT "WAVE"
#define WAV_SUBCHUNK_1_ID "fmt "
#define WAV_SUBCHUNK_1_SIZE 16
#define WAV_SUBCHUNK_2_ID "data"

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
BOOL CDDriveExtractTrackToMP3(HANDLE cdDrive, CD_TRACK track, char * dir, char * filename, MP3Tags * tags, CDProgressListener * listener);
BOOL CDDriveExtractTrackToWAV(HANDLE cdDrive, CD_TRACK track, char * dir, char * filename, CDProgressListener * listener);
BOOL CDDriveOpenTray(HANDLE cdDrive);
unsigned char * CDDriveReadRawTrack(HANDLE cdDrive, CD_TRACK track);
CDROM_TOC CDDriveRetrieveTOC(HANDLE cdDrive);

// CD Functions
CD_TRACK * CDGetTracksFromTOC(CDROM_TOC toc);
