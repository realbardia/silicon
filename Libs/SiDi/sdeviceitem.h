#ifndef SDEVICEITEM_H
#define SDEVICEITEM_H

#include <QString>
#include <QStringList>

#include "sdevicefeatures.h"
#include "sdiscfeatures.h"

#include "SiDi_global.h"

class SIDI_EXPORT SDeviceItem
{
public:
    SDeviceItem();
    SDeviceItem( const QString & name , int bus , int id , int lun );
    ~SDeviceItem();

    void setString( const QString & str );
    void set( const QString & name , int bus , int id , int lun, const QString & path = QString() );
    void setName( const QString & name );
    void setBus( int bus );
    void setId( int id );
    void setLun( int lun );

    void eject() const;
    void mount() const;
    void unmount() const;

    const QList<int> & speeds() const;
    const SDeviceFeatures & deviceFeatures() const;
    const SDiscFeatures & currentDiscFeatures() const;

    QString toQString() const;
    const QString & name() const;
    const QString & address() const;
    int bus() const;
    int id() const;
    int lun() const;

    bool isEmpty() const;

    SDeviceItem operator=( SDeviceItem item );
    bool operator==( const SDeviceItem & item ) const;
    bool operator!=( const SDeviceItem & item ) const;

private:
    int scsi_bus;
    int scsi_id;
    int scsi_lun;

    QString device_name;
    QString path;
};

#endif // SDEVICEITEM_H
