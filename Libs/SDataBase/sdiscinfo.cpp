#include "sdiscinfo.h"

SDiscInfo::SDiscInfo( const QString & id )
{
    this->setDiscId( id );

    disc_type         = SDiscInfo::Unknown;
    appendable_bool   = false;
    size_int          = 0;
    free_capacity_int = 0;
}

void SDiscInfo::setDiscId( const QString & disc )
{
    disc_id_str = disc;
}

void SDiscInfo::setMd5Sum( const QString & md5 )
{
    md5_sum_str = md5;
}

void SDiscInfo::setLabel( const QString & label )
{
    label_str = label;
}

void SDiscInfo::setAddress( const QString & address )
{
    address_str = address;
}

void SDiscInfo::setPublisher( const QString & publisher )
{
    publisher_str = publisher;
}

void SDiscInfo::setApplicationId( const QString & app_id )
{
    app_id_str = app_id;
}

void SDiscInfo::setFormat(  const QString & format )
{
    format_str = format;
}

void SDiscInfo::setCopyRight(  const QString & copyright )
{
    copy_right_str = copyright;
}

void SDiscInfo::setSysId( const QString & sys_id )
{
    sys_id_str = sys_id;
}

void SDiscInfo::setSize( int size )
{
    size_int = size;
}

void SDiscInfo::setFreeCapacity( int free_capacity )
{
    free_capacity_int = free_capacity;
}

void SDiscInfo::setAppendable( bool appendable )
{
    appendable_bool = appendable;
}

void SDiscInfo::setType( SDiscInfo::DiscType type )
{
    disc_type = type;
}

void SDiscInfo::setTypeString( const QString & type )
{
    if( type == "Audio" )
        disc_type = SDiscInfo::Audio;

    else if( type == "BluRay" )
        disc_type = SDiscInfo::BluRay_VIDEO;

    else if( type == "Data" )
        disc_type = SDiscInfo::Data;

    else if( type == "DVD-Video" )
        disc_type = SDiscInfo::DVD_VIDEO;

    else if( type == "SVCD" )
        disc_type = SDiscInfo::SVCD;

    else if( type == "VCD" )
        disc_type = SDiscInfo::VCD;

    else
        disc_type == SDiscInfo::Unknown;
}

const QString & SDiscInfo::discId() const
{
    return disc_id_str;
}

const QString & SDiscInfo::md5Sum() const
{
    return md5_sum_str;
}

const QString & SDiscInfo::label() const
{
    return label_str;
}

const QString & SDiscInfo::address() const
{
    return address_str;
}

const QString & SDiscInfo::publisher() const
{
    return publisher_str;
}

const QString & SDiscInfo::applicationId() const
{
    return app_id_str;
}

const QString & SDiscInfo::format() const
{
    return format_str;
}

const QString & SDiscInfo::copyRight() const
{
    return copy_right_str;
}

const QString & SDiscInfo::sysId() const
{
    return sys_id_str;
}

int SDiscInfo::size() const
{
    return size_int;
}

int SDiscInfo::freeCapacity() const
{
    return free_capacity_int;
}

bool SDiscInfo::appendable() const
{
    return appendable_bool;
}

SDiscInfo::DiscType SDiscInfo::type() const
{
    return disc_type;
}

QString SDiscInfo::typeString() const
{
    QString result;
    switch( static_cast<int>(disc_type) )
    {
    case SDiscInfo::Audio :
        result = "Audio";
        break;

    case SDiscInfo::BluRay_VIDEO :
        result = "BluRay";
        break;

    case SDiscInfo::Data :
        result = "Data";
        break;

    case SDiscInfo::DVD_VIDEO :
        result = "DVD-Video";
        break;

    case SDiscInfo::SVCD :
        result = "SVCD";
        break;

    case SDiscInfo::VCD :
        result = "VCD";
        break;
    }

    return result;
}
