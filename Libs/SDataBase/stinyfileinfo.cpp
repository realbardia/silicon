#include "stinyfileinfo.h"

STinyFileInfo::STinyFileInfo()
{
    is_dir = false;
    id_int = -1;
}

void STinyFileInfo::setID( int id )
{
    id_int = id;
}

void STinyFileInfo::setName( const QString & name )
{
    name_str = name;
}

void STinyFileInfo::setAddress( const QString & address )
{
    address_str = address;
}

void STinyFileInfo::setDiscID( const QString & disc )
{
    disc_id_str = disc;
}

void STinyFileInfo::setType( const QString & type )
{
    type_str = type;
}

void STinyFileInfo::setDirectory( bool stt )
{
    is_dir = stt;
}

int STinyFileInfo::ID() const
{
    return id_int;
}

const QString & STinyFileInfo::name() const
{
    return name_str;
}

const QString & STinyFileInfo::address() const
{
    return address_str;
}

const QString & STinyFileInfo::discID() const
{
    return disc_id_str;
}

const QString & STinyFileInfo::type() const
{
    return type_str;
}

bool STinyFileInfo::isDirectory() const
{
    return is_dir;
}
