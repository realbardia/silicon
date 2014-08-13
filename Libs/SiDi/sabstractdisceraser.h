#ifndef SABSTRACTDISCERASER_H
#define SABSTRACTDISCERASER_H

#include <QObject>

#include "sabstractprocess.h"
#include "SiDi_global.h"

class SAbstractDiscEraserPrivate;
class SIDI_EXPORT SAbstractDiscEraser : public SAbstractProcess
{
    Q_OBJECT
public:
    SAbstractDiscEraser( QObject *parent = 0 );
    ~SAbstractDiscEraser();

    void setDevice( const SDeviceItem & item );
    void setForce( bool stt );
    void setLeadOut( bool stt );

    const SDeviceItem & device() const;
    bool force() const;
    bool leadOut() const;

private:
    SAbstractDiscEraserPrivate *pp;
};

#endif // SABSTRACTDISCERASER_H
