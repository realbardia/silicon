#ifndef AUDIOCDRECORD_H
#define AUDIOCDRECORD_H

#include <QObject>
#include <QAbstractButton>

#include <SAbstractAudioBurner>
#include <SProcessEvent>
#include <SDeviceItem>

class AudioCdRecordPrivate;
class AudioCdRecord : public SAbstractAudioBurner
{
    Q_OBJECT

public:
    AudioCdRecord( QObject *parent = 0 );
    ~AudioCdRecord();

    QString logs() const;
    QString usedCommand() const;

    QString application() const;

    STime elapsedTime() const;
    STime remainingTime() const;

    int imageSize() const;

private slots:
    void processOnOutput();
    void processOnError();
    void clockJobs();
    void stopTimer();
    void checkItemicLog( const QString & str );

    void step_1();
    void step_2();

private:
    QStringList arguments() const;
    QString command() const;

    void checkProgressLine( const QString & str );
    void reset();

protected:
    virtual void goEvent( SProcessEvent *event );
    void stopEvent();

private:
    AudioCdRecordPrivate *p;
};

#endif // AUDIOCDRECORD_H
