#ifndef SABASTRACTIMAGECREATOR_H
#define SABASTRACTIMAGECREATOR_H

#include "sabstractprocess.h"
#include "SiDi_global.h"

class SAbstractImageCreatorPrivate;
class SIDI_EXPORT SAbstractImageCreator : public SAbstractProcess
{
    Q_OBJECT
public:
    SAbstractImageCreator( QObject *parent = 0 );
    ~SAbstractImageCreator();

    void setPathSpec( const QStringList & path );
    void addPathSpec( const QStringList & path );

    void setOutPut( const QString & file );

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



    const QStringList & pathSpec() const;
    const QString & outPut() const;

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

    virtual int imageSize() const;

signals:
    void createdSizeChanged( int size );
    void imageSizeDetected( int size );

protected:
    virtual void pathSpecChanged( const QStringList & list );

private:
    SAbstractImageCreatorPrivate *pp;
};

#endif // SABASTRACTIMAGECREATOR_H
