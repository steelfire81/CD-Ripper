#include "stdafx.h"

// Constructor
// Convert and store tag data as an ID3Frame
ID3Frame::ID3Frame(string id, unsigned char e, string d)
{
	frameID = id;
	encoding = e;
	data = d;

	size = d.length() + 1;
	flag1 = ID3_FRAME_FLAGS_1;
	flag2 = ID3_FRAME_FLAGS_2;
}

// getFrameID
// Return this frame's 4 character identifier
string ID3Frame::getFrameID()
{
	return frameID;
}

// getFormattedSize
// Return this frame's size in reverse endian
long ID3Frame::getFormattedSize()
{
	return formatLongForID3Tag(size);
}

// getFlag1
// Return this frame's flag 1
unsigned char ID3Frame::getFlag1()
{
	return flag1;
}

// getFlag2
// Return this frame's flag 2
unsigned char ID3Frame::getFlag2()
{
	return flag2;
}

// getEncoding
// Return this frame's data encoding
unsigned char ID3Frame::getEncoding()
{
	return encoding;
}

// getData
// Return this frame's data block (minus encoding)
string ID3Frame::getData()
{
	return data;
}

// getTotalFrameSize
// Return the total size of this frame (for writing the ID3 header)
long ID3Frame::getTotalFrameSize()
{
	return size + ID3_FRAME_HEADER_LENGTH;
}

// getDataSize
// Return the length of the data in this frame
long ID3Frame::getDataSize()
{
	return data.length();
}

//*** OTHER METHODS ***//
// generateID3FrameNode
// Create an ID3FrameNode from an ID3Frame
ID3_FRAME_NODE * generateID3FrameNode(ID3Frame * frame)
{
	ID3_FRAME_NODE * node = (ID3_FRAME_NODE *) malloc(sizeof(ID3_FRAME_NODE));
	node->frame = frame;
	node->next = NULL;
	return node;
}
