#include "sudisks.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusArgument>
#include <QDBusObjectPath>

#include <QFileInfo>
#include <QHash>

#include <SVariableConverter>
#include <Silicon>

#include "speeddetector.h"

QList<int> speeds_list;
QHash<QString,qlonglong> capacities;


class SUDisksDeviceNotifierPrivate
{
public:
    QDBusConnection *dbus_connection;

    QHash<QString,SDeviceItem> device_list;
    QHash<QString,SDeviceFeatures> features_list;
    QHash<QString,SDiscFeatures> disc_list;

    QHash<SpeedDetector *,SDeviceItem> cdrecord_hashs;
};



SUDisksDeviceNotifier::SUDisksDeviceNotifier(QObject *parent) :
    SAbstractDeviceNotifier(parent)
{
    if( speeds_list.isEmpty() )
        speeds_list << 56 << 52 << 48 << 40 << 32 << 24 << 16 << 12 << 10 << 8 << 4 << 2 ;
    if( capacities.isEmpty() )
    {
        capacities.insert( "optical_cd_r"           , 735051776   );
        capacities.insert( "optical_cd_rw"          , 735051776   );
        capacities.insert( "optical_dvd_plus_r"     , 4700372992  );
        capacities.insert( "optical_dvd_plus_r_dl"  , 8547991552  );
        capacities.insert( "optical_dvd_plus_rw"    , 4700372992  );
        capacities.insert( "optical_dvd_plus_rw_dl" , 8547991552  );
        capacities.insert( "optical_dvd_r"          , 4707319808  );
        capacities.insert( "optical_dvd_r_dl"       , 8543666176  );
        capacities.insert( "optical_dvd_rw"         , 4707319808  );
        capacities.insert( "optical_dvd_rw_dl"      , 8543666176  );
        capacities.insert( "optical_bd_r"           , 25025314816 );
        capacities.insert( "optical_bd_r_dl"        , 50050629632 );
        capacities.insert( "optical_bd_re"          , 25025314816 );
        capacities.insert( "optical_bd_re_dl"       , 50050629632 );
        capacities.insert( "optical_hddvd_r"        , 15076554752 );
        capacities.insert( "optical_hddvd_r_dl"     , 33393473536 );
        capacities.insert( "optical_hddvd_rw"       , 15076554752 );
        capacities.insert( "optical_hddvd_rw_dl"    , 33393473536 );
        capacities.insert( "optical_mrw"            , 4707319808 );
        capacities.insert( "optical_mrw_w"          , 4707319808 );
    }

    p = new SUDisksDeviceNotifierPrivate;

    QString service = "org.freedesktop.UDisks";
    QString path;
    //QString dev_infc = "org.freedesktop.UDisks.Device";
    QString mng_infc = "org.freedesktop.UDisks";

    p->dbus_connection = new QDBusConnection( QDBusConnection::systemBus() );
    p->dbus_connection->connect( service , path , mng_infc , "DeviceChanged" , this , SLOT(dumpChengedMessage(QDBusMessage)) );
    p->dbus_connection->connect( service , path , mng_infc , "DeviceAdded"   , this , SLOT(dumpAddedMessage(QDBusMessage))   );
    p->dbus_connection->connect( service , path , mng_infc , "DeviceRemoved" , this , SLOT(dumpRemovedMessage(QDBusMessage)) );

    this->refresh();
}

void SUDisksDeviceNotifier::refresh()
{
    QString service = "org.freedesktop.UDisks";
    QString path = "/org/freedesktop/UDisks";
    QString interface = "org.freedesktop.UDisks";
    QString method = "EnumerateDeviceFiles";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
    QDBusMessage message = p->dbus_connection->call( Introspect_msg );

    QList<QVariant> args = message.arguments();
    if( args.count() != 1 )
        return ;

    QStringList list = args.at(0).toStringList();

    if( list.isEmpty() )
        return ;

    for( int i=0 ; i<list.count() ; i++ )
        if( list.at(i).count("/") == 2 )
            this->detect( QString(list.at(i)).remove(0,5) );
}

void SUDisksDeviceNotifier::detect( const QString & name )
{
    QString path = name;
    if( path.at(0) != '/' )
        path = "/org/freedesktop/UDisks/devices/" + path;

    QVariant variant = getProperty( path , "DriveIsMediaEjectable" );
    if( variant.type() == QVariant::Bool && !variant.toBool() )
        return ;

    detectDevice( path );
    detectDisc( path );
}

