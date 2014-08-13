#include "sfileinfo.h"

SFileInfo::SFileInfo()
    : music_info( QString() , QString() )
{
    this->setCategory( SFileInfo::Normal );
}

SFileInfo::SFileInfo( const QString & disc_id , const QString & name , const QString & parent )
    : music_info( name , disc_id )
{
    this->setDiscId( disc_id );
    this->setName( name );
    this->setParent( parent );
    this->setCategory( SFileInfo::Normal );
}

void SFileInfo::setID( int id )
{
    id_int = id;
}

void SFileInfo::setName( const QString & name )
{
    name_str = name;
}

void SFileInfo::setType( const QString & type )
{
    type_str = type;
}

void SFileInfo::setPermission( const QString & permission )
{
    permission_str = permission;
}

void SFileInfo::setOwner( const QString & owner )
{
    owner_str = owner;
}

void SFileInfo::setHash( const QString & hash )
{
    hash_str = hash;
}

void SFileInfo::setDiscId( const QString & disc_id )
{
    disc_id_str = disc_id;
}

void SFileInfo::setParent( const QString & parent )
{
    parent_str = parent;
}

void SFileInfo::setModifiedDate( const QDate & date )
{
    modified_date = date;
}

void SFileInfo::setAccessesDate( const QDate & date )
{
    accessed_date = date;
}

void SFileInfo::setSize( int size )
{
    size_int = size;
}

void SFileInfo::setRate( int rate )
{
    rate_int = rate;
}

void SFileInfo::setCategory( SFileInfo::FileCat cat )
{
    this->cat = cat;
}

void SFileInfo::setMusicInfo( const SMusicInfo & music )
{
    setCategory( SFileInfo::Music );
    music_info = music;
}

void SFileInfo::setVideoInfo( const SVideoInfo & video )
{
    setCategory( SFileInfo::Video );
    video_info = video;
}

void SFileInfo::setPictureInfo( const SPictureInfo & picture )
{
    setCategory( SFileInfo::Photo );
    picture_info = picture;
}

int SFileInfo::id() const
{
    return id_int;
}

const QString & SFileInfo::name() const
{
    return name_str;
}

const QString & SFileInfo::type() const
{
    return type_str;
}

const QString & SFileInfo::permission() const
{
    return permission_str;
}

const QString & SFileInfo::owner() const
{
    return owner_str;
}

const QString & SFileInfo::hash() const
{
    return hash_str;
}

const QString & SFileInfo::discId() const
{
    return disc_id_str;
}

const QString & SFileInfo::parent() const
{
    return parent_str;
}

const QDate & SFileInfo::modifiedDate() const
{
    return modified_date;
}

const QDate & SFileInfo::accessedDate() const
{
    return accessed_date;
}

int SFileInfo::size() const
{
    return size_int;
}

int SFileInfo::rate() const
{
    return rate_int;
}

SFileInfo::FileCat SFileInfo::category() const
{
    return cat;
}

const SMusicInfo & SFileInfo::musicInfo() const
{
    return music_info;
}

const SVideoInfo & SFileInfo::videoInfo() const
{
    return video_info;
}

const SPictureInfo & SFileInfo::pictureInfo() const
{
    return picture_info;
}
