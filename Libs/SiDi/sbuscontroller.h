#ifndef SBUSCONTROLLER_H
#define SBUSCONTROLLER_H

#include <QObject>
#include <QVariant>

#include "sdeviceitem.h"
#include "SiDi_global.h"

class SBusControllerPrivate;
class SIDI_EXPORT SBusController : public QObject
{
    Q_OBJECT
public:
    SBusController( QObject *parent = 0 );
    ~SBusController();

    static bool isBusy( SDeviceItem device );
    static bool isBusy( const QString & device );

    bool isActived() const;
    bool isDeactived() const;

    bool getAccess( SDeviceItem device , const QVariant & data = 0 );
    bool getAccess( const QString & device , const QVariant & data = 0 );

    void finish();
    void finish( SDeviceItem device );
    void finish( const QString & device );

    bool passUpAndWait() const;

public slots:
    void setPassUpAndWait( bool state );

signals:
    void go( const QVariant & data = 0 );

private:
    SBusControllerPrivate *p;
};

#endif // SBUSCONTROLLER_H
