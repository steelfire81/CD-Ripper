#pragma once

class MP3Tags
{
	private:
		char * album;
		char * albumArtist;
		char * artist;
		char * comment;
		char * compilation;
		char * composer;
		char * disk;
		char * encoder;
		char * genre;
		char * title;
		char * track;
		char * year;
	public:
		void setAlbum(char * a);
		void setAlbumArtist(char * aa);
		void setArtist(char * a);
		void setComment(char * c);
		void setCompilation(BOOL c);
		void setComposer(char * c);
		void setDisk(int d);
		void setDisk(int d, int dTotal);
		void setEncoder(char * e);
		void setGenre(char * g);
		void setTitle(char * t);
		void setTrack(int t);
		void setTrack(int t, int tTotal);
		void setYear(int y);
};