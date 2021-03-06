#pragma once

#include "stdafx.h"
using namespace std;

// Constants - Encoding
#define ID3_ENCODING_ISO 0
#define ID3_ENCODING_UNICODE 1

// Constants - Frame IDs
#define ID3_FRAME_ALBUM "TALB"
#define ID3_FRAME_ALBUM_SORT "TSOA"
#define ID3_FRAME_ALBUM_ARTIST "TPE2"
#define ID3_FRAME_ALBUM_ARTIST_SORT "TSO2"
#define ID3_FRAME_ARTIST "TPE1"
#define ID3_FRAME_ARTIST_SORT "TSOP"
#define ID3_FRAME_BPM "TBPM"
#define ID3_FRAME_COMMENT "COMM"
#define ID3_FRAME_COMPILATION "TCMP"
#define ID3_FRAME_COMPOSER "TCOM"
#define ID3_FRAME_COMPOSER_SORT "TSOC"
#define ID3_FRAME_DISK_NUMBER "TPOS"
#define ID3_FRAME_GENRE "TCON"
#define ID3_FRAME_GROUPING "TIT1"
#define ID3_FRAME_TITLE "TIT2"
#define ID3_FRAME_TITLE_SORT "TSOT"
#define ID3_FRAME_TRACK_NUMBER "TRCK"
#define ID3_FRAME_YEAR "TYER"

// Constants - Other
#define ID3_FRAME_FLAGS_1 0 // 00000000
#define ID3_FRAME_FLAGS_2 0 // 00000000
#define ID3_FRAME_HEADER_LENGTH 10

// ID3Frame Class
// Class for storing relevant information for writing an ID3 frame
class ID3Frame
{
	protected:
		string frameID;
		long size;
		unsigned char flag1;
		unsigned char flag2;
		unsigned char encoding;
		string data;
	public:
		ID3Frame(string id, unsigned char e, string d);
		string getFrameID();
		long getFormattedSize();
		unsigned char getFlag1();
		unsigned char getFlag2();
		unsigned char getEncoding();
		string getData();
		long getTotalFrameSize();
		virtual long getDataSize();
};

// Structs
typedef struct ID3_FRAME_NODE {
	ID3Frame * frame;
	ID3_FRAME_NODE * next;
} ID3_FRAME_NODE;

// Other Methods
ID3_FRAME_NODE * generateID3FrameNode(ID3Frame * frame);
