#pragma once

#include "stdafx.h"
using namespace std;

// Constants
#define ID3_COMMENT_DESCRIPTOR_DEFAULT 0
#define ID3_COMMENT_LANGUAGE_ENGLISH "eng"

class ID3CommentFrame : public ID3Frame
{
	public:
		ID3CommentFrame(unsigned char e, string lang, string d);
		virtual long getDataSize();
};
