#ifndef SABSTRACTDISCBURNER_H
#define SABSTRACTDISCBURNER_H

#include <QMap>
#include "sabstractprocess.h"
#include "SiDi_global.h"

class SAbstractDiscBurnerPrivate;
class SIDI_EXPORT SAbstractDiscBurner : public SAbstractProcess
{
    Q_OBJECT
public:
    enum SourceType
    {
        DiscToDisc,
        ImageToDisc,
        Unknown
    };

    SAbstractDiscBurner( QObject *parent = 0 );
    ~SAbstractDiscBurner();

    void setDestinationDevice( const SDeviceItem & item );
    void setSourceDevice( const SDeviceItem & item );
    void setImage( const QString & file );

    void setSpeed( int value );
    void setMinimumBuffer( int value );
    void setRingBuffer( int value );
    void setPadSize( int value );

    void setPad( bool stt );
    void setDummy( bool stt );
    void setIsoSize( bool stt );
    void setPublicRead( bool stt );
    void setForce( bool stt );
    void setSessionAtOnce( bool stt );
    void setTrackAtOnce( bool stt );
    void setMultiSession( bool stt );
    void setEject( bool stt );


    const SDeviceItem & destinationDevice() const;
    const SDeviceItem & sourceDevice() const;
    const QString & image() const;

    int speed() const;
    int minimumBuffer() const;
    int ringBuffer() const;
    int padSize() const;
    virtual int imageSize() const;

    bool pad() const;
    bool dummy() const;
    bool isoSize() const;
    bool publicRead() const;
    bool force() const;
    bool sessionAtOnce() const;
    bool trackAtOnce() const;
    bool multiSession() const;
    bool eject() const;


    SourceType sourceType() const;

signals:
    void ringBufferChanged( int percent );
    void bufferChanged( int percent );
    void writeSizeChenged( int value );
    void writeSpeedChanged( double value );

private:
    SAbstractDiscBurnerPrivate *pp;
};

#endif // SABSTRACTDISCBURNER_H
