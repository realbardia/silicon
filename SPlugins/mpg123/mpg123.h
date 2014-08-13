#ifndef MPG123_H
#define MPG123_H

#include <SAbstractConverter>
#include <SProcessEvent>

class MPG123Private;
class MPG123 : public SAbstractConverter
{
    Q_OBJECT

public:
    MPG123( QObject *parent = 0 );
    ~MPG123();

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
    void clockJobs();
    void stopTimer();

private:
    MPG123Private *p;
};

#endif // MPG123_H
