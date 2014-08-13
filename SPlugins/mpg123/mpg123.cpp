#include "mpg123.h"

#include <STime>
#include <SDesktopFunctions>

#include <QDir>
#include <QProcess>
#include <QList>
#include <QFile>
#include <QTimer>
#include <QDebug>

class MPG123Private
{
public:
    QString command;

    QProcess *process;
    QTimer *timer;
    QTimer *clock;

    int process_precent;

    int done_frames;
    int all_frames;

    QString log_str;
    QString used_command;

    STime elapsed_time;
    STime remaining_time;
};

MPG123::MPG123( QObject *parent ) :
    SAbstractConverter( parent )
{
    p = new MPG123Private;
    p->done_frames = 0;
    p->all_frames  = 1;

    p->process = new QProcess( this );
        p->process->setProcessChannelMode( QProcess::MergedChannels );

    p->timer = new QTimer( this );
    p->clock = new QTimer( this );


#ifdef Q_OS_WIN32
    QDir dir( SDesktopFunctions::binaryPath() );
        dir.cdUp();

    p->command = dir.path() + "/mpg123.exe";
    p->command.replace( "/" , "\\" );
#else
    p->command = "mpg123";
#endif


    connect( p->timer       , SIGNAL(timeout())                 , SLOT(processOnOutput()) );
    connect( p->process     , SIGNAL(finished(int))             , SLOT(stopTimer())       );
    connect( p->process     , SIGNAL(readyReadStandardOutput()) , SLOT(processOnOutput()) );
    connect( p->process     , SIGNAL(readyReadStandardError())  , SLOT(processOnError())  );
}

STime MPG123::elapsedTime() const
{
    return p->elapsed_time;
}

STime MPG123::remainingTime() const
{
    return p->remaining_time;
}

QString MPG123::logs() const
{
    return p->log_str;
}

QString MPG123::usedCommand() const
{
    return p->used_command;
}

QString MPG123::application() const
{
    return p->command;
}

void MPG123::stopEvent()
{
    p->process->kill();
}

void MPG123::goEvent( SProcessEvent *event )
{
    if( !event->address().isEmpty() )
        setDestination( event->address() );

    QStringList arguments;
        arguments << "-v";
        arguments << "--rate";
        arguments << "44100";
        arguments << "--stereo";
        arguments << "--buffer";
        arguments << "3072";
        arguments << "--resync";
        arguments << "-w";
        arguments << destination();
        arguments << source();

    p->used_command.clear();
    p->used_command = application() + " ";
    for( int i=0 ; i<arguments.count() ; i++ )
    {
        QString str = arguments.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        p->used_command = p->used_command + str + " ";
    }

    p->log_str = p->used_command;
    emit itemicLogAdded( MPG123::Information , p->used_command );

    p->process->start( application() , arguments );

    p->timer->start( 25 );
    p->clock->start( 1000 );
}

void MPG123::processOnOutput()
{
    QString proc_str = p->process->readLine();
        proc_str.remove("\n");

    if( !proc_str.isEmpty() )
    {
        QRegExp reg;
        int pos;

        reg.setPattern( "Frame#\\s*(\\d+)\\s*\\[\\s*(\\d+)\\].*" );
        pos = reg.indexIn( proc_str );
        if( pos > -1 )
        {
            p->done_frames = reg.cap(1).toInt();
            p->all_frames  = reg.cap(2).toInt() + p->done_frames -1;

            p->process_precent = 100*((double)p->done_frames/p->all_frames);
            emit this->percentChanged( p->process_precent );
        }
    }

    if( isFinished() && proc_str == NULL )
    {
        p->timer->stop();
        p->clock->stop();
    }
}

void MPG123::processOnError()
{
    QString error_str = p->process->readAllStandardError();
    if( !error_str.isEmpty() )
        emit error( error_str );
}

void MPG123::clockJobs()
{
    p->elapsed_time.addSecond( 1 );

    int remain_sec = 0;
    if( p->all_frames !=0 )
        remain_sec = p->elapsed_time.toSecond()*(p->all_frames-p->done_frames)/p->done_frames;

    p->remaining_time.reset();
    p->remaining_time.setSecond( remain_sec );

    emit elapsedTimeChanged(   p->elapsed_time              );
    emit remainingTimeChanged( p->remaining_time            );
    emit elapsedTimeChanged(   p->elapsed_time.toString()   );
    emit remainingTimeChanged( p->remaining_time.toString() );
}

void MPG123::stopTimer()
{
    emit this->itemicLogAdded( MPG123::Information , "Finished" );
    emit this->finished( p->process->exitCode() );

    dropProcess();
}

MPG123::~MPG123()
{
    if( isStarted() )
        stopTimer();

    delete p;
}
