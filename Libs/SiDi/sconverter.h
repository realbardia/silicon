#ifndef SCONVERTER_H
#define SCONVERTER_H

#include "sabstractprocess.h"
#include "sabstractconverter.h"
#include "SiDi_global.h"

#include <QObject>
#include <QHash>
#include <QMultiHash>
#include <QString>
#include <QVariant>

class SAbstractConverter;
class SAbstractConverterEngine;
class SConverterPrivate;
class SIDI_EXPORT SConverter : public QObject
{
    Q_OBJECT
public:
    SConverter( QObject *parent = 0 );
    ~SConverter();

    static SAbstractConverter *converter( const QString & source_type , const QString & destination_type , QObject *parent = 0 );

    static void addConverterEngine( SAbstractConverterEngine *engine , const QMultiHash<QString,QString> & types );
    static void removeConverterEngine( SAbstractConverterEngine *engine , const QMultiHash<QString,QString> & types );

    void setSource( const QString & source , const QString & type );
    const QString & source() const;
    const QString & sourceType() const;

    void setDestination( const QString & destination , const QString & type );
    const QString & destination() const;
    const QString & destinationType() const;

    QStringList wayOf( const QString & src_type , const QString & dst_type ) const;
    const QMultiHash<QString,QString> & availableWays() const;

    QHash<QString,QVariant> optionsOf( const QString & src_type , const QString & dst_type ) const;
    void setDefaultOption( const QString & src_type , const QString & dst_type , const QHash<QString,QVariant> & options );

    QString logs() const;
    QString usedCommand() const;

    bool isFinished() const;
    bool isStarted() const;

    STime elapsedTime() const;
    STime remainingTime() const;

    QString application() const;

    bool passupAndWait() const;

public slots:
    void setPassupAndWait( bool stt );

    void start();
    void stop();

signals:
    void preview( const QByteArray & bytes );
    void preview( const QVariant & variant );

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

private slots:
    void start_prev();
    void logChanged_slot( const QString & log );
    void percentChanged_slot( int percent );

private:
    SConverterPrivate *p;
};

#endif // SCONVERTER_H
