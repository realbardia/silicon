#ifndef SMUSICINFO_H
#define SMUSICINFO_H

#include <QString>
#include <QTime>
#include <QImage>

#include "sdatabase_global.h"

class SDATABASESHARED_EXPORT SMusicInfo
{
public:
    SMusicInfo( const QString & address , const QString & disc_id );

    void setArtist( const QString & artist );
    void setAlbum( const QString & album );
    void setTitle( const QString & title );
    void setGenre( const QString & genre );
    void setLyric( const QString & lyric );
    void setMood( const QString & mood );
    void setAddress( const QString & address );
    void setDiscId( const QString & disc_id );
    void setDuration( const QTime & duration );
    void setCover( const QImage & cover );
    void setYear( int year );
    void setSize( int size );
    void setTrackNumber( int track_number );

    const QString & artist() const;
    const QString & album() const;
    const QString & title() const;
    const QString & genre() const;
    const QString & lyric() const;
    const QString & mood() const;
    const QString & address() const;
    const QString & discId() const;
    const QTime & duration() const;
    const QImage & cover() const;
    int year() const;
    int size() const;
    int trackNumber() const;

private:
    QString artist_str;
    QString album_str;
    QString title_str;
    QString genre_str;
    QString lyric_str;
    QString mood_str;
    QString address_str;
    QString disc_id_str;
    QTime   duration_time;
    QImage  cover_pixmap;
    int     year_int;
    int     size_int;
    int     track_number_int;
};

#endif // SMUSICINFO_H