void SUDisksDeviceNotifier::detectDevice( const QString & name )
{
    SDeviceFeatures features;
        features.media_size_int = getProperty( name , "DeviceSize" ).toULongLong();
        features.block_major_int = getProperty( name , "DeviceMajor" ).toInt();
        features.block_minor_int = getProperty( name , "DeviceMinor" ).toInt();


        features.partitioning_scheme_str = getProperty( name , "PartitionScheme" ).toString();
        features.storage_device_str = name;
        features.device_block_str = getProperty( name , "DeviceFile" ).toString();
        features.storage_drive_type_str = getProperty( name , "IdType" ).toString();
        features.storage_drive_model_str = getProperty( name , "DriveModel" ).toString();
        features.storage_vendor_str = getProperty( name , "DriveVendor" ).toString();
        features.info_product_str = features.storage_drive_model_str;
        features.info_vendor_str = features.storage_vendor_str;

    QStringList compatibility = getProperty( name , "DriveMediaCompatibility" ).toStringList();
        features.cdr_stt = compatibility.contains( "optical_cd_r" );
        features.cdrw_stt = compatibility.contains( "optical_cd_rw" );
        features.dvd_stt =  compatibility.contains( "optical_dvd" );
        features.dvdr_stt =  compatibility.contains( "optical_dvd_r" );
        features.dvdrw_stt =  compatibility.contains( "optical_dvd_rw" );
        features.dvdrdl_stt =  compatibility.contains( "optical_dvd_plus_r_dl" );
        features.dvdram_stt =  compatibility.contains( "optical_dvd_ram" );
        features.dvdplusr_stt =  compatibility.contains( "optical_dvd_plus_r" );
        features.dvdplusrw_stt =  compatibility.contains( "optical_dvd_plus_rw" );
        features.dvdplusrwdl_stt =  compatibility.contains( "optical_dvd_plus_rw_dl" );
        features.dvdplusrdl_stt =  compatibility.contains( "optical_dvd_plus_r_dl" );
        features.bd_stt =  compatibility.contains( "optical_bd" );
        features.bdr_stt =  compatibility.contains( "optical_bd_r" );
        features.bdre_stt =  compatibility.contains( "optical_bd_re" );
        features.hddvd_stt =  compatibility.contains( "optical_hddvd" );
        features.hddvdr_stt =  compatibility.contains( "optical_hddvd_r" );
        features.hddvdrw_stt =  compatibility.contains( "optical_hddvd_rw" );
        features.mo_stt =  compatibility.contains( "optical_mo" );
        features.mrw_stt =  compatibility.contains( "optical_mrw" );
        features.mrw_w_stt =  compatibility.contains( "optical_mrw_w" );

        features.multi_session_support_stt = getProperty( name , "OpticalDiscIsAppendable" ).toBool();
        features.media_changed_support_stt = getProperty( name , "DriveIsMediaEjectable" ).toBool();
        features.media_available_stt = getProperty( name , "DeviceIsMediaAvailable" ).toBool();
        features.removable_stt = getProperty( name , "DeviceIsRemovable" ).toBool();

        features.speed_list.clear();


// Detect Device Address (bus,id,lun) ===================================================//
    QString sysfs_path = getProperty( name , "NativePath" ).toString();
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
        device.set( features.info_product_str , int_list.at(0) , int_list.at(2) , int_list.at(1), features.device_block_str );

    if( p->features_list.contains(name) )
        p->features_list.remove(name);

    p->features_list.insert( name , features );


// Detect Device KB/s Speed & BluRay , DVD , CD speed =============================//

    SpeedDetector *cdrecord = new SpeedDetector( this );
    QObject::connect( cdrecord , SIGNAL(prcapFinished(SpeedDetector*,QString)) , SLOT(speedDetected(SpeedDetector*,QString)) , Qt::QueuedConnection );

    p->cdrecord_hashs.insert( cdrecord , device );
    cdrecord->prcap( device );

// END ==============================//

    p->device_list.remove( name );
    p->device_list.insert( name , device );

    //emit this->deviceDetected( device );
}

