#include "win32devicenotifier.h"

#include <QProcess>
#include <QMainWindow>
#include <QDebug>

#include <Silicon>
#include "cdrecordhw.h"

QList<int> speeds_list;


char FirstDriveFromMask (ULONG unitmask)
{
    char i;

    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;

        unitmask = unitmask >> 1;
    }

    return (i + 'A');
}


class Win32DeviceNotifierPrivate
{
public:
    QHash<QString,SDeviceItem> device_list;
    QHash<QString,SDeviceFeatures> features_list;
    QHash<QString,SDiscFeatures> disc_list;

    QHash<CdRecordHW*,SDeviceItem> cdrecord_hashs;
    CdRecordHW *cdrecord;
};



Win32DeviceNotifier::Win32DeviceNotifier(QObject *parent) :
    SAbstractDeviceNotifier(parent)
{
    if( speeds_list.isEmpty() )
        speeds_list << 56 << 52 << 48 << 40 << 32 << 24 << 16 << 12 << 10 << 8 << 4 << 2 ;

    p = new Win32DeviceNotifierPrivate;
    p->cdrecord = new CdRecordHW( this );

    connect( Silicon::mainWindow() , SIGNAL(winEventSignal(MSG*,long*)) , SLOT(winEvent(MSG*,long*)) );

    connect( p->cdrecord , SIGNAL(devicesScaned()) , SLOT(device_scaned()) );

    refresh();
}

void Win32DeviceNotifier::refresh()
{
    p->cdrecord->scanbus();
}

void Win32DeviceNotifier::device_scaned()
{
    const QList<SDeviceItem> & list = p->device_list.values();
    const QList<SDeviceItem> & n_list = p->cdrecord->devicesList();

    /*! ---------------- REMOVE REMOVED DEVICES ------------------ */
    for( int i=0 ; i<list.count() ; i++ )
    {
        SDeviceItem device = list.at(i);
        if( !n_list.contains(device) )
        {
            p->device_list.remove( device.toQString() );
            p->disc_list.remove( device.toQString() );

            emit deviceRemoved( device );
        }
    }



    /*! ------------------- ADD NEW DEVICES --------------------- */
    for( int i=0 ; i<n_list.count() ; i++ )
    {
        SDeviceItem device = n_list.at(i);

        p->device_list.insert( device.toQString() , device );
        p->disc_list.insert( device.toQString() , device.currentDiscFeatures() );

        emit deviceDetected( device );

        CdRecordHW *cdrhw = new CdRecordHW( this );
            cdrhw->prcap( device );

        connect( cdrhw , SIGNAL(prcapFinished(CdRecordHW*,QString)) , SLOT(prcapFinished(CdRecordHW*,QString)) , Qt::QueuedConnection );
    }


}

void Win32DeviceNotifier::prcapFinished( CdRecordHW *cdrecord , const QString & str )
{
    SDeviceItem     device   = p->cdrecord_hashs.value( cdrecord );
    SDeviceFeatures features = device.deviceFeatures();
    QString         name     = device.toQString();

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

QVariant Win32DeviceNotifier::getProperty( const QString & device , const QString & key ) const
{
    return QVariant();
}

bool Win32DeviceNotifier::mount( const SDeviceItem & device , const QString & mount_point )
{
    return false;
}

bool Win32DeviceNotifier::unmount( const SDeviceItem & device )
{
    return false;
}

void Win32DeviceNotifier::eject( const SDeviceItem & device )
{
}

QList<SDeviceItem> Win32DeviceNotifier::deviceList() const
{
    return p->device_list.values();
}

const SDeviceFeatures & Win32DeviceNotifier::deviceFeatures( const SDeviceItem & device )
{
    QString name = p->device_list.key( device );
    return p->features_list[name];
}

const SDiscFeatures   & Win32DeviceNotifier::discFeatures( const SDeviceItem & device )
{
    QString name = p->device_list.key( device );
    return p->disc_list[name];
}

bool Win32DeviceNotifier::winEvent( MSG * msg, long * )
{
    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        qDebug() << "winEvent in MgFrame : WM_DEVICECHANGE" ;
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;

        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(DBTF_NET)
                {
                    char ALET = FirstDriveFromMask(lpdbv->dbcv_unitmask);
                    emit USB_Plugged(QChar(ALET));
                    refresh();
                }
            }
            break;

        case DBT_DEVICEREMOVECOMPLETE:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(DBTF_NET)
                {
                    char ALET = FirstDriveFromMask(lpdbv->dbcv_unitmask);
                    emit USB_Removed(QChar(ALET));
                    refresh();
                }
            }
            break;
        } // skip the rest
    } // end of if msgType

    return false; // let qt handle the rest
}

Win32DeviceNotifier::~Win32DeviceNotifier()
{
    delete p;
}
