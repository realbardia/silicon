#ifndef SFILEINFO_H
#define SFILEINFO_H

#include <QString>
#include <QDate>

#include "smusicinfo.h"
#include "spictureinfo.h"
#include "svideoinfo.h"

#include "sdatabase_global.h"

class SDATABASESHARED_EXPORT SFileInfo
{
public:
    enum FileCat{
        Normal,
        Video,
        Music,
        Photo,
        Document
    };

    SFileInfo();
    SFileInfo( const QString & disc_id , const QString & name , const QString & parent );

    void setID( int id );
    void setName( const QString & name );
    void setType( const QString & type );
    void setPermission( const QString & permission );
    void setOwner( const QString & owner );
    void setHash( const QString & hash );
    void setDiscId( const QString & disc_id );
    void setParent( const QString & parent );
    void setModifiedDate( const QDate & date );
    void setAccessesDate( const QDate & date );
    void setSize( int size );
    void setRate( int rate );
    void setCategory( FileCat cat );
    void setMusicInfo( const SMusicInfo & music );
    void setVideoInfo( const SVideoInfo & video );
    void setPictureInfo( const SPictureInfo & picture );

    int id() const;
    const QString & name() const;
    const QString & type() const;
    const QString & permission() const;
    const QString & owner() const;
    const QString & hash() const;
    const QString & discId() const;
    const QString & parent() const;
    const QDate & modifiedDate() const;
    const QDate & accessedDate() const;
    int size() const;
    int rate() const;
    FileCat category() const;
    const SMusicInfo & musicInfo() const;
    const SVideoInfo & videoInfo() const;
    const SPictureInfo & pictureInfo() const;

private:
    int id_int;
    QString name_str;
    QString type_str;
    QString permission_str;
    QString owner_str;
    QString hash_str;
    QString disc_id_str;
    QString parent_str;
    QDate   modified_date;
    QDate   accessed_date;
    int     size_int;
    int     rate_int;
    FileCat cat;

    SMusicInfo   music_info;
    SVideoInfo   video_info;
    SPictureInfo picture_info;
};

#endif // SFILEINFO_H