void SUDisksDeviceNotifier::detectDisc( const QString & name )
{
    SDiscFeatures features;

        features.volume_is_appendable = getProperty( name , "OpticalDiscIsAppendable" ).toBool();
        features.volume_is_mounted = getProperty( name , "DeviceIsMounted" ).toBool();
        features.volume_mounted_readonly = getProperty( name , "DeviceIsReadOnly" ).toBool();
        features.volume_disc_is_blank = getProperty( name , "OpticalDiscIsBlank" ).toBool();


        features.parent_str = name;
        features.mount_point_str = getProperty( name , "DeviceMountPaths" ).toStringList().join(", ");
        features.info_product_str = getProperty( name , "IdLabel" ).toString();
        features.block_storage_device_str = name;
        features.volume_fstype_str = getProperty( name , "IdType" ).toString();
        features.volume_fsusage_str = getProperty( name , "IdUsage" ).toString();
        features.volume_fsversion_str = getProperty( name , "IdVersion" ).toString();
        features.volume_label_str = getProperty( name , "IdLabel" ).toString();
        features.volume_disc_type_str = getProperty( name , "DriveMedia" ).toString();
        features.block_device_str = name;

        features.volume_block_size_int = getProperty( name , "DeviceBlockSize" ).toInt();
        features.block_major_int = getProperty( name , "DeviceMajor" ).toInt();
        features.block_minor_int = getProperty( name , "DeviceMinor" ).toInt();

        features.volume_size_int = getProperty( name , "DeviceSize" ).toULongLong();
        features.block_numbers_int = features.volume_size_int / ( features.volume_block_size_int + (features.volume_block_size_int==0) ) ;
        features.volume_capacity_int = (features.volume_disc_is_blank)?capacities.value(features.volume_disc_type_str):0;

/*
        features.videoDVD = getProperty( name , "volume.disc.is_videodvd" ).toBool();
        features.videoBluRay = getProperty( name , "volume.disc.is_blurayvideo" ).toBool();
        features.rewritable = getProperty( name , "volume.disc.is_rewritable" ).toBool();
        features.vcd = getProperty( name , "volume.disc.is_vcd" ).toBool();
        features.svcd = getProperty( name , "volume.disc.is_svcd" ).toBool();*/
        features.audio = getProperty( name , "OpticalDiscNumAudioTracks" ).toInt() != 0;
        features.dataDisc = !features.audio;

// Fix Disc type Str , for Example : dvd_plus_rw to DVD+RW =======================//
    features.volume_disc_type_str = features.volume_disc_type_str.toUpper();
        features.volume_disc_type_str.remove( "OPTICAL",Qt::CaseInsensitive);
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
        p->disc_list.remove(name);

    p->disc_list.insert( name , features );
// END =====================//

    if( p->device_list.contains( features.parent_str ) )
        emit this->deviceDetected( p->device_list[features.parent_str] );

    emit this->discDetected( features );
}

void SUDisksDeviceNotifier::speedDetected( SpeedDetector *cdrecord , const QString & str )
{
    SDeviceItem     device   = p->cdrecord_hashs.value( cdrecord );
    SDeviceFeatures features = device.deviceFeatures();
    QString         name     = features.storage_device_str;

    QList<int> & CD  = features.cd_speed_list;
    QList<int> & DVD = features.dvd_speed_list;
    QList<int> & BD  = features.bluray_speed_list;
    QList<int> & SPD = features.speed_list;


    QStringList list = str.split( "\n" , QString::SkipEmptyParts );
    for( int i=0 ; i<list.count() ; i++ )
    {
        QRegExp reg;
        int pos;

        reg.setPattern( "\\s+Number of supported write speeds\\:\\s+(\\d+)" );
        pos = reg.indexIn( list.at(i) );
        if( pos > -1 )
        {
            list = list.mid(i+1 , reg.cap(1).toInt());
            break;
        }
    }



    QRegExp reg;
    reg.setPattern( "\\s*Write\\sspeed\\s\\#\\s\\d+\\:\\s*(\\d+)\\s+kB\\/s\\s+CLV\\/PCAV\\s+\\(CD\\s*(\\d+)x\\,\\s*DVD\\s*(\\d+)x(\\,\\s*BD\\s*)?(\\d+)?(\\x)?\\)\\s*" );

    for( int i=0 ; i<list.count() ; i++ )
    {
        int pos;

        pos = reg.indexIn( list.at(i) );
        if( pos > -1 )
        {
            SPD << reg.cap(1).toInt();
            CD  << reg.cap(2).toInt();
            DVD << reg.cap(3).toInt();
            BD  << reg.cap(5).toInt();
        }
    }

    if( p->features_list.contains(name) )
        p->features_list.remove(name);

    p->features_list.insert( name , features );


    emit this->deviceDetected( device );

    p->cdrecord_hashs.remove( cdrecord );
    delete cdrecord;
}

