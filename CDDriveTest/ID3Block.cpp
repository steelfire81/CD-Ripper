#include "stdafx.h"

// Constructor
// Create an ID3 block from a list of ID3 frames
ID3Block::ID3Block(ID3_FRAME_NODE * frames)
{
	identifier = ID3_IDENTIFIER;
	majorVersion = ID3_MAJOR_VERSION;
	minorVersion = ID3_MINOR_VERSION;
	flags = ID3_FLAGS;
	frameList = frames;

	size = ID3_HEADER_LENGTH;
	ID3_FRAME_NODE * curr = frameList;
	while (curr != NULL)
	{
		size += curr->frame->getTotalFrameSize();
		curr = curr->next;
	}
}

// getIdentifier
// Return the 3-character identifier to be written at the start of the ID3 block
char * ID3Block::getIdentifier()
{
	return identifier;
}

// getMajorVersion
// Return the major version of ID3 tags
char ID3Block::getMajorVersion()
{
	return majorVersion;
}

// getMinorVersion
// Return the minor version of ID3 tags
char ID3Block::getMinorVersion()
{
	return minorVersion;
}

// getFlags
// Return the ID3 flags
unsigned char ID3Block::getFlags()
{
	return flags;
}

// getFormattedSize
// Return the size of the block in reverse endian for writing
long ID3Block::getFormattedSize()
{
	return reverseLongBytewiseEndian(size - ID3_HEADER_LENGTH);
}

// getTotalSize
// Return the total size of this ID3 block including the header
long ID3Block::getTotalSize()
{
	return size;
}

// getFrameListHead
// Return the start of the ID3 tag frame list
ID3_FRAME_NODE * ID3Block::getFrameListHead()
{
	return frameList;
}
