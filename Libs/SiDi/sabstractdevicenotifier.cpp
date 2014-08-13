#include "sabstractdevicenotifier.h"

SAbstractDeviceNotifier::SAbstractDeviceNotifier(QObject *parent) :
    QObject(parent)
{
}

void SAbstractDeviceNotifier::refresh()
{

}

QVariant SAbstractDeviceNotifier::getProperty( const QString & , const QString & ) const
{
    return QVariant();
}

QList<SDeviceItem> SAbstractDeviceNotifier::deviceList() const
{
    return QList<SDeviceItem>();
}

bool SAbstractDeviceNotifier::mount( const SDeviceItem & , const QString & )
{
    return false;
}

bool SAbstractDeviceNotifier::unmount( const SDeviceItem & )
{
    return false;
}

void SAbstractDeviceNotifier::eject( const SDeviceItem & )
{
}

const SDeviceFeatures & SAbstractDeviceNotifier::deviceFeatures( const SDeviceItem & )
{
}

const SDiscFeatures   & SAbstractDeviceNotifier::discFeatures( const SDeviceItem & )
{
}

SAbstractDeviceNotifier::~SAbstractDeviceNotifier()
{
}
