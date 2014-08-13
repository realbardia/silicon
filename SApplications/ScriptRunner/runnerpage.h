#ifndef RUNNERPAGE_H
#define RUNNERPAGE_H

#include <SPage>

class SApplication;
class RunnerPagePrivate;
class RunnerPage : public SPage
{
    Q_OBJECT
public:
    RunnerPage( SApplication *parent );
    ~RunnerPage();

    QString script() const;

public slots:

signals:
    void started();

private slots:
    void start_prev();
    void example();

private:
    void init_actions();

private:
    RunnerPagePrivate *p;
};

#endif // RUNNERPAGE_H
