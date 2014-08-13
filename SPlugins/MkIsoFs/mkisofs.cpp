#include "mkisofs.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QProcess>
#include <QDebug>

#include <SDesktopFunctions>

class MkIsoFsPrivate
{
public:
    QProcess *process;
    QProcess *manual_size_process;
    QProcess *size_process;

    QTimer *timer;
    QTimer *clock;

    QString log_str;
    QString used_command_str;

    QString sectors_size_str;

    STime elapsed_time;
    STime remaining_time;

    int created_size_int;
    int disc_usage_percent;
    int image_size_int;
    int process_precent;
};

MkIsoFs::MkIsoFs( QObject *parent )
    : SAbstractImageCreator( parent )
{
    p = new MkIsoFsPrivate;

    p->process = new QProcess( this );
        p->process->setProcessChannelMode( QProcess::MergedChannels );

    p->size_process  = new QProcess( this );
    p->manual_size_process = new QProcess( this );

    p->timer   = new QTimer( this );
    p->clock   = new QTimer( this );

    connect( p->timer               , SIGNAL(timeout())                 , SLOT(processOnOutput())  );
    connect( p->clock               , SIGNAL(timeout())                 , SLOT(clockJobs())        );
    connect( p->process             , SIGNAL(finished(int))             , SLOT(stopTimer())        );
    connect( p->process             , SIGNAL(readyReadStandardOutput()) , SLOT(processOnOutput())  );
    connect( p->process             , SIGNAL(readyReadStandardError())  , SLOT(processOnError())   );
    connect( p->size_process        , SIGNAL(finished(int))             , SLOT(step_2())           );
    connect( p->manual_size_process , SIGNAL(finished(int))             , SLOT(sizeDetected())     );

    reset();
}

QString MkIsoFs::logs() const
{
    return p->log_str;
}

QString MkIsoFs::usedCommand() const
{
    return p->used_command_str;
}

QString MkIsoFs::application() const
{
    return command();
}

STime MkIsoFs::elapsedTime() const
{
    return p->elapsed_time;
}

STime MkIsoFs::remainingTime() const
{
    return p->remaining_time;
}

int MkIsoFs::imageSize() const
{
    return p->image_size_int;
}

void MkIsoFs::goEvent( SProcessEvent *event )
{
    if( !event->address().isEmpty() )
        setOutPut( event->address() );

    step_1();
}

void MkIsoFs::step_1()
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
    p->size_process->start( command() , args );
}

void MkIsoFs::step_2()
{
    p->sectors_size_str = p->size_process->readAll();
        p->sectors_size_str = p->sectors_size_str.remove(" ").remove("\n").remove("\r");

    p->image_size_int = p->sectors_size_str.toInt();

    const QStringList & args    = arguments();


    /*! -------------------- CdRecord Command -------------------- */
    p->used_command_str = command() + " ";
    for( int i=0 ; i<args.count() ; i++ )
    {
        QString str = args.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        p->used_command_str = p->used_command_str + str + " ";

        if( args.at(i) == "-graft-points" )
        {
            p->used_command_str = p->used_command_str + " [path specs]";
            break;
        }
    }
    /*! ---------------------------------------------------------- */

    p->log_str = p->used_command_str;
    emit itemicLogAdded( MkIsoFs::Information , p->used_command_str );

    p->process->start( command() , args );

    p->timer->start( 25 );
    p->clock->start( 1000 );
}

QStringList MkIsoFs::arguments() const
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

    args << "-o" << outPut();
    args << "-graft-points";
    args << pathSpec();

    return args;
}

QString MkIsoFs::command() const
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

void MkIsoFs::stopEvent()
{
    p->process->kill();
}

