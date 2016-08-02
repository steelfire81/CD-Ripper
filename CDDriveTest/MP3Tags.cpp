#include "stdafx.h"

// setAlbum
// Set the album title
void MP3Tags::setAlbum(char * a)
{
	album = a;
}

// setAlbumArtist
// Set the album artist
void MP3Tags::setAlbumArtist(char * aa)
{
	albumArtist = aa;
}

// setArtist
// Set the artist
void MP3Tags::setArtist(char * a)
{
	artist = a;
}

// setComment
// Set the comment
void MP3Tags::setComment(char * c)
{
	comment = c;
}

// setCompilation
// Set whether or not this is a compilation
void MP3Tags::setCompilation(BOOL c)
{
	if (c)
		compilation = "1";
}

// setComposer
// Set the composer
void MP3Tags::setComposer(char * c)
{
	composer = c;
}

// setDisk
// Set the disk number
void MP3Tags::setDisk(int d)
{
	if (d > 0)
	{
		disk = (char *) calloc(STRLEN_INT(d) + 1, sizeof(char));
		sprintf_s(disk, sizeof(char) * (STRLEN_INT(d) + 1), "%d", d);
	}
}

// setDisk
// Set the disk number and total number of disks
void MP3Tags::setDisk(int d, int dTotal)
{
	if ((d > 0) && (dTotal > 0) && (dTotal >= d))
	{
		disk = (char *) calloc(STRLEN_INT(d) + STRLEN_INT(dTotal) + 2, sizeof(char));
		sprintf_s(disk, sizeof(char) * (STRLEN_INT(d) + STRLEN_INT(dTotal) + 2), "%d/%d", d,
			dTotal);
	}
}

// setEncoder
// Set the encoder
void MP3Tags::setEncoder(char * e)
{
	encoder = e;
}

// setGenre
// Set the genre
void MP3Tags::setGenre(char * g)
{
	genre = g;
}

// setTitle
// Set the title
void MP3Tags::setTitle(char * t)
{
	title = t;
}

// setTrack
// Set the track number
void MP3Tags::setTrack(int t)
{
	if (t > 0)
	{
		track = (char *) calloc(STRLEN_INT(t) + 1, sizeof(char));
		sprintf_s(track, sizeof(char) * (STRLEN_INT(t) + 1), "%d", t);
	}
}

// setTrack
// Set the track number and total number of tracks
void MP3Tags::setTrack(int t, int tTotal)
{
	if ((t > 0) && (tTotal > 0) && (tTotal >= t))
	{
		track = (char *) calloc(STRLEN_INT(t) + STRLEN_INT(tTotal) + 2, sizeof(char));
		sprintf_s(track, sizeof(char) * (STRLEN_INT(t) + STRLEN_INT(tTotal) + 2), "%d/%d", t,
			tTotal);
	}
}

// setYear
// Set the year
void MP3Tags::setYear(int y)
{
	if ((y > 0) && (y < 10000))
	{
		year = (char *) calloc(5, sizeof(char));
		sprintf_s(year, sizeof(char) * 5, "%04d", y);
	}
}