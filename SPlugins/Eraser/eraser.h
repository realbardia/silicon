#ifndef ERASER_H
#define ERASER_H

#include <SAbstractDiscEraser>
#include <SProcessEvent>

class EraserPrivate;
class Eraser : public SAbstractDiscEraser
{
    Q_OBJECT

public:
    Eraser( QObject *parent = 0 );
    ~Eraser();

    virtual QString logs() const;
    virtual QString usedCommand() const;

    virtual STime elapsedTime() const;
    virtual STime remainingTime() const;

    QString application() const;

protected:
    virtual void goEvent( SProcessEvent *event );
    void stopEvent();

private slots:
    void step_1();
    void step_2();

    void processOnOutput();
    void processOnError();
    void stopTimer();

private:
    EraserPrivate *p;
};

#endif // ERASER_H
