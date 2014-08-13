#ifndef SSCANDISC_H
#define SSCANDISC_H

#include <QObject>

#include "sdeviceitem.h"
#include "SiDi_global.h"

class SDiscInfo;
class SScanDiscPrivate;

class SIDI_EXPORT SScanDisc : public QObject
{
    Q_OBJECT
public:
    SScanDisc(QObject *parent = 0);
    ~SScanDisc();

    void scan( const SDeviceItem & device );

public slots:
    void debug( const QString & str );

signals:
    void log( const QString & log );
    void finished( bool result );

private slots:
    void step_start();
    void step_0_done();
    void step_1_done( const SDiscInfo & disc );
    void step_2_done( const QString & str );
    void step_3_done( const SDiscInfo & disc , bool stt );
    void step_4_done( const SDeviceItem & device );
    void step_5_done( const QMap<QString,QString> & map );

    void finished_unsuccessfully( int on_step );

private:
    SScanDiscPrivate *p;
};

#endif // SSCANDISC_H
