#ifndef SABSTRACTDEVICENOTIFIER_H
#define SABSTRACTDEVICENOTIFIER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QString>

#include "sdeviceitem.h"
#include "sdiscfeatures.h"

#include "SiDi_global.h"

class SIDI_EXPORT SAbstractDeviceNotifier : public QObject
{
    Q_OBJECT
public:
    SAbstractDeviceNotifier( QObject* parent=0 );
    ~SAbstractDeviceNotifier();

    virtual void refresh() = 0;

    virtual QVariant getProperty( const QString & device , const QString & key ) const;
    virtual QList<SDeviceItem> deviceList() const;

    virtual bool mount( const SDeviceItem & device , const QString & mount_point );
    virtual bool unmount( const SDeviceItem & device );
    virtual void eject( const SDeviceItem & device );

    virtual const SDeviceFeatures & deviceFeatures( const SDeviceItem & device );
    virtual const SDiscFeatures   & discFeatures( const SDeviceItem & device );

signals:
    void deviceDetected( const SDeviceItem & item );
    void deviceRemoved( const SDeviceItem & item );

    void discDetected( const SDiscFeatures & item );
    void discRemoved( const SDiscFeatures & item );

signals:

public slots:

};

#endif // SABSTRACTDEVICENOTIFIER_H
