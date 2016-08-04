#pragma once

#include "stdafx.h"

// Constants
#define ID3_FLAGS 0
#define ID3_HEADER_LENGTH 10
#define ID3_IDENTIFIER "ID3"
#define ID3_MAJOR_VERSION 3
#define ID3_MINOR_VERSION 0

// ID3Block Class
// Class for storing an ID3 block to be written at the start of an MP3 file
class ID3Block
{
	private:
		char * identifier;
		char majorVersion;
		char minorVersion;
		unsigned char flags;
		long size;
		ID3_FRAME_NODE * frameList;
	public:
		ID3Block(ID3_FRAME_NODE * frames);
		char * getIdentifier();
		char getMajorVersion();
		char getMinorVersion();
		unsigned char getFlags();
		long getFormattedSize();
		long getTotalSize();
		ID3_FRAME_NODE * getFrameListHead();
};

// Other Methods
ID3_FRAME_NODE * generateID3FrameNode(ID3Frame frame);
