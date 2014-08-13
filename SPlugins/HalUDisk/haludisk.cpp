#include "haludisk.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QHash>
#include <QFileInfo>

#include <SVariableConverter>
#include <Silicon>

QList<int> speeds_list;


class HalUDiskPrivate
{
public:
    QDBusConnection *dbus_connection;

    QHash<QString,SDeviceItem> device_list;
    QHash<QString,SDeviceFeatures> features_list;
    QHash<QString,SDiscFeatures> disc_list;
    QHash<QString,QString> device_to_disc;
};



HalUDisk::HalUDisk(QObject *parent) :
    SAbstractDeviceNotifier(parent)
{
    if( speeds_list.isEmpty() )
        speeds_list << 56 << 52 << 48 << 40 << 32 << 24 << 16 << 12 << 10 << 8 << 4 << 2 ;

    p = new HalUDiskPrivate;

    QString service = "org.freedesktop.Hal";
    QString path;
    QString dev_infc = "org.freedesktop.Hal.Device";
    QString mng_infc = "org.freedesktop.Hal.Manager";

    p->dbus_connection = new QDBusConnection( QDBusConnection::systemBus() );
    p->dbus_connection->connect( service , path , dev_infc , "PropertyModified" , this , SLOT(dumpPropertyMessage(QDBusMessage)) );
    p->dbus_connection->connect( service , path , mng_infc , "DeviceAdded" , this , SLOT(dumpAddedMessage(QDBusMessage)) );
    p->dbus_connection->connect( service , path , mng_infc , "DeviceRemoved" , this , SLOT(dumpRemovedMessage(QDBusMessage)) );

    this->refresh();
}

void HalUDisk::refresh()
{
    QString service = "org.freedesktop.Hal";
    QString path = "/org/freedesktop/Hal/devices";
    QString interface = "org.freedesktop.DBus.Introspectable";
    QString method = "Introspect";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
    QDBusMessage message = p->dbus_connection->call( Introspect_msg );

    QList<QVariant> args = message.arguments();
    if( args.count() != 1 )
        return ;

    QStringList list = args.at(0).toStringList();

    if( list.isEmpty() )
        return ;

    list = list.at(0).split("\n");
    for( int i=0 ; i<list.count() ; i++ )
        if( list.at(i).contains("<node name=\"storage_",Qt::CaseInsensitive) ||
            list.at(i).contains("<node name=\"volume_",Qt::CaseInsensitive)    )
            this->detect( QString(list.at(i)).remove(" ").remove("<nodename=\"").remove("\"/>") );
}

void HalUDisk::detect( const QString & name )
{
    QString path = name;
    if( path.at(0) != '/' )
        path = "/org/freedesktop/Hal/devices/" + path;

    QVariant variant = getProperty( path , "info.capabilities" );
    if( variant.isNull() )
        return ;

    QStringList list = variant.toStringList();
    if( list.contains( "storage.cdrom" , Qt::CaseInsensitive ) )
        this->detectDevice( path );
    else if( list.contains( "volume.disc" , Qt::CaseInsensitive ) )
        this->detectDisc( path );

}

