
#include "stdafx.h"

// Constructor
// Create a comment frame
ID3CommentFrame::ID3CommentFrame(unsigned char e, string lang, string d) : ID3Frame(ID3_FRAME_COMMENT, e, d)
{
	size = d.length() + 5; // 1 encoding + 3 language + 1 content descriptor = 5 additional bytes
	data = lang;
	data.append(to_string(ID3_COMMENT_DESCRIPTOR_DEFAULT));
	data.append(d);
}

long ID3CommentFrame::getDataSize()
{
	return size - 1; // Exclude encoding byte
}
