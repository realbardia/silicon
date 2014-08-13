#ifndef SGRAPHICTIMER_H
#define SGRAPHICTIMER_H

#include <QWidget>

class SGraphicTimerPrivate;
class SGraphicTimer : public QWidget
{
    Q_OBJECT
public:
    SGraphicTimer(QWidget *parent = 0);
    ~SGraphicTimer();

signals:
    void finished();
    void finished( bool stt );

public slots:
    void start( int milisecond );
    void stop();

private slots:
    void timer_shot();

protected:
    void paintEvent( QPaintEvent *event );

private:
    SGraphicTimerPrivate *p;
};

#endif // SGRAPHICTIMER_H