void HalUDisk::detectDevice( const QString & name )
{
    SDeviceFeatures features;
        features.hotplug_type_int = getProperty( name , "linux.hotplug_type" ).toInt();
        features.media_size_int = getProperty( name , "storage.removable.media_size" ).toULongLong();
        features.block_major_int = getProperty( name , "block.major" ).toInt();
        features.block_minor_int = getProperty( name , "block.minor" ).toInt();
        features.write_speed_int = getProperty( name , "storage.cdrom.write_speed" ).toInt();


        features.partitioning_scheme_str = getProperty( name , "storage.partitioning_scheme" ).toString();
        features.storage_device_str = getProperty( name , "block.storage_device" ).toString();
        features.device_block_str = getProperty( name , "block.device" ).toString();
        features.storage_bus_str = getProperty( name , "storage.bus" ).toString();
        features.storage_drive_type_str = getProperty( name , "storage.drive_type" ).toString();
        features.storage_drive_model_str = getProperty( name , "storage.model" ).toString();
        features.storage_vendor_str = getProperty( name , "storage.vendor" ).toString();
        features.storage_firmware_str = getProperty( name , "storage.firmware_version" ).toString();
        features.info_product_str = getProperty( name , "info.product" ).toString();
        features.info_vendor_str = getProperty( name , "info.vendor" ).toString();


        features.cdr_stt = getProperty( name , "storage.cdrom.cdr" ).toBool();
        features.cdrw_stt = getProperty( name , "storage.cdrom.cdrw" ).toBool();
        features.dvd_stt = getProperty( name , "storage.cdrom.dvd" ).toBool();
        features.dvdr_stt = getProperty( name , "storage.cdrom.dvdr" ).toBool();
        features.dvdrw_stt = getProperty( name , "storage.cdrom.dvdrw" ).toBool();
        features.dvdrdl_stt = getProperty( name , "storage.cdrom.dvdrdl" ).toBool();
        features.dvdram_stt = getProperty( name , "storage.cdrom.dvdram" ).toBool();
        features.dvdplusr_stt = getProperty( name , "storage.cdrom.dvdplusr" ).toBool();
        features.dvdplusrw_stt = getProperty( name , "storage.cdrom.dvdplusrw" ).toBool();
        features.dvdplusrwdl_stt = getProperty( name , "storage.cdrom.dvdplusrwdl" ).toBool();
        features.dvdplusrdl_stt = getProperty( name , "storage.cdrom.dvdplusrdl" ).toBool();
        features.bd_stt = getProperty( name , "storage.cdrom.bd" ).toBool();
        features.bdr_stt = getProperty( name , "storage.cdrom.bdr" ).toBool();
        features.bdre_stt = getProperty( name , "storage.cdrom.bdre" ).toBool();
        features.hddvd_stt = getProperty( name , "storage.cdrom.hddvd" ).toBool();
        features.hddvdr_stt = getProperty( name , "storage.cdrom.hddvdr" ).toBool();
        features.hddvdrw_stt = getProperty( name , "storage.cdrom.hddvdrw" ).toBool();
        features.mo_stt = getProperty( name , "storage.cdrom.mo" ).toBool();
        features.mrw_stt = getProperty( name , "storage.cdrom.mrw" ).toBool();
        features.mrw_w_stt = getProperty( name , "storage.cdrom.mrw_w" ).toBool();

        features.block_is_volume_stt = getProperty( name , "block.is_volume" ).toBool();
        features.multi_session_support_stt = getProperty( name , "storage.cdrom.support_multisession" ).toBool();
        features.media_changed_support_stt = getProperty( name , "storage.cdrom.support_media_changed" ).toBool();
        features.storage_no_partition_hint_stt = getProperty( name , "storage.no_partitions_hint" ).toBool();
        features.storage_media_check_stt = getProperty( name , "storage.media_check_enabled" ).toBool();
        features.storage_auto_mount_stt = getProperty( name , "storage.automount_enabled_hint" ).toBool();
        features.media_available_stt = getProperty( name , "storage.removable.media_available" ).toBool();
        features.removable_stt = getProperty( name , "storage.removable" ).toBool();


        features.read_speed_int = getProperty( name , "storage.cdrom.read_speed" ).toInt();
        features.speed_list.clear();

// Detect Device KB/s Speed & BluRay , DVD , CD speed =============================//

    // Device KB/s Speed //
    QStringList speed_list = getProperty( name , "storage.cdrom.write_speeds" ).toStringList();
    for( int i=0 ; i<speed_list.count() ; i++ )
        features.speed_list << speed_list.at(i).toInt();

    int max_speed;
    // BluRay Speed //


    // DVD Speed //
    max_speed = features.write_speed_int / 1385;
    for( int i=0 ; i<speeds_list.count() ; i++ )
        if( max_speed >= speeds_list.at(i) || i == speeds_list.count()-1 )
        {
            features.dvd_speed_list = speeds_list.mid( i );
            break;
        }


    // CD Speed //
    max_speed = features.write_speed_int / 150;
    for( int i=0 ; i<speeds_list.count() ; i++ )
        if( max_speed >= speeds_list.at(i) || i == speeds_list.count()-1 )
        {
            features.cd_speed_list = speeds_list.mid( i );
            break;
        }

// END ==============================//


// Detect Device Address (bus,id,lun) ===================================================//
    QString sysfs_path = getProperty( name , "linux.sysfs_path" ).toString();
    QString device_address;
    for( int i=0 ; i<sysfs_path.size() ;i++ )
        if( sysfs_path.mid(i,7) == "/target" )
            for( int j=i+7 ; j<sysfs_path.size() && sysfs_path[j] != '/' ; j++ )
                device_address.append( sysfs_path[j] );


    features.storage_address_str = device_address;
    QList<int> int_list;
    QStringList str_list = device_address.split( ":" , QString::SkipEmptyParts );
    for( int i=0 ; i<str_list.count() ; i++ )
        int_list.append( str_list.at(i).toInt() );

    if( int_list.count() != 3 )
        return ;
// END =====================================//

    SDeviceItem device;
        device.set( features.info_product_str , int_list.at(0) , int_list.at(2) , int_list.at(1) );

    if( p->features_list.contains(name) )
        p->features_list.remove(name);

    p->features_list.insert( name , features );

    p->device_list.remove( name );
    p->device_list.insert( name , device );

    emit this->deviceDetected( device );
}