void MkIsoFs::processOnOutput()
{
    QStringList list = QString( p->process->readLine() ).split("\n",QString::SkipEmptyParts);

    for( int i=0 ; i<list.count() ; i++ )
    {
        QString proc_str = list.at(i);
        QString tmp = proc_str.simplified();

        if( tmp.contains( "% done," ) )
        {
            if( p->timer->interval() != 1000 )
            {
                p->timer->setInterval( 1000 );
                // qDebug( "void MkIsoFs::processOnOutput() : Timer Interval set to 1000 ms" );
            }

            this->checkProgressLine( proc_str );
        }
        else if( !tmp.isEmpty() && tmp.left(5) != "Using" )
        {
            if( p->timer->interval() != 25 )
            {
                p->timer->setInterval( 25 );
                // qDebug( "void MkIsoFs::processOnOutput() : Timer Interval set to 25 ms" );
            }

            p->log_str = p->log_str + '\n' + proc_str;
            emit this->logChanged( p->log_str );
        }
    }

    if( isFinished() && list.isEmpty() )
    {
        p->timer->stop();
        p->clock->stop();
    }
}

void MkIsoFs::processOnError()
{
    return ;

    QString error_str = p->process->readAllStandardError();
    if( !error_str.isEmpty() )
        emit error( error_str );
}

void MkIsoFs::clockJobs()
{
    p->elapsed_time.addSecond( 1 );

    int remain_sec = 0;
    if( p->created_size_int != 0 )
        remain_sec = p->elapsed_time.toSecond()*(p->image_size_int-p->created_size_int)/p->created_size_int;

    p->remaining_time.reset();
    p->remaining_time.setSecond( remain_sec );

    emit this->elapsedTimeChanged( p->elapsed_time );
    emit this->remainingTimeChanged( p->remaining_time );
    emit this->elapsedTimeChanged( p->elapsed_time.toString() );
    emit this->remainingTimeChanged( p->remaining_time.toString() );
}

void MkIsoFs::stopTimer()
{
    p->process_precent = 100;
    p->process->setProcessChannelMode( QProcess::MergedChannels);
    emit percentChanged( (int)p->process_precent );

    emit itemicLogAdded( MkIsoFs::Information , "Finished" );
    emit finished( p->process->exitCode() );

    dropProcess();
}

void MkIsoFs::checkItemicLog( const QString & str )
{
    if( str.left(9) == "mkisofs: " )
        emit this->itemicLogAdded( MkIsoFs::Information , str.right( str.size()-9 ) );

    else if( str.left(13) == "genisoimage: " )
        emit this->itemicLogAdded( MkIsoFs::Information , str.right( str.size()-13 ) );

    else if( str.left(3) == "I: " )
        emit this->itemicLogAdded( MkIsoFs::Information , str.right( str.size()-3 ) );

    else if( str.left(9) == "WARNING: " )
        emit this->itemicLogAdded( MkIsoFs::Warning , str.right( str.size()-9 ) );

    else if( str.left(6) == "HINT: " )
        emit this->itemicLogAdded( MkIsoFs::Information , str.right( str.size()-6 ) );

    else if( str == "Fixating..." )
        emit this->itemicLogAdded( MkIsoFs::Information , str );
}

void MkIsoFs::checkProgressLine( const QString & str )
{
    if( isFinished() )
        return;

    QRegExp reg;
    int pos;
    bool ok;

    reg.setPattern( ".*(\\d+\\.\\d+)\\%\\s*(done).*" );
    pos = reg.indexIn( str );
    if( pos > -1 )
        p->process_precent = reg.cap(1).toDouble( &ok );

    p->created_size_int = p->image_size_int * p->process_precent / 100;

    emit percentChanged( (int)p->process_precent );
    emit createdSizeChanged( p->created_size_int );
}

void MkIsoFs::reset()
{
    if( isStarted() )
        return ;

    p->elapsed_time.reset();
    p->remaining_time.reset();

    p->created_size_int = 0;
    p->image_size_int = 0;
    p->process_precent = 0;
    p->disc_usage_percent = 0;
}

void MkIsoFs::pathSpecChanged( const QStringList & list )
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
    p->manual_size_process->start( command() , args );
}

void MkIsoFs::sizeDetected()
{
    p->sectors_size_str = p->manual_size_process->readAll();
        p->sectors_size_str = p->sectors_size_str.remove(" ").remove("\n").remove("\r");

    p->image_size_int = p->sectors_size_str.toInt();
    emit imageSizeDetected( p->image_size_int );
}

MkIsoFs::~MkIsoFs()
{
    if( !isFinished() )
        stopTimer();

    delete p;
}
