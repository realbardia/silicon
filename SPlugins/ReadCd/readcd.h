#ifndef READCD_H
#define READCD_H

#include <SAbstractDiscReader>
#include <SProcessEvent>

class ReadCdPrivate;
class ReadCd : public SAbstractDiscReader
{
    Q_OBJECT

public:
    ReadCd( QObject *parent = 0 );
    ~ReadCd();

    virtual QString logs() const;
    virtual QString usedCommand() const;

    virtual STime elapsedTime() const;
    virtual STime remainingTime() const;

    QString application() const;

protected:
    virtual void goEvent( SProcessEvent *event );
    void stopEvent();

private slots:
    void processOnOutput();
    void processOnError();
    void stopTimer();
    void clockJobs();
    void checkItemicLog( const QString & str );

    void step_1();
    void step_2();

private:
    void reset();
    void checkProgressLine( const QString & str );

private:
    ReadCdPrivate *p;
};

#endif // READCD_H