QVariant SUDisksDeviceNotifier::getProperty( const QString & device , const QString & key ) const
{
    if( device.isEmpty() )
        return QVariant();

    QString service = "org.freedesktop.UDisks";
    QString path = device;
    QString interface = "org.freedesktop.UDisks.Device";
    QString name = key;

    if( path.at(0) != '/' )
        path = "/org/freedesktop/UDisks/devices/" + path;

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("Get") );
        Introspect_msg.setArguments( QList<QVariant>()<< interface << name );

    QDBusMessage message = p->dbus_connection->call( Introspect_msg );

    QList<QVariant> args = message.arguments();
    if( args.count() != 1 )
        return QVariant();

    QVariant result = args.at(0);
    if( result.userType() == qMetaTypeId<QDBusVariant>() )
        result = qvariant_cast<QDBusVariant>(result).variant();

    return result;
}

QList<SDeviceItem> SUDisksDeviceNotifier::deviceList() const
{
    return p->device_list.values();
}

bool SUDisksDeviceNotifier::mount( const SDeviceItem & device , const QString & )
{
    QString service = "org.freedesktop.UDisks";
    QString path = device.deviceFeatures().storage_device_str;
    QString interface = "org.freedesktop.UDisks.Device";
    QString method = "FilesystemMount";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
        Introspect_msg.setArguments( QList<QVariant>()<<device.currentDiscFeatures().volume_fstype_str <<QStringList() );

    p->dbus_connection->call( Introspect_msg , QDBus::Block , 0 );

    return true;
}

bool SUDisksDeviceNotifier::unmount( const SDeviceItem & device )
{
    QString service = "org.freedesktop.UDisks";
    QString path = device.deviceFeatures().storage_device_str;
    QString interface = "org.freedesktop.UDisks.Device";
    QString method = "FilesystemUnmount";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
        Introspect_msg.setArguments( QList<QVariant>()<<QStringList() );

    p->dbus_connection->call( Introspect_msg , QDBus::Block , 0 );

    return true;
}

void SUDisksDeviceNotifier::eject( const SDeviceItem & device )
{
    QString service = "org.freedesktop.UDisks";
    QString path = device.deviceFeatures().storage_device_str;
    QString interface = "org.freedesktop.UDisks.Device";
    QString method = "DriveEject";

    QDBusMessage Introspect_msg = QDBusMessage::createMethodCall( service , path , interface , method );
        Introspect_msg.setArguments( QList<QVariant>()<<QStringList() );

    p->dbus_connection->call( Introspect_msg , QDBus::Block , 0 );
}

const SDeviceFeatures & SUDisksDeviceNotifier::deviceFeatures( const SDeviceItem & device )
{
    QString name = p->device_list.key( device );
    return p->features_list[name];
}

const SDiscFeatures   & SUDisksDeviceNotifier::discFeatures( const SDeviceItem & device )
{
    QString name = p->device_list.key( device );
    return p->disc_list[name];
}

void SUDisksDeviceNotifier::dumpChengedMessage( const QDBusMessage & dmsg )
{
    if( dmsg.type() != QDBusMessage::SignalMessage )
        return ;

    const QList<QVariant> & list = dmsg.arguments();
    if( list.isEmpty() )
        return ;

    QString path = qvariant_cast<QDBusObjectPath>(list.first()).path();
        path = QFileInfo(path).fileName();

    detect( path );
}

void SUDisksDeviceNotifier::dumpAddedMessage( const QDBusMessage & dmsg )
{
    dumpChengedMessage( dmsg );
}

void SUDisksDeviceNotifier::dumpRemovedMessage( const QDBusMessage & dmsg )
{
    if( dmsg.type() != QDBusMessage::SignalMessage )
        return ;

    const QList<QVariant> & list = dmsg.arguments();
    if( list.isEmpty() )
        return ;

    QString path = qvariant_cast<QDBusObjectPath>(list.first()).path();
        path = QFileInfo(path).fileName();
        path = "/dev/" + path;

    if( p->device_list.contains(path) )
        emit this->deviceRemoved( p->device_list.take(path) );

    if( p->features_list.contains(path) )
        p->features_list.remove( path );

    if( p->disc_list.contains(path) )
    {
        SDiscFeatures disc = p->disc_list.take(path);
        emit this->discRemoved( disc );
    }
}

SUDisksDeviceNotifier::~SUDisksDeviceNotifier()
{
    delete p->dbus_connection;
    delete p;
}
