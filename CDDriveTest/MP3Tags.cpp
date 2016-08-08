#include "stdafx.h"

// setAlbum
// Set the album title
void MP3Tags::setAlbum(char * a)
{
	album = a;
}

// setAlbumSort
// Set the sort album
void MP3Tags::setAlbumSort(char * as)
{
	albumSort = as;
}

// setAlbumArtist
// Set the album artist
void MP3Tags::setAlbumArtist(char * aa)
{
	albumArtist = aa;
}

// setAlbumArtistSort
// Set the sort album artist
void MP3Tags::setAlbumArtistSort(char * aas)
{
	albumArtistSort = aas;
}

// setArtist
// Set the artist
void MP3Tags::setArtist(char * a)
{
	artist = a;
}

// setArtistSort
// Set the sort artist
void MP3Tags::setArtistSort(char * as)
{
	artistSort = as;
}

// setBPM
// Set the BPM
void MP3Tags::setBPM(int b)
{
	bpm = (char *) calloc(STRLEN_INT(b) + 1, sizeof(char));
	sprintf_s(bpm, sizeof(char) * (STRLEN_INT(b) + 1), "%d", b);
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

// setComposerSort
// Set the sort composer
void MP3Tags::setComposerSort(char * cs)
{
	composerSort = cs;
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

// setTitleSort
// Set the sort title
void MP3Tags::setTitleSort(char * ts)
{
	titleSort = ts;
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

// convertToID3FrameList
// Converts these MP3 tags to a list of ID3 tags for writing
ID3_FRAME_NODE * MP3Tags::convertToID3FrameList()
{
	ID3_FRAME_NODE * head = (ID3_FRAME_NODE *) malloc(sizeof(ID3_FRAME_NODE));
	ID3_FRAME_NODE * curr = head;

	if (album != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_ALBUM, ID3_ENCODING_ISO, album));
		curr = curr->next;
	}
	if (albumSort != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_ALBUM_SORT, ID3_ENCODING_ISO, albumSort));
		curr = curr->next;
	}
	if (albumArtist != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_ALBUM_ARTIST, ID3_ENCODING_ISO, albumArtist));
		curr = curr->next;
	}
	if (albumArtistSort != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_ALBUM_ARTIST_SORT, ID3_ENCODING_ISO, albumArtistSort));
		curr = curr->next;
	}
	if (artist != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_ARTIST, ID3_ENCODING_ISO, artist));
		curr = curr->next;
	}
	if (artistSort != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_ARTIST_SORT, ID3_ENCODING_ISO, artistSort));
		curr = curr->next;
	}
	if (bpm != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_BPM, ID3_ENCODING_ISO, bpm));
		curr = curr->next;
	}
	if (comment != NULL)
	{
		// TODO: Generate comment-specific frame
		fprintf_s(stderr, "ERROR: NOT CONFIGURED TO WRITE COMMENT FRAME YET\n");
	}
	if (compilation != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_COMPILATION, ID3_ENCODING_ISO, compilation));
		curr = curr->next;
	}
	if (composer != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_COMPOSER, ID3_ENCODING_ISO, composer));
		curr = curr->next;
	}
	if (composerSort != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_COMPOSER_SORT, ID3_ENCODING_ISO, composerSort));
		curr = curr->next;
	}
	if (disk != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_DISK_NUMBER, ID3_ENCODING_ISO, disk));
		curr = curr->next;
	}
	if (genre != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_GENRE, ID3_ENCODING_ISO, genre));
		curr = curr->next;
	}
	if (title != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_TITLE, ID3_ENCODING_ISO, title));
		curr = curr->next;
	}
	if (titleSort != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_TITLE_SORT, ID3_ENCODING_ISO, titleSort));
		curr = curr->next;
	}
	if (track != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_TRACK_NUMBER, ID3_ENCODING_ISO, track));
		curr = curr->next;
	}
	if (year != NULL)
	{
		curr->next = generateID3FrameNode(new ID3Frame(ID3_FRAME_YEAR, ID3_ENCODING_ISO, year));
		curr = curr->next;
	}

	ID3_FRAME_NODE * realHead = head->next;
	free(head);

	return realHead;
}
