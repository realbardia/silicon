#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include <SApplication>
#include <SAboutData>

class ScriptRunnerPrivate;
class ScriptRunner : public SApplication
{
    Q_OBJECT
public:
    ScriptRunner( const SAboutData & parent );
    ~ScriptRunner();

public slots:
    void start();

signals:

private slots:
    void start_prev();
    void appDestroyed( SApplication *app );

    void finish();

private:
    void defineVariable( const QString & name , SApplication *value = 0 );
    SApplication *run( const QString & command );

private:
    ScriptRunnerPrivate *p;
};

#endif // SCRIPTRUNNER_H
