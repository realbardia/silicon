#ifndef SABSTRACTDISCREADER_H
#define SABSTRACTDISCREADER_H

#include "sabstractprocess.h"
#include "SiDi_global.h"

class SAbstractDiscReaderPrivate;
class SIDI_EXPORT SAbstractDiscReader : public SAbstractProcess
{
    Q_OBJECT
public:
    SAbstractDiscReader( QObject *parent = 0 );
    ~SAbstractDiscReader();

    void setDevice( const SDeviceItem & item );
    void setPath( const QString & file );
    void setSectors( int start , int end );

    const SDeviceItem & currentDevice() const;
    const QString & file() const;

    int startSector() const;
    int endSector() const;

signals:
    void doneSizeChanged( int value );
    void counterChanged( int value );

private:
    SAbstractDiscReaderPrivate *pp;
};

#endif // SABSTRACTDISCREADER_H
