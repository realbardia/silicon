#include "sdeviceitem.h"
#include "sdevicelist.h"

SDeviceItem::SDeviceItem()
{
    set( NULL , -1 , -1 , -1 );
}

SDeviceItem::SDeviceItem( const QString & name , int bus , int id , int lun )
{
    set( name , bus , id , lun );
}

void SDeviceItem::setString( const QString & str )
{
    bool ok;
    QStringList list = str.split(',',QString::SkipEmptyParts);

    if( list.count() != 3 )
        return ;

    set( device_name , list.at(0).toInt( &ok ) , list.at(1).toInt( &ok ) , list.at(2).toInt( &ok ) );
}

void SDeviceItem::set(const QString & name , int bus , int id , int lun , const QString &path)
{
    setName( name );
    setBus( bus );
    setId( id );
    setLun( lun );
    SDeviceItem::path = path;
}

void SDeviceItem::setName( const QString & name )
{
    device_name = name;
}

void SDeviceItem::setBus( int bus )
{
    scsi_bus = bus;
}

void SDeviceItem::setId( int id )
{
    scsi_id = id;
}

void SDeviceItem::setLun( int lun )
{
    scsi_lun = lun;
}

void SDeviceItem::eject() const
{
    SDeviceList::eject( *this );
}

void SDeviceItem::mount() const
{
    SDeviceList::mount( *this );
}

void SDeviceItem::unmount() const
{
    SDeviceList::unmount( *this );
}

const QList<int> & SDeviceItem::speeds() const
{
    return deviceFeatures().speed_list;
}

const SDeviceFeatures & SDeviceItem::deviceFeatures() const
{
    return SDeviceList::deviceFeatures( *this );
}

const SDiscFeatures & SDeviceItem::currentDiscFeatures() const
{
    return SDeviceList::discFeatures( *this );
}

QString SDeviceItem::toQString() const
{
    if( !path.isEmpty() )
        return path;

    QString result;

    result = result + QString::number(scsi_bus) + "," ;
    result = result + QString::number(scsi_id) + "," ;
    result = result + QString::number(scsi_lun) ;

    return result;
}

const QString & SDeviceItem::name() const
{
    return device_name;
}

const QString & SDeviceItem::address() const
{
    return deviceFeatures().device_block_str;
}

int SDeviceItem::bus() const
{
    return scsi_bus;
}

int SDeviceItem::id() const
{
    return scsi_id;
}

int SDeviceItem::lun() const
{
    return scsi_lun;
}

bool SDeviceItem::isEmpty() const
{
    return ( device_name.isEmpty() && scsi_bus == -1 && scsi_id == -1 && scsi_lun == -1 );
}

SDeviceItem SDeviceItem::operator=( SDeviceItem item )
{
    set( item.name() , item.bus() , item.id() , item.lun(), item.path );
    return item;
}

bool SDeviceItem::operator==( const SDeviceItem & item ) const
{
    return ( bus() == item.bus() && id() == item.id() && lun() == item.lun() );
}

bool SDeviceItem::operator!=( const SDeviceItem & item ) const
{
    return !(*this == item);
}

SDeviceItem::~SDeviceItem()
{
}
