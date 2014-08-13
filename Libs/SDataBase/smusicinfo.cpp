#include "smusicinfo.h"

SMusicInfo::SMusicInfo( const QString & address , const QString & disc_id  )
{
    this->setAddress( address );
    this->setDiscId( disc_id );
}

void SMusicInfo::setArtist( const QString & artist )
{
    artist_str = artist;
}

void SMusicInfo::setAlbum( const QString & album )
{
    album_str = album;
}

void SMusicInfo::setTitle( const QString & title )
{
    title_str = title;
}

void SMusicInfo::setGenre( const QString & genre )
{
    genre_str = genre;
}

void SMusicInfo::setLyric( const QString & lyric )
{
    lyric_str = lyric;
}

void SMusicInfo::setMood( const QString & mood )
{
    mood_str = mood;
}

void SMusicInfo::setAddress( const QString & address )
{
    address_str = address;
}

void SMusicInfo::setDiscId( const QString & disc_id )
{
    disc_id_str = disc_id;
}

void SMusicInfo::setDuration( const QTime & duration )
{
    duration_time = duration;
}

void SMusicInfo::setCover( const QImage & cover )
{
    cover_pixmap = cover;
}

void SMusicInfo::setYear( int year )
{
    year_int = year;
}

void SMusicInfo::setSize( int size )
{
    size_int = size;
}

void SMusicInfo::setTrackNumber( int track_number )
{
    track_number_int = track_number;
}

const QString & SMusicInfo::artist() const
{
    return artist_str;
}

const QString & SMusicInfo::album() const
{
    return album_str;
}

const QString & SMusicInfo::title() const
{
    return title_str;
}

const QString & SMusicInfo::genre() const
{
    return genre_str;
}

const QString & SMusicInfo::lyric() const
{
    return lyric_str;
}

const QString & SMusicInfo::mood() const
{
    return mood_str;
}

const QString & SMusicInfo::address() const
{
    return address_str;
}

const QString & SMusicInfo::discId() const
{
    return disc_id_str;
}

const QTime & SMusicInfo::duration() const
{
    return duration_time;
}

const QImage & SMusicInfo::cover() const
{
    return cover_pixmap;
}

int SMusicInfo::year() const
{
    return year_int;
}

int SMusicInfo::size() const
{
    return size_int;
}

int SMusicInfo::trackNumber() const
{
    return track_number_int;
}
