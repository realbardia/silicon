#ifndef SABSTRACTPROCESS_H
#define SABSTRACTPROCESS_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "sdeviceitem.h"
#include "stime.h"
#include "sprocessevent.h"

#include "SiDi_global.h"

class SAbstractProcessPrivate;
class SIDI_EXPORT SAbstractProcess : public QObject
{
    Q_OBJECT
public:

    /*!
     * Type of used to partitioning itemic logs.
     */
    enum ItemicLogType {
        Warning,
        Information,
        Fatal,
        Critical
    };

    SAbstractProcess(QObject *parent = 0);
    ~SAbstractProcess();

    virtual QString logs() const;
    virtual QString usedCommand() const;
    virtual QString errorStr() const;

    bool isFinished() const;
    bool isStarted() const;

    virtual STime elapsedTime() const;
    virtual STime remainingTime() const;

    virtual QString application() const;

    bool passupAndWait() const;

public slots:
    void setPassupAndWait( bool stt );

    void stop();
    void start( const SDeviceItem & device );
    void start( const QString & address );

protected:
    virtual void goEvent( SProcessEvent *event );
    virtual void startEvent();
    virtual void stopEvent();

    void dropProcess();

private slots:
    void access_granted();
    void go_prev();
    void drop_prev();

signals:
    void percentChanged( int percent );
    void finished( int exit_code );
    void logChanged( const QString & log );
    void itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message );
    void elapsedTimeChanged( STime time );
    void remainingTimeChanged( STime time );
    void elapsedTimeChanged( QString time );
    void remainingTimeChanged( QString time );
    void error( const QString & str );

    void goEventSignal( SProcessEvent *event );
    void stopSignal();

private:
    SAbstractProcessPrivate *ppp;
};

#endif // SABSTRACTPROCESS_H
