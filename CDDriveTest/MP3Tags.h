#pragma once

#include "stdafx.h"
using namespace std;

// MP3Tags Class
// Class for formatting and storing MP3 tag data
class MP3Tags
{
	private:
		unordered_map<string, string> tags;
		void setTag(string tag, string value);
	public:
		void setAlbum(string a);
		void setAlbumSort(string as);
		void setAlbumArtist(string aa);
		void setAlbumArtistSort(string aas);
		void setArtist(string a);
		void setArtistSort(string as);
		void setBPM(int b);
		void setComment(string c);
		void setCompilation(bool c);
		void setComposer(string c);
		void setComposerSort(string cs);
		void setDisk(int d);
		void setDisk(int d, int dTotal);
		void setGenre(string g);
		void setGrouping(string g);
		void setTitle(string t);
		void setTitleSort(string ts);
		void setTrack(int t);
		void setTrack(int t, int tTotal);
		void setYear(int y);
		ID3_FRAME_NODE * convertToID3FrameList();
};