void HalUDisk::detectDisc( const QString & name )
{
    SDiscFeatures features;
        features.hotplug_type_int = getProperty( name , "linux.hotplug_type" ).toInt();
        features.volume_block_size_int = getProperty( name , "volume.block_size" ).toInt();
        features.block_major_int = getProperty( name , "block.major" ).toInt();
        features.block_minor_int = getProperty( name , "block.minor" ).toInt();

        features.volume_size_int = getProperty( name , "volume.size" ).toULongLong();
        features.block_numbers_int = getProperty( name , "volume.num_blocks" ).toULongLong();
        features.volume_capacity_int = getProperty( name , "volume.disc.capacity" ).toULongLong();


        features.mount_point_str = getProperty( name , "volume.mount_point" ).toString();
        features.parent_str = getProperty( name , "info.parent" ).toString();
        features.info_product_str = getProperty( name , "info.product" ).toString();
        features.info_udi_str = getProperty( name , "info.udi" ).toString();
        features.block_storage_device_str = getProperty( name , "block.storage_device" ).toString();
        features.volume_fstype_str = getProperty( name , "volume.fstype" ).toString();
        features.volume_fsusage_str = getProperty( name , "volume.fsusage" ).toString();
        features.volume_fsversion_str = getProperty( name , "volume.fsversion" ).toString();
        features.volume_label_str = getProperty( name , "volume.label" ).toString();
        features.volume_disc_type_str = getProperty( name , "volume.disc.type" ).toString();
        features.block_device_str = getProperty( name , "block.device" ).toString();


        features.videoDVD = getProperty( name , "volume.disc.is_videodvd" ).toBool();
        features.videoBluRay = getProperty( name , "volume.disc.is_blurayvideo" ).toBool();
        features.rewritable = getProperty( name , "volume.disc.is_rewritable" ).toBool();
        features.dataDisc = getProperty( name , "volume.disc.has_data" ).toBool();
        features.vcd = getProperty( name , "volume.disc.is_vcd" ).toBool();
        features.svcd = getProperty( name , "volume.disc.is_svcd" ).toBool();
        features.audio = getProperty( name , "volume.disc.has_audio" ).toBool();

        features.volume_is_appendable = getProperty( name , "volume.disc.is_appendable" ).toBool();
        features.volume_is_mounted = getProperty( name , "volume.is_mounted" ).toBool();
        features.volume_mounted_readonly = getProperty( name , "volume.is_mounted_read_only" ).toBool();
        features.volume_device_mapper = getProperty( name , "volume.linux.is_device_mapper" ).toBool();
        features.volume_disc_is_blank = getProperty( name , "volume.disc.is_blank" ).toBool();

// Fix Disc type Str , for Example : dvd_plus_rw to DVD+RW =======================//
    features.volume_disc_type_str = features.volume_disc_type_str.toUpper();
    features.volume_disc_type_str.replace("plus","+",Qt::CaseInsensitive);
    features.volume_disc_type_str.replace("dl"," DL",Qt::CaseInsensitive);
/*
    if( !features.volume_disc_type_str.contains("+R") )
        features.volume_disc_type_str.replace("_R","-R",Qt::CaseInsensitive);
*/
    features.volume_disc_type_str.remove("_");

// END ======================//

// ADD or Replace current Disc to list =============================//
    if( p->disc_list.contains(name) )
    {
        p->disc_list.remove(name);
        p->device_to_disc.remove( features.parent_str );
    }

    p->disc_list.insert( name , features );
    p->device_to_disc.insert( features.parent_str , name );
// END =====================//

    if( p->device_list.contains( features.parent_str ) )
        emit this->deviceDetected( p->device_list[features.parent_str] );

    emit this->discDetected( features );
}

