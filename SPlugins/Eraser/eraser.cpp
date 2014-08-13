#include "eraser.h"

#include <STime>
#include <SDeviceItem>
#include <SDesktopFunctions>
#include <SDiscDetector>

#include <QDir>
#include <QProcess>
#include <QList>
#include <QFile>
#include <QTimer>

class EraserPrivate
{
public:
    QString command;

    SDiscDetector *detector;

    QProcess *process;
    QTimer *timer;

    int process_precent;

    QString log_str;
    QString used_command;

    STime elapsed_time;
    STime remaining_time;
};

Eraser::Eraser( QObject *parent ) :
    SAbstractDiscEraser( parent )
{
    p = new EraserPrivate;

    p->detector = new SDiscDetector( this );
    p->process = new QProcess( this );
        p->process->setProcessChannelMode( QProcess::MergedChannels );

    p->timer = new QTimer( this );


#ifdef Q_OS_WIN32
    QDir dir( SDesktopFunctions::binaryPath() );
        dir.cdUp();

    p->command = dir.path() + "/cdrtools/dvd+rw-format.exe";
    p->command.replace( "/" , "\\" );
#else
    p->command = "dvd+rw-format";
#endif


    connect( p->timer       , SIGNAL(timeout())                 , SLOT(processOnOutput()) );
    connect( p->process     , SIGNAL(finished(int))             , SLOT(stopTimer())       );
    connect( p->process     , SIGNAL(readyReadStandardOutput()) , SLOT(processOnOutput()) );
    connect( p->process     , SIGNAL(readyReadStandardError())  , SLOT(processOnError())  );
    connect( p->detector    , SIGNAL(accepted())                , SLOT(step_2())          );
    connect( p->detector    , SIGNAL(rejected())                , SLOT(stopTimer())       );
}

STime Eraser::elapsedTime() const
{
    return p->elapsed_time;
}

STime Eraser::remainingTime() const
{
    return p->remaining_time;
}

QString Eraser::logs() const
{
    return p->log_str;
}

QString Eraser::usedCommand() const
{
    return p->used_command;
}

QString Eraser::application() const
{
    return p->command;
}

void Eraser::stopEvent()
{
    p->process->kill();
}

void Eraser::goEvent( SProcessEvent *event )
{
    if( !event->device().isEmpty() )
        setDevice( event->device() );

    step_1();
}

void Eraser::step_1()
{
    p->detector->setReWritableDisc( device() );
    p->detector->check();
}

void Eraser::step_2()
{
    QStringList args;

    if( force() )
        args << "-force";
    if( leadOut() )
        args << "-lead-out";

    args << device().address() ;

    p->used_command = p->command + " ";
    for( int i=0 ; i<args.count() ; i++ )
    {
        QString str = args.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        p->used_command = p->used_command + str + " ";
    }
    p->log_str = p->used_command ;

    p->process->start( p->command , args );
    p->timer->start( 1000 );
}

void Eraser::processOnOutput()
{
    QString proc_str = p->process->readLine();
        proc_str.remove("\n");

    if( !proc_str.isEmpty() )
    {
        QRegExp reg;
        int pos;
        bool ok;

        reg.setPattern( ".*(\\d*)\\.\\d\\%" );
        pos = reg.indexIn( proc_str );
        if( pos > -1 )
        {
            p->process_precent = reg.cap(1).toInt(&ok);
            emit this->percentChanged( p->process_precent );
        }
    }

    if( isFinished() && proc_str == NULL )
    {
        p->timer->stop();
    }
}

void Eraser::processOnError()
{
    QString error_str = p->process->readAllStandardError();
    if( !error_str.isEmpty() )
        emit error( error_str );
}

void Eraser::stopTimer()
{
    emit this->itemicLogAdded( Eraser::Information , "Finished" );
    emit this->finished( p->process->exitCode() );

    dropProcess();
}

Eraser::~Eraser()
{
    if( isStarted() )
        stopTimer();

    delete p;
}
