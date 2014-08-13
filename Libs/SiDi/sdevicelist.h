#ifndef SDEVICELIST_H
#define SDEVICELIST_H

#include <QObject>
#include <QList>

#include "sdevicefeatures.h"
#include "sdiscfeatures.h"
#include "sdeviceitem.h"

#include "SiDi_global.h"

class SAbstractDeviceNotifier;
class SIDI_EXPORT SDeviceList : public QObject
{
    Q_OBJECT
public:
    SDeviceList(QObject *parent = 0);
    ~SDeviceList();

    static void setNotifier( SAbstractDeviceNotifier *notifier );
    QList<SDeviceItem> deviceList() const;

    static bool mount( const SDeviceItem & device , const QString & mount_point = QString() );
    static bool unmount( const SDeviceItem & device );
    static void eject( const SDeviceItem & device );

    static const SDeviceFeatures & deviceFeatures( const SDeviceItem & device );
    static const SDiscFeatures   & discFeatures( const SDeviceItem & device );

public slots:
    void refresh();

signals:
    void deviceDetected( const SDeviceItem & disc );
};

#endif // SDEVICELIST_H
