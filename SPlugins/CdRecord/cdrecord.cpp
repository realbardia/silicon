#include "cdrecord.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QDebug>
#include <QProcess>

#include <SDiscDetector>
#include <SDesktopFunctions>

class CdRecordPrivate
{
public:
    QProcess *process;

    QTimer *timer;
    QTimer *clock;

    QString log_str;
    QString used_command_str;

    STime elapsed_time;
    STime remaining_time;

    SDiscDetector *detector;

    int written_size_int;
    int image_size_int;
    int ring_buffer_percent_int;
    int buffer_percent_int;
    int written_speed_int;
    int process_precent;
};

CdRecord::CdRecord( QObject *parent )
    : SAbstractDiscBurner( parent )
{
    p = new CdRecordPrivate;

    p->process = new QProcess( this );
        p->process->setProcessChannelMode( QProcess::MergedChannels );

    p->detector = new SDiscDetector( this );

    p->timer   = new QTimer( this );
    p->clock   = new QTimer( this );

    connect( p->timer           , SIGNAL(timeout())                   , SLOT(processOnOutput()) );
    connect( p->clock           , SIGNAL(timeout())                   , SLOT(clockJobs())       );
    connect( p->process         , SIGNAL(finished(int))               , SLOT(stopTimer())       );
    connect( p->process         , SIGNAL(readyReadStandardOutput())   , SLOT(processOnOutput()) );
    connect( p->process         , SIGNAL(readyReadStandardError())    , SLOT(processOnError())  );
    connect( p->detector        , SIGNAL(accepted())                  , SLOT(step_2())          );
    connect( p->detector        , SIGNAL(rejected())                  , SLOT(stopTimer())       );

    reset();
}

QString CdRecord::logs() const
{
    return p->log_str;
}

QString CdRecord::usedCommand() const
{
    return p->used_command_str;
}

QString CdRecord::application() const
{
    return command();
}


STime CdRecord::elapsedTime() const
{
    return p->elapsed_time;
}

STime CdRecord::remainingTime() const
{
    return p->remaining_time;
}

int CdRecord::imageSize() const
{
    return p->image_size_int;
}

void CdRecord::goEvent( SProcessEvent *event )
{
    if( !event->device().isEmpty() )
        setDestinationDevice( event->device() );
    else
        return;

    step_1();
}

void CdRecord::step_1()
{
    switch( static_cast<int>(sourceType()) )
    {
    case CdRecord::ImageToDisc:
        p->detector->setSourceImage( image() );
        break;
    case CdRecord::DiscToDisc:
        p->detector->setSourceDisc( sourceDevice() );
        break;
    }

    p->detector->setDestinationDisc( destinationDevice() );
    p->detector->check();
}

void CdRecord::step_2()
{
    const QStringList & args = arguments();

    p->used_command_str.clear();
    p->used_command_str = command() + " ";
    for( int i=0 ; i<args.count() ; i++ )
    {
        QString str = args.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        p->used_command_str = p->used_command_str + str + " ";
    }

    p->log_str = p->used_command_str;
    emit itemicLogAdded( CdRecord::Information , p->used_command_str );

    p->process->start( command() , args );

    p->timer->start( 25 );
    p->clock->start( 1000 );
}

QStringList CdRecord::arguments() const
{
    QStringList args;

/*! --------------------- Normal Arguments --------------------!*/
    args << "-v";

    if( publicRead()           )        args << "-r";
    if( dummy()                )        args << "-dummy";
    if( force()                )        args << "-force";
    if( sessionAtOnce()        )        args << "-sao";
    if( trackAtOnce()          )        args << "-tao";
    if( multiSession()         )        args << "-multi";
    if( pad()                  )        args << "-pad";
    if( pad() && padSize() > 0 )        args << QString::number( padSize() );
    if( eject()                )        args << "-eject";

    if( ringBuffer() > 0       )        args << "fs="     + QString::number( ringBuffer() );
    if( minimumBuffer() > 0    )        args << "minbuf=" + QString::number( minimumBuffer() );
    if( speed() > 0            )        args << "speed="  + QString::number( speed() );

    args << "dev=" + destinationDevice().toQString();
    /*! -----------------------------------------------------------!*/



    switch( static_cast<int>(sourceType()) )
    {
    case CdRecord::ImageToDisc:
    /*! ------------------- If is Image Arguments -----------------!*/
        if( !image().isEmpty() )
            args << image() ;
    /*! -----------------------------------------------------------!*/
        break;



    case CdRecord::DiscToDisc:
    /*! ------------------- If is Disc Arguments ------------------!*/
        if( isoSize() )
            args << "-isosize";
        if( sourceDevice().isEmpty() )
            args << sourceDevice().address();
    /*! -----------------------------------------------------------!*/
        break;
    }

    return args;
}