QVariant HalUDisk::getProperty( const QString & device , const QString & key ) const
{
    if( device.isEmpty() )
        return QVariant();

    QString service = "org.freedesktop.Hal";
    QString path = device;
    QString interface = "org.freedesktop.Hal.Device";
    QString method = "GetPropertyString";

    if( path.at(0) != '/' )
        path = "/org/freedesktop/Hal/devices/" + path;

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
        Introspect_msg.setArguments( QList<QVariant>()<< key );

    QDBusMessage message = p->dbus_connection->call( Introspect_msg );
    QList<QVariant> args = message.arguments();
    if( args.count() != 1 )
        return QVariant();

    return args.at(0);
}

bool HalUDisk::mount( const SDeviceItem & device , const QString & mount_point )
{
    QString service = "org.freedesktop.UDisks";
    QString path = "/org/freedesktop/UDisks/devices/" + QFileInfo(device.address()).fileName();
    QString interface = "org.freedesktop.UDisks.Device";
    QString method = "FilesystemMount";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
        Introspect_msg.setArguments( QList<QVariant>()<<device.currentDiscFeatures().volume_fstype_str <<QStringList() );

    p->dbus_connection->call( Introspect_msg , QDBus::Block , 0 );

    return true;
}

bool HalUDisk::unmount( const SDeviceItem & device )
{
    QString service = "org.freedesktop.UDisks";
    QString path = "/org/freedesktop/UDisks/devices/" + QFileInfo(device.address()).fileName();
    QString interface = "org.freedesktop.UDisks.Device";
    QString method = "FilesystemUnmount";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
        Introspect_msg.setArguments( QList<QVariant>()<<QStringList() );

    p->dbus_connection->call( Introspect_msg , QDBus::Block , 0 );

    return true;
}

void HalUDisk::eject( const SDeviceItem & device )
{
    QString service = "org.freedesktop.UDisks";
    QString path = "/org/freedesktop/UDisks/devices/" + QFileInfo(device.address()).fileName();
    QString interface = "org.freedesktop.UDisks.Device";
    QString method = "DriveEject";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
        Introspect_msg.setArguments( QList<QVariant>()<<QStringList() );

    p->dbus_connection->call( Introspect_msg , QDBus::Block , 0 );
}

const SDeviceFeatures & HalUDisk::deviceFeatures( const SDeviceItem & device )
{
    QString name = p->device_list.key( device );
    return p->features_list[name];
}

const SDiscFeatures & HalUDisk::discFeatures( const SDeviceItem & device )
{
    QString name = p->device_to_disc.value( p->device_list.key(device) );
    return p->disc_list[name];
}

QList<SDeviceItem> HalUDisk::deviceList() const
{
    return p->device_list.values();
}

void HalUDisk::dumpPropertyMessage( const QDBusMessage & dmsg )
{
    if( dmsg.type() == QDBusMessage::SignalMessage )
        this->detect( dmsg.path() );
}

void HalUDisk::dumpAddedMessage( const QDBusMessage & dmsg )
{
    if( dmsg.type() != QDBusMessage::SignalMessage )
        return ;

    QList<QVariant> list = dmsg.arguments();
    if( list.isEmpty() )
        return ;

    QString path = list.at(0).toString();

    this->detect( path );
}

void HalUDisk::dumpRemovedMessage( const QDBusMessage & dmsg )
{
    if( dmsg.type() != QDBusMessage::SignalMessage )
        return ;

    QList<QVariant> list = dmsg.arguments();
    if( list.isEmpty() )
        return ;

    QString path = list.at(0).toString();

    if( p->device_list.contains(path) )
        emit this->deviceRemoved( p->device_list.take(path) );

    if( p->features_list.contains(path) )
        p->features_list.remove( path );

    if( p->disc_list.contains(path) )
    {
        SDiscFeatures disc = p->disc_list.take(path);
        p->device_to_disc.remove( disc.parent_str );

        if( p->device_list.contains( disc.parent_str ) )
            emit this->deviceDetected( p->device_list[disc.parent_str] );

        emit this->discRemoved( disc );
    }
}

HalUDisk::~HalUDisk()
{
    delete p->dbus_connection;
    delete p;
}
