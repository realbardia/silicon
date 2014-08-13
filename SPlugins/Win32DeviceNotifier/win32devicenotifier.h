#ifndef WIN32DEVICENOTIFIER_H
#define WIN32DEVICENOTIFIER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QString>

#include <SDeviceItem>
#include <SDiscFeatures>
#include <SAbstractDeviceNotifier>

#include <stdio.h>
#include <conio.h>
#include <winsock.h>
#include <iostream>
#include <tchar.h>
#include <dbt.h>
#include <windows.h>

class CdRecordHW;
class Win32DeviceNotifierPrivate;

class Win32DeviceNotifier : public SAbstractDeviceNotifier
{
    Q_OBJECT
public:
    Win32DeviceNotifier(QObject *parent = 0);
    ~Win32DeviceNotifier();

    void refresh();

    QVariant getProperty( const QString & device , const QString & key ) const;
    QList<SDeviceItem> deviceList() const;

    bool mount( const SDeviceItem & device , const QString & mount_point );
    bool unmount( const SDeviceItem & device );
    void eject( const SDeviceItem & device );

    const SDeviceFeatures & deviceFeatures( const SDeviceItem & device );
    const SDiscFeatures   & discFeatures( const SDeviceItem & device );

private slots:
    void device_scaned();
    void prcapFinished( CdRecordHW *cdrecord , const QString & str );

    bool winEvent ( MSG * msg, long * result );

signals:
    void USB_Plugged(QChar tchar);
    void USB_Removed(QChar tchar);

private:
    Win32DeviceNotifierPrivate *p;
};

#endif // WIN32DEVICENOTIFIER_H
