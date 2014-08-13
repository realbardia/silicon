#ifndef SABSTRACTIMAGEMOUNTER_H
#define SABSTRACTIMAGEMOUNTER_H

#include "sabstractprocess.h"

class SAbstractImageMounterPrivate;
class SAbstractImageMounter : public SAbstractProcess
{
    Q_OBJECT
public:
    SAbstractImageMounter( QObject *parent = 0 );
    ~SAbstractImageMounter();

    virtual bool isMount( const QString & file ) const;

    virtual QString mountPoint( const QString & file ) const;
    virtual QString file( const QString & mountPoint ) const;

    virtual QStringList mountedList() const;

public slots:
    virtual void mount( const QString & file , const QString & mount_point );
    virtual void unmount( const QString & mount_point );
    virtual void reload();

signals:
    void mounted( bool stt );
    void unmounted( bool stt );

private:
    SAbstractImageMounterPrivate *pp;
};

#endif // SABSTRACTIMAGEMOUNTER_H
