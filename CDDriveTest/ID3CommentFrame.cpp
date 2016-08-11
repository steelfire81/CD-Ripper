
#include "stdafx.h"

// Constructor
// Create a comment frame
ID3CommentFrame::ID3CommentFrame(unsigned char e, char * l, char * d) : ID3Frame(ID3_FRAME_COMMENT, e, d)
{
	size = strlen(d) + 5; // 1 encoding + 3 language + 1 content descriptor = 5 additional bytes
	data = (char *) calloc(strlen(d) + 5, sizeof(char));
	sprintf_s(data, sizeof(char) * (strlen(d) + 5), "%s%u%s", l, ID3_COMMENT_DESCRIPTOR_DEFAULT, d);
	data[3] = (char) ID3_COMMENT_DESCRIPTOR_DEFAULT;
}

long ID3CommentFrame::getDataSize()
{
	return size - 1; // Exclude encoding byte
}
