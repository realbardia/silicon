#include "readcd.h"

#include <STime>
#include <SDeviceItem>
#include <SDesktopFunctions>
#include <SDiscDetector>

#include <QDir>
#include <QProcess>
#include <QList>
#include <QFile>
#include <QTimer>

class ReadCdPrivate
{
public:
    QString used_command;
    QString log_str;

    int process_percent;
    int done_size;
    int counter_size;

    STime elapsed_time;
    STime remaining_time;

    SDiscDetector *detector;

    QProcess *process;

    QTimer *timer;
    QTimer *clock;
};

ReadCd::ReadCd( QObject *parent ) :
    SAbstractDiscReader( parent )
{
    p = new ReadCdPrivate;

    p->process = new QProcess( this );
        p->process->setProcessChannelMode( QProcess::MergedChannels );

    p->detector = new SDiscDetector( this );

    p->timer = new QTimer( this );
    p->clock = new QTimer( this );

    connect( p->timer       , SIGNAL(timeout())                 , SLOT(processOnOutput()) );
    connect( p->clock       , SIGNAL(timeout())                 , SLOT(clockJobs())       );
    connect( p->process     , SIGNAL(finished(int))             , SLOT(stopTimer())       );
    connect( p->process     , SIGNAL(readyReadStandardOutput()) , SLOT(processOnOutput()) );
    connect( p->process     , SIGNAL(readyReadStandardError())  , SLOT(processOnError())  );
    connect( p->detector    , SIGNAL(accepted())                , SLOT(step_2())          );
    connect( p->detector    , SIGNAL(rejected())                , SLOT(stopTimer())       );
}


QString ReadCd::logs() const
{
    return p->log_str;
}

QString ReadCd::usedCommand() const
{
    return p->used_command;
}

STime ReadCd::elapsedTime() const
{
    return p->elapsed_time;
}

STime ReadCd::remainingTime() const
{
    return p->remaining_time;
}

QString ReadCd::application() const
{
    QString app;

    QStringList list_of_programs;
        list_of_programs << "readcd" << "readom";

    for( int i=0 ; i<list_of_programs.count() ; i++ )
    {
#ifdef Q_OS_WIN32
        QDir dir( SDesktopFunctions::binaryPath() );
            dir.cdUp();

        app = dir.path() + "/cdrtools/" + list_of_programs.at(i) + ".exe";
        app.replace( "/" , "\\" );
#else
        app = "/usr/bin/" + list_of_programs.at(i);
#endif

        if( QFile::exists( app ) )
            return app;
    }

    return QString();
}

void ReadCd::stopEvent()
{
    p->process->kill();
}

void ReadCd::goEvent( SProcessEvent *event )
{
    if( !event->device().isEmpty() )
        setDevice( event->device() );

    step_1();
}

void ReadCd::step_1()
{
    p->detector->setSourceDisc( currentDevice() );
    p->detector->check();
}

void ReadCd::step_2()
{
    QStringList args;

    reset();

    args << "dev=" + currentDevice().toQString();
    args << "sectors=" + QString::number( startSector() ) + "-" + QString::number( endSector() );
    args << "f=" + file();

    p->used_command = application() + " ";
    for( int i=0 ; i<args.count() ; i++ )
    {
        QString str = args.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        p->used_command = p->used_command + str + " ";
    }

    p->log_str = p->used_command ;
    emit this->itemicLogAdded( ReadCd::Information , p->used_command );

    p->process->start( application() , args );

    p->timer->start( 25 );
    p->clock->start( 1000 );
}

void ReadCd::processOnOutput()
{
    QString proc_str = p->process->readLine();
        proc_str.remove("\n");

    if( !proc_str.isEmpty() )
    {
        QString tmp = proc_str.simplified();
            tmp.remove( QRegExp("[0-9]") ).remove("\n").remove(" ");

        if( tmp.left(9) == QString("addr:cnt:") )
        {
            if( p->timer->interval() != 1000 )
            {
                p->timer->setInterval( 1000 );
                // qDebug( "void ReadCd::processOnOutput() : Timer Interval set to 1000 ms" );
            }

            this->checkProgressLine( proc_str );
        }
        else
        {
            if( p->timer->interval() != 25 )
            {
                p->timer->setInterval( 25 );
                // qDebug( "void ReadCd::processOnOutput() : Timer Interval set to 25 ms" );
            }

            this->checkItemicLog( proc_str );

            p->log_str = p->log_str + '\n' + proc_str;
            emit this->logChanged( p->log_str );
        }
    }

    if( isFinished() && proc_str == NULL )
    {
        p->timer->stop();
        p->clock->stop();
    }
}

void ReadCd::checkItemicLog( const QString & str )
{
    if( str.left(8) == "readom: " )
        emit this->itemicLogAdded( ReadCd::Information , str.right( str.size()-8 ) );

    else if( str.left(8) == "readcd: " )
        emit this->itemicLogAdded( ReadCd::Information , str.right( str.size()-8 ) );

    else if( str.left(9) == "WARNING: " )
        emit this->itemicLogAdded( ReadCd::Warning , str.right( str.size()-9 ) );

    else if( str.left(6) == "HINT: " )
        emit this->itemicLogAdded( ReadCd::Information , str.right( str.size()-6 ) );

    else if( str == "Fixating..." )
        emit this->itemicLogAdded( ReadCd::Information , str );

}

void ReadCd::processOnError()
{
    QString error_str = p->process->readAllStandardError();
    if( !error_str.isEmpty() )
        emit error( error_str );
}

void ReadCd::checkProgressLine( const QString & str )
{
    QRegExp reg;
    int pos;
    bool ok;

    reg.setPattern( "addr\\:\\s+(\\d+)\\s+cnt\\:\\s+(\\d+)" );
    pos = reg.indexIn( str );
    if( pos > -1 )
    {
        p->done_size = reg.cap(1).toInt(&ok);
        p->counter_size = reg.cap(2).toInt( &ok );
    }

    if( endSector() - startSector() != 0 )
        p->process_percent = 100 * ( p->done_size - startSector() ) / ( endSector() - startSector() );

    emit this->doneSizeChanged( p->done_size );
    emit this->counterChanged( p->counter_size );
    emit this->percentChanged( p->process_percent );
}

void ReadCd::stopTimer()
{
    if( p->process->exitCode() == 0 )
    {
        p->process_percent = 100;
        emit this->percentChanged( p->process_percent );
    }

    emit itemicLogAdded( ReadCd::Information , "Finished" );
    emit finished( p->process->exitCode() );

    dropProcess();
}

void ReadCd::clockJobs()
{
    p->elapsed_time.addSecond( 1 );

    int remain_sec = 0;
    if( p->done_size - startSector() !=0 )
        remain_sec = p->elapsed_time.toSecond()*(endSector()-p->done_size)/( p->done_size - startSector() );

    p->remaining_time.reset();
    p->remaining_time.setSecond( remain_sec );

    emit this->elapsedTimeChanged( p->elapsed_time );
    emit this->remainingTimeChanged( p->remaining_time );
    emit this->elapsedTimeChanged( p->elapsed_time.toString() );
    emit this->remainingTimeChanged( p->remaining_time.toString() );
}

void ReadCd::reset()
{
    if( isStarted() )
        return ;

    p->used_command.clear();
    p->log_str.clear();

    p->process_percent = 0;
    p->done_size = 0;
    p->counter_size = 0;

    p->elapsed_time.reset();
    p->remaining_time.reset();
}

ReadCd::~ReadCd()
{
    if( !isFinished() )
        stop();

    delete p;
}
