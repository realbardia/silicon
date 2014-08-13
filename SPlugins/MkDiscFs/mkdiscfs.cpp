#include "mkdiscfs.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QProcess>
#include <QDebug>

#include <SDesktopFunctions>
#include <SDiscDetector>

class MkDiscFsPrivate
{
public:
    QProcess *process;
    QProcess *isofs_process;
    QProcess *size_process;
    QProcess *manual_size_process;

    QTimer *timer;
    QTimer *clock;

    QString log_str;
    QString used_command_str;

    SDiscDetector *detector;

    QString sectors_size_str;

    STime elapsed_time;
    STime remaining_time;

    int written_size_int;
    int image_size_int;
    int ring_buffer_percent_int;
    int buffer_percent_int;
    int written_speed_int;
    int process_precent;
};

MkDiscFs::MkDiscFs( QObject *parent )
    : SAbstractDataBurner( parent )
{
    p = new MkDiscFsPrivate;

    p->process = new QProcess( this );
        p->process->setProcessChannelMode( QProcess::MergedChannels );

    p->size_process  = new QProcess( this );
    p->isofs_process = new QProcess( this );
        p->isofs_process->setProcessChannelMode( QProcess::SeparateChannels );
        p->isofs_process->setStandardOutputProcess( p->process );

    p->manual_size_process = new QProcess( this );

    p->detector = new SDiscDetector( this );

    p->timer   = new QTimer( this );
    p->clock   = new QTimer( this );

    connect( p->timer               , SIGNAL(timeout())                 , SLOT(processOnOutput())  );
    connect( p->clock               , SIGNAL(timeout())                 , SLOT(clockJobs())        );
    connect( p->process             , SIGNAL(finished(int))             , SLOT(stopTimer())        );
    connect( p->process             , SIGNAL(readyReadStandardOutput()) , SLOT(processOnOutput())  );
    connect( p->process             , SIGNAL(readyReadStandardError())  , SLOT(processOnError())   );
    connect( p->size_process        , SIGNAL(finished(int))             , SLOT(step_2())           );
    connect( p->detector            , SIGNAL(accepted())                , SLOT(step_3())           );
    connect( p->detector            , SIGNAL(rejected())                , SLOT(stopTimer())        );
    connect( p->manual_size_process , SIGNAL(finished(int))             , SLOT(sizeDetected())     );

    reset();
}

QString MkDiscFs::logs() const
{
    return p->log_str;
}

QString MkDiscFs::usedCommand() const
{
    return p->used_command_str;
}

QString MkDiscFs::application() const
{
    return command();
}

STime MkDiscFs::elapsedTime() const
{
    return p->elapsed_time;
}

STime MkDiscFs::remainingTime() const
{
    return p->remaining_time;
}

int MkDiscFs::imageSize() const
{
    return p->image_size_int;
}

void MkDiscFs::goEvent( SProcessEvent *event )
{
    if( !event->device().isEmpty() )
        setDevice( event->device() );

    step_1();
}

void MkDiscFs::step_1()
{
    if( pathSpec().isEmpty() )
        return ;

    QStringList args;

    if( jolietDirState() )      args << "-J";
    if( rockRidgeState() )      args << "-R";
    if( !deepDirectoryState() ) args << "-D";

    args << "-print-size";
    args << "-quiet";
    args << "-graft-points";

    args << pathSpec();
    p->size_process->start( isofsCommand() , args );
}

void MkDiscFs::step_2()
{
    p->sectors_size_str = p->size_process->readAll();
        p->sectors_size_str = p->sectors_size_str.remove(" ").remove("\n").remove("\r");

    p->detector->setDestinationDisc( currentDevice() , p->sectors_size_str.toUInt()*2048 );
    p->detector->check();
}

void MkDiscFs::step_3()
{
    const QStringList & args    = arguments();
    const QStringList & ifsArgs = isofsArguments();



    /*! ---------------------- IsoFs Command --------------------- */
    p->used_command_str.clear();
    p->used_command_str = isofsCommand() + " ";
    for( int i=0 ; i<ifsArgs.count() ; i++ )
    {
        QString str = ifsArgs.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        p->used_command_str = p->used_command_str + str + " ";

        if( ifsArgs.at(i) == "-graft-points" )
        {
            p->used_command_str = p->used_command_str + " [path specs]";
            break;
        }
    }
    /*! ---------------------------------------------------------- */

    p->used_command_str = p->used_command_str + " | ";

    /*! -------------------- CdRecord Command -------------------- */
    p->used_command_str = p->used_command_str + command() + " ";
    for( int i=0 ; i<args.count() ; i++ )
    {
        QString str = args.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        p->used_command_str = p->used_command_str + str + " ";
    }
    /*! ---------------------------------------------------------- */

    p->log_str = p->used_command_str;
    emit itemicLogAdded( MkDiscFs::Information , p->used_command_str );



    p->isofs_process->start( isofsCommand() , ifsArgs );
    p->process->start( command() , args );

    p->timer->start( 25 );
    p->clock->start( 1000 );
}

