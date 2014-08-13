#include "sdevicelist.h"
#include "sabstractdevicenotifier.h"

#include "sdeviceitem.h"

SAbstractDeviceNotifier *device_notifier = 0;
QList<SDeviceList *> objects_created;

SDeviceList::SDeviceList(QObject *parent) :
    QObject(parent)
{
    objects_created << this;
    if( device_notifier == 0 )
        return;

    QObject::connect( device_notifier , SIGNAL(deviceDetected(SDeviceItem)) , SIGNAL(deviceDetected(SDeviceItem)) );
}

void SDeviceList::setNotifier( SAbstractDeviceNotifier *notifier )
{
    if( device_notifier != 0 )
        delete device_notifier;

    device_notifier = notifier;

    if( notifier == 0 )
        return ;

    for( int i=0 ; i<objects_created.count() ; i++ )
        QObject::connect( device_notifier , SIGNAL(deviceDetected(SDeviceItem)) , objects_created.at(i) , SIGNAL(deviceDetected(SDeviceItem)) );
}

void SDeviceList::refresh()
{
    if( device_notifier == 0 )
        return;

    device_notifier->refresh();
}

QList<SDeviceItem> SDeviceList::deviceList() const
{
    if( device_notifier == 0 )
        return QList<SDeviceItem>();

    return device_notifier->deviceList();
}

bool SDeviceList::mount( const SDeviceItem & device , const QString & mount_point )
{
    return device_notifier->mount( device , mount_point );
}

bool SDeviceList::unmount( const SDeviceItem & device )
{
    return device_notifier->unmount( device );
}

void SDeviceList::eject( const SDeviceItem & device )
{
    device_notifier->eject( device );
}

const SDeviceFeatures & SDeviceList::deviceFeatures( const SDeviceItem & device )
{
    return device_notifier->deviceFeatures( device );
}

const SDiscFeatures   & SDeviceList::discFeatures( const SDeviceItem & device )
{
    return device_notifier->discFeatures( device );
}

SDeviceList::~SDeviceList()
{
    objects_created.removeOne( this );
}
