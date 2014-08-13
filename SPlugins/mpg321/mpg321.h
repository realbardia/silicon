#ifndef MPG321_H
#define MPG321_H

#include <SAbstractConverter>
#include <SProcessEvent>

class MPG321Private;
class MPG321 : public SAbstractConverter
{
    Q_OBJECT

public:
    MPG321( QObject *parent = 0 );
    ~MPG321();

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
    MPG321Private *p;
};

#endif // MPG321_H
