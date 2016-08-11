#pragma once

#include "stdafx.h"

// Constants
#define ID3_COMMENT_DESCRIPTOR_DEFAULT 0
#define ID3_COMMENT_LANGUAGE_ENGLISH "eng"

class ID3CommentFrame : public ID3Frame
{
	public:
		ID3CommentFrame(unsigned char e, char * l, char * d);
		virtual long getDataSize();
};
