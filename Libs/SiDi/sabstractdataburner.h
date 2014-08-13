#ifndef SABSTRACTDATABURNER_H
#define SABSTRACTDATABURNER_H

#include "sabstractprocess.h"
#include "SiDi_global.h"

class SAbstractDataBurnerPrivate;
class SIDI_EXPORT SAbstractDataBurner : public SAbstractProcess
{
    Q_OBJECT
public:
    SAbstractDataBurner( QObject *parent = 0 );
    ~SAbstractDataBurner();

    void setPathSpec( const QStringList & path );
    void addPathSpec( const QStringList & path );

    void setJolietDirectory( bool stt );
    void setRockRidge( bool stt );
    void setGraphtPoint( bool stt );
    void setDeepDirectory( bool stt );
    void setAllowLeadingDots( bool stt );
    void setAllowLowercase( bool stt );

    void setCopyRight( const QString & copyright );
    void setAbstract( const QString & abstract );
    void setPublisher( const QString & publisher );
    void setSystemID( const QString & sys );
    void setApplicationID( const QString & application );
    void setBiblioGraphic( const QString & biblio_graphic );
    void setVolumeID( const QString & volume );

    void setDevice( const SDeviceItem & item );
    void setSpeed( int value );
    void setMinimumBuffer( int value );
    void setRingBuffer( int value );
    void setPadSize( int value );

    void setPad( bool stt );
    void setDummy( bool stt );
    void setPublicRead( bool stt );
    void setForce( bool stt );
    void setSessionAtOnce( bool stt );
    void setTrackAtOnce( bool stt );
    void setMultiSession( bool stt );
    void setEject( bool stt );




    const QStringList & pathSpec() const;

    bool jolietDirState() const;
    bool rockRidgeState() const;
    bool graphPointState() const;
    bool quietState() const;
    bool sizeDetectorState() const;
    bool deepDirectoryState() const;
    bool allowLeadingDotsState() const;
    bool allowLowercaseState() const;

    const QString & copyright() const;
    const QString & abstract() const;
    const QString & publisher() const;
    const QString & systemID() const;
    const QString & applicationID() const;
    const QString & biblioGraphic() const;
    const QString & volume() const;

    const SDeviceItem & currentDevice() const;
    int speed() const;
    int minimumBuffer() const;
    int ringBuffer() const;
    int padSize() const;
    virtual int imageSize() const;

    bool pad() const;
    bool dummy() const;
    bool publicRead() const;
    bool force() const;
    bool sessionAtOnce() const;
    bool trackAtOnce() const;
    bool multiSession() const;
    bool eject() const;

signals:
    void ringBufferChanged( int percent );
    void bufferChanged( int percent );
    void writeSizeChenged( int value );
    void writeSpeedChanged( double value );
    void imageSizeDetected( int size );

protected:
    virtual void pathSpecChanged( const QStringList & list );

private:
    SAbstractDataBurnerPrivate *pp;
};

#endif // SABSTRACTDATABURNER_H
