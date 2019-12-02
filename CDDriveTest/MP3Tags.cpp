#include "stdafx.h"

// TODO: Refactor this to make adding new fields simpler

// setTag
// Set the value for a given tag
void MP3Tags::setTag(string tag, string value)
{
	tags.insert_or_assign(tag, value);
}

// setAlbum
// Set the album title
void MP3Tags::setAlbum(string a)
{
	setTag(ID3_FRAME_ALBUM, a);
}

// setAlbumSort
// Set the sort album
void MP3Tags::setAlbumSort(string as)
{
	setTag(ID3_FRAME_ALBUM_SORT, as);
}

// setAlbumArtist
// Set the album artist
void MP3Tags::setAlbumArtist(string aa)
{
	setTag(ID3_FRAME_ALBUM_ARTIST, aa);
}

// setAlbumArtistSort
// Set the sort album artist
void MP3Tags::setAlbumArtistSort(string aas)
{
	setTag(ID3_FRAME_ALBUM_ARTIST_SORT, aas);
}

// setArtist
// Set the artist
void MP3Tags::setArtist(string a)
{
	setTag(ID3_FRAME_ARTIST, a);
}

// setArtistSort
// Set the sort artist
void MP3Tags::setArtistSort(string as)
{
	setTag(ID3_FRAME_ARTIST_SORT, as);
}

// setBPM
// Set the BPM
void MP3Tags::setBPM(int b)
{
	setTag(ID3_FRAME_BPM, to_string(b));
}

// setComment
// Set the comment
void MP3Tags::setComment(string c)
{
	setTag(ID3_FRAME_COMMENT, c);
}

// setCompilation
// Set whether or not this is a compilation
void MP3Tags::setCompilation(bool c)
{
	if (c)
		setTag(ID3_FRAME_COMPILATION, "1");
}

// setComposer
// Set the composer
void MP3Tags::setComposer(string c)
{
	setTag(ID3_FRAME_COMPOSER, c);
}

// setComposerSort
// Set the sort composer
void MP3Tags::setComposerSort(string cs)
{
	setTag(ID3_FRAME_COMPOSER_SORT, cs);
}

// setDisk
// Set the disk number
void MP3Tags::setDisk(int d)
{
	if (d > 0)
	{
		setTag(ID3_FRAME_DISK_NUMBER, to_string(d));
	}
}

// setDisk
// Set the disk number and total number of disks
void MP3Tags::setDisk(int d, int dTotal)
{
	if ((d > 0) && (dTotal > 0) && (dTotal >= d))
	{
		string disk = to_string(d);
		disk.append("/");
		disk.append(to_string(dTotal));
		setTag(ID3_FRAME_DISK_NUMBER, disk);
	}
}

// setGenre
// Set the genre
void MP3Tags::setGenre(string g)
{
	setTag(ID3_FRAME_GENRE, g);
}

// setGrouping
// Set the grouping
void MP3Tags::setGrouping(string g)
{
	setTag(ID3_FRAME_GROUPING, g);
}

// setTitle
// Set the title
void MP3Tags::setTitle(string t)
{
	setTag(ID3_FRAME_TITLE, t);
}

// setTitleSort
// Set the sort title
void MP3Tags::setTitleSort(string ts)
{
	setTag(ID3_FRAME_TITLE_SORT, ts);
}

// setTrack
// Set the track number
void MP3Tags::setTrack(int t)
{
	if (t > 0)
	{
		setTag(ID3_FRAME_TRACK_NUMBER, to_string(t));
	}
}

// setTrack
// Set the track number and total number of tracks
void MP3Tags::setTrack(int t, int tTotal)
{
	if ((t > 0) && (tTotal > 0) && (tTotal >= t))
	{
		string track = to_string(t);
		track.append("/");
		track.append(to_string(tTotal));
		setTag(ID3_FRAME_TRACK_NUMBER, track);
	}
}

// setYear
// Set the year
void MP3Tags::setYear(int y)
{
	if ((y > 0) && (y < 10000))
	{
		// TODO: Format with leading 0s
		setTag(ID3_FRAME_YEAR, to_string(y));
	}
}

// convertToID3FrameList
// Converts these MP3 tags to a list of ID3 tags for writing
ID3_FRAME_NODE * MP3Tags::convertToID3FrameList()
{
	ID3_FRAME_NODE * head = (ID3_FRAME_NODE *) malloc(sizeof(ID3_FRAME_NODE));
	ID3_FRAME_NODE * curr = head;

	for (pair<string, string> element : tags)
	{
		string tag = element.first;
		string value = element.second;

		// Comments use a special frame
		ID3Frame * frame;
		if (tag.compare(ID3_FRAME_COMMENT) == 0)
		{
			frame = new ID3CommentFrame(ID3_ENCODING_ISO, ID3_COMMENT_LANGUAGE_ENGLISH, value);
		}
		else
		{
			frame = new ID3Frame(tag, ID3_ENCODING_ISO, value);
		}

		curr->next = generateID3FrameNode(frame);
		curr = curr->next;
	}

	ID3_FRAME_NODE * realHead = head->next;
	free(head);

	return realHead;
}
