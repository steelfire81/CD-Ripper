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
		char * setAlbum(char * a);
		char * setAlbumSort(char * as);
		char * setAlbumArtist(char * aa);
		char * setAlbumArtistSort(char * aas);
		char * setArtist(char * a);
		char * setArtistSort(char * as);
		char * setBPM(int b);
		char * setComment(char * c);
		char * setCompilation(BOOL c);
		char * setComposer(char * c);
		char * setComposerSort(char * cs);
		char * setDisk(int d);
		char * setDisk(int d, int dTotal);
		char * setGenre(char * g);
		char * setTitle(char * t);
		char * setTitleSort(char * ts);
		char * setTrack(int t);
		char * setTrack(int t, int tTotal);
		char * setYear(int y);
		ID3_FRAME_NODE * convertToID3FrameList();
};