QString CdRecord::command() const
{
    QString app;

    QStringList list_of_programs;
        list_of_programs << "cdrecord" << "wodim";

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

void CdRecord::stopEvent()
{
    p->process->kill();
}

void CdRecord::processOnOutput()
{
    QStringList list = QString( p->process->readLine() ).split("\n",QString::SkipEmptyParts);

    for( int i=0 ; i<list.count() ; i++ )
    {
        QString proc_str = list.at(i);

        QString tmp = proc_str.simplified();
            tmp.replace( QRegExp("[0-9]| ") , "" );

        if( tmp == QString("Track:ofMBwritten(fifo%)[buf%].x.") )
        {
            if( p->timer->interval() != 1000 )
            {
                p->timer->setInterval( 1000 );
                // qDebug( "void CdRecord::processOnOutput() : Timer Interval set to 1000 ms" );
            }

            checkProgressLine( proc_str );
        }
        else
        {
            if( p->timer->interval() != 25 )
            {
                p->timer->setInterval( 25 );
                // qDebug( "void CdRecord::processOnOutput() : Timer Interval set to 25 ms" );
            }

            checkItemicLog( proc_str );

            p->log_str = p->log_str + '\n' + proc_str;
            emit logChanged( p->log_str );
        }
    }

    if( isFinished() && list.isEmpty() )
    {
        p->timer->stop();
        p->clock->stop();
    }
}

void CdRecord::processOnError()
{
    return ;

    QString error_str = p->process->readAllStandardError();
    if( !error_str.isEmpty() )
        emit error( error_str );
}

void CdRecord::clockJobs()
{
    p->elapsed_time.addSecond( 1 );

    int remain_sec = 0;
    if( p->written_size_int !=0 )
        remain_sec = p->elapsed_time.toSecond()*(p->image_size_int-p->written_size_int)/p->written_size_int;

    p->remaining_time.reset();
    p->remaining_time.setSecond( remain_sec );

    emit elapsedTimeChanged(   p->elapsed_time              );
    emit remainingTimeChanged( p->remaining_time            );
    emit elapsedTimeChanged(   p->elapsed_time.toString()   );
    emit remainingTimeChanged( p->remaining_time.toString() );
}

void CdRecord::stopTimer()
{
    emit itemicLogAdded( CdRecord::Information , "Finished" );
    emit finished( p->process->exitCode() );

    dropProcess();
}

void CdRecord::checkItemicLog( const QString & str )
{
    if( str.left(7) == "wodim: " )
        emit itemicLogAdded( CdRecord::Information , str.right( str.size()-7 ) );

    else if( str.left(10) == "cdrecord: " )
        emit itemicLogAdded( CdRecord::Information , str.right( str.size()-10 ) );

    else if( str.left(9) == "WARNING: " )
        emit itemicLogAdded( CdRecord::Warning , str.right( str.size()-9 ) );

    else if( str.left(6) == "HINT: " )
        emit itemicLogAdded( CdRecord::Information , str.right( str.size()-6 ) );

    else if( str == "Fixating..." )
        emit itemicLogAdded( CdRecord::Information , str );
}

void CdRecord::checkProgressLine( const QString & str )
{
    QRegExp reg;
    int pos;
    bool ok;

    reg.setPattern( "(\\d+)(?:\\s*)of\\s+\\d+\\s+(KB|MB|GB)" );
    pos = reg.indexIn( str );
    if( pos > -1 )
        p->written_size_int = reg.cap(1).toInt(&ok);

    reg.setPattern( "(\\d+)(?:\\s*)(KB|MB|GB)" );
    pos = reg.indexIn( str );
    if( pos > -1 )
        p->image_size_int = reg.cap(1).toInt(&ok);

    reg.setPattern( "fifo\\s+(\\d+)(?:\\s*)%" );
    pos = reg.indexIn( str );
    if( pos > -1 )
        p->ring_buffer_percent_int = reg.cap(1).toInt(&ok);

    reg.setPattern( "buf\\s+(\\d+)(?:\\s*)%" );
    pos = reg.indexIn( str );
    if( pos > -1 )
        p->buffer_percent_int = reg.cap(1).toInt(&ok);

    reg.setPattern( "(\\d+\\.\\d)(?:\\s*)x\\.$" );
    pos = reg.indexIn( str );
    if( pos > -1 )
        p->written_speed_int = reg.cap(1).toDouble(&ok);

    if( p->image_size_int > 0 )
        p->process_precent = 100 * p->written_size_int / p->image_size_int;

    emit ringBufferChanged( p->ring_buffer_percent_int );
    emit bufferChanged(     p->buffer_percent_int      );
    emit writeSizeChenged(  p->written_size_int        );
    emit writeSpeedChanged( p->written_speed_int       );
    emit percentChanged(    p->process_precent         );
}

void CdRecord::reset()
{
    if( isStarted() )
        return ;

    p->ring_buffer_percent_int = 0;
    p->buffer_percent_int = 0;
    p->written_size_int = 0;
    p->written_speed_int = 0;
    p->process_precent = 0;

    p->used_command_str.clear();
    p->log_str.clear();

    p->elapsed_time.reset();
    p->remaining_time.reset();
}

CdRecord::~CdRecord()
{
    if( !isFinished() )
        stopTimer();

    delete p;
}