QStringList MkDiscFs::arguments() const
{
    QStringList args;

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

    args << "dev=" + currentDevice().toQString();
    args << "tsize=" + p->sectors_size_str + "s" << "-";

    return args;
}

QStringList MkDiscFs::isofsArguments() const
{
    QStringList args;

    if( jolietDirState()           )        args << "-J";
    if( rockRidgeState()           )        args << "-R";
    if( !deepDirectoryState()      )        args << "-D";
    if( allowLeadingDotsState()    )        args << "-allow-leading-dots";
    if( allowLowercaseState()      )        args << "-allow-lowercase";
    if( quietState()               )        args << "-quiet";
    if( !copyright().isEmpty()     )        args << "-copyright" << copyright() ;
    if( !abstract().isEmpty()      )        args << "-abstract"  << abstract() ;
    if( !publisher().isEmpty()     )        args << "-publisher" << publisher() ;
    if( !systemID().isEmpty()      )        args << "-sysid"     << systemID() ;
    if( !applicationID().isEmpty() )        args << "-A"         << applicationID() ;
    if( !biblioGraphic().isEmpty() )        args << "-biblio"    << biblioGraphic() ;
    if( !volume().isEmpty()        )        args << "-V"         << volume() ;

    args << "-graft-points";
    args << pathSpec();

    return args;
}

QString MkDiscFs::command() const
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

QString MkDiscFs::isofsCommand() const
{
    QString app;

    QStringList list_of_programs;
        list_of_programs << "mkisofs" << "genisoimage";

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

void MkDiscFs::stopEvent()
{
    p->process->kill();
}

void MkDiscFs::processOnOutput()
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
                // qDebug( "void MkDiscFs::processOnOutput() : Timer Interval set to 1000 ms" );
            }

            checkProgressLine( proc_str );
        }
        else
        {
            if( p->timer->interval() != 25 )
            {
                p->timer->setInterval( 25 );
                // qDebug( "void MkDiscFs::processOnOutput() : Timer Interval set to 25 ms" );
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

void MkDiscFs::processOnError()
{
    return ;

    QString error_str = p->process->readAllStandardError();
    if( !error_str.isEmpty() )
        emit error( error_str );
}

void MkDiscFs::clockJobs()
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

void MkDiscFs::stopTimer()
{
    emit itemicLogAdded( MkDiscFs::Information , "Finished" );
    emit finished( p->process->exitCode() );

    dropProcess();
}

void MkDiscFs::checkItemicLog( const QString & str )
{
    if( str.left(7) == "wodim: " )
        emit itemicLogAdded( MkDiscFs::Information , str.right( str.size()-7 ) );

    else if( str.left(10) == "cdrecord: " )
        emit itemicLogAdded( MkDiscFs::Information , str.right( str.size()-10 ) );

    else if( str.left(9) == "WARNING: " )
        emit itemicLogAdded( MkDiscFs::Warning , str.right( str.size()-9 ) );

    else if( str.left(6) == "HINT: " )
        emit itemicLogAdded( MkDiscFs::Information , str.right( str.size()-6 ) );

    else if( str == "Fixating..." )
        emit itemicLogAdded( MkDiscFs::Information , str );
}

void MkDiscFs::checkProgressLine( const QString & str )
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

void MkDiscFs::reset()
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

void MkDiscFs::pathSpecChanged( const QStringList & list )
{
    if( list.isEmpty() )
    {
        p->image_size_int = 0;
        return;
    }

    QStringList args;

    if( jolietDirState() )      args << "-J";
    if( rockRidgeState() )      args << "-R";
    if( !deepDirectoryState() ) args << "-D";

    args << "-print-size";
    args << "-quiet";
    args << "-graft-points";

    args << list;

    p->manual_size_process->start( isofsCommand() , args );
}

void MkDiscFs::sizeDetected()
{
    p->sectors_size_str = p->manual_size_process->readAll();
        p->sectors_size_str = p->sectors_size_str.remove(" ").remove("\n").remove("\r");

    p->image_size_int = p->sectors_size_str.toInt();
    emit imageSizeDetected( p->image_size_int );
}

MkDiscFs::~MkDiscFs()
{
    if( !isFinished() )
        stopTimer();

    delete p;
}
