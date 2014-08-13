#ifndef CDRECORD_H
#define CDRECORD_H

#include <QObject>
#include <QAbstractButton>

#include <SAbstractDiscBurner>
#include <SProcessEvent>
#include <SDeviceItem>

class CdRecordPrivate;
class CdRecord : public SAbstractDiscBurner
{
    Q_OBJECT

public:
    CdRecord( QObject *parent = 0 );
    ~CdRecord();

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
    CdRecordPrivate *p;
};

#endif // CDRECORD_H
