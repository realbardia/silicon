#ifndef SPEEDDETECTOR_H
#define SPEEDDETECTOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>

#include <SDeviceItem>

class SpeedDetectorPrivate;

class SpeedDetector : public QObject
{
    Q_OBJECT

public:
    SpeedDetector( QObject *parent = 0 );
    ~SpeedDetector();

public slots:
    void prcap( const SDeviceItem & item );

signals:
    void prcapFinished( const QString & str );
    void prcapFinished( SpeedDetector *cdrecord , const QString & str );
    void deviceSpeedsDetected( QList<int> list );

private slots:
    void prcap_prev();

private:
    QList<int> detectSpeedFromString( const QStringList & list );

private:
    SpeedDetectorPrivate *p;
};

#endif // SPEEDDETECTOR_H
