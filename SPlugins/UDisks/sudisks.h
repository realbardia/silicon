#ifndef SUDISKSDEVICENOTIFIER_H
#define SUDISKSDEVICENOTIFIER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QString>

#include <SDeviceItem>
#include <SDiscFeatures>
#include <SAbstractDeviceNotifier>

class QDBusConnection;
class QDBusMessage;
class SUDisksDeviceNotifierPrivate;
class SpeedDetector;

class SUDisksDeviceNotifier : public SAbstractDeviceNotifier
{
    Q_OBJECT
public:
    SUDisksDeviceNotifier(QObject *parent = 0);
    ~SUDisksDeviceNotifier();

    void refresh();

    QVariant getProperty( const QString & device , const QString & key ) const;
    QList<SDeviceItem> deviceList() const;

    bool mount( const SDeviceItem & device , const QString & mount_point );
    bool unmount( const SDeviceItem & device );
    void eject( const SDeviceItem & device );

    const SDeviceFeatures & deviceFeatures( const SDeviceItem & device );
    const SDiscFeatures   & discFeatures( const SDeviceItem & device );

private slots:
    void dumpChengedMessage( const QDBusMessage & message );
    void dumpAddedMessage( const QDBusMessage & message );
    void dumpRemovedMessage( const QDBusMessage & message );
    void detect( const QString & name );
    void detectDevice( const QString & name );
    void detectDisc( const QString & name );
    void speedDetected( SpeedDetector *cdrecord , const QString & str );

private:
    SUDisksDeviceNotifierPrivate *p;
};

#endif // SUDISKSDEVICENOTIFIER_H
