#ifndef SHALDEVICENOTIFIER_H
#define SHALDEVICENOTIFIER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QString>

#include <SDeviceItem>
#include <SDiscFeatures>
#include <SAbstractDeviceNotifier>

class QDBusConnection;
class QDBusMessage;
class HalUDiskPrivate;

class HalUDisk : public SAbstractDeviceNotifier
{
    Q_OBJECT
public:
    HalUDisk(QObject *parent = 0);
    ~HalUDisk();

    void refresh();

    QVariant getProperty( const QString & device , const QString & key ) const;
    QList<SDeviceItem> deviceList() const;

    bool mount( const SDeviceItem & device , const QString & mount_point );
    bool unmount( const SDeviceItem & device );
    void eject( const SDeviceItem & device );

    const SDeviceFeatures & deviceFeatures( const SDeviceItem & device );
    const SDiscFeatures   & discFeatures( const SDeviceItem & device );

private slots:
    void dumpPropertyMessage( const QDBusMessage & message );
    void dumpAddedMessage( const QDBusMessage & message );
    void dumpRemovedMessage( const QDBusMessage & message );
    void detect( const QString & name );
    void detectDevice( const QString & name );
    void detectDisc( const QString & name );

private:
    HalUDiskPrivate *p;
};

#endif // SHALDEVICENOTIFIER_H
