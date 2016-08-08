#pragma once

#include "stdafx.h"

// MP3Tags Class
// Class for formatting and storing MP3 tag data
class MP3Tags
{
	private:
		char * album;
		char * albumSort;
		char * albumArtist;
		char * albumArtistSort;
		char * artist;
		char * artistSort;
		char * bpm;
		char * comment;
		char * compilation;
		char * composer;
		char * composerSort;
		char * disk;
		char * genre;
		char * title;
		char * titleSort;
		char * track;
		char * year;
	public:
		void setAlbum(char * a);
		void setAlbumSort(char * as);
		void setAlbumArtist(char * aa);
		void setAlbumArtistSort(char * aas);
		void setArtist(char * a);
		void setArtistSort(char * as);
		void setBPM(int b);
		void setComment(char * c);
		void setCompilation(BOOL c);
		void setComposer(char * c);
		void setComposerSort(char * cs);
		void setDisk(int d);
		void setDisk(int d, int dTotal);
		void setGenre(char * g);
		void setTitle(char * t);
		void setTitleSort(char * ts);
		void setTrack(int t);
		void setTrack(int t, int tTotal);
		void setYear(int y);
		ID3_FRAME_NODE * convertToID3FrameList();
};
