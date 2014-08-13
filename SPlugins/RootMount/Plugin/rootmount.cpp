#include "rootmount.h"

#include <STime>
#include <SDeviceItem>
#include <SDesktopFunctions>
#include <SDiscDetector>

#include <QDir>
#include <QProcess>
#include <QList>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QTest>
#include <QTextStream>
#include <QQueue>
#include <QDebug>
#include <QRegExp>
#include <QApplication>

class MItem
{
public:
    QStringList arguments;
};

class RootMountPrivate
{
public:
    QProcess *process;

    QString command;
    QString mount;
    QString umount;
    QString mtab;
    QString sudo;

    QString error_str;

    QHash<QString,QString> hash;

    QQueue<MItem> queue;

    QQueue<QString> mounted;
    QQueue<QString> mounted_error;
    QQueue<QString> unmounted;
    QQueue<QString> unmounted_error;
};

RootMount::RootMount( QObject *parent )
    : SAbstractImageMounter( parent )
{
    p = new RootMountPrivate;
    p->process = new QProcess( this );

    p->command = QApplication::applicationDirPath() + "/silicon_rootmount";
    p->mount   = "--mount";
    p->umount  = "--unmount";
    p->mtab    = "/etc/mtab";
    p->sudo    = "pkexec";

    connect( p->process , SIGNAL(finished(int,QProcess::ExitStatus)) , SLOT(finish(int,QProcess::ExitStatus)) );

    read_mtab();
}

void RootMount::read_mtab()
{
    p->hash.clear();

    QProcess process;
    process.start( "mount" );
        process.waitForFinished();

    QStringList output = QString(process.readAll()).split("\n");

    QRegExp reg;
        reg.setPattern( "^(/.*)\\son\\s(/.*)\\stype\\siso\\S*\\s\\(ro\\)" );

    output = output.filter( reg );

    for( int i=0 ; i<output.count() ; i++ )
    {
        QString str = output.at(i);
        int pos;

        pos = reg.indexIn( str );
        if( pos == -1 )
            continue;

        p->hash.insert( reg.cap(1) , reg.cap(2) );
    }
}

bool RootMount::isMount( const QString & file ) const
{
    return p->hash.contains(file);
}

QString RootMount::mountPoint( const QString & file ) const
{
    return p->hash.value( file );
}

QString RootMount::file( const QString & mountPoint ) const
{
    return p->hash.key( mountPoint );
}

QString RootMount::usedCommand() const
{
    return p->command;
}

QString RootMount::application() const
{
    return p->command;
}

QString RootMount::errorStr() const
{
    return p->error_str;
}

QStringList RootMount::mountedList() const
{
    return p->hash.keys();
}

void RootMount::mount( const QString & file , const QString & mount_point )
{
    MItem item;
        item.arguments << p->mount;
        item.arguments << file;
        item.arguments << mount_point;

    p->queue.enqueue( item );
    p->mounted.enqueue( file );

    if( p->queue.count() == 1 )
        QTimer::singleShot( 237 , this , SLOT(start_prev()) );
}

void RootMount::unmount( const QString & mount_point )
{
    MItem item;
        item.arguments << p->umount;
        item.arguments << mount_point;

    p->queue.enqueue( item );
    p->unmounted.enqueue( mount_point );

    if( p->queue.count() == 1 )
        QTimer::singleShot( 237 , this , SLOT(start_prev()) );
}

void RootMount::start_prev()
{
    start( p->mtab );
}

void RootMount::reload()
{
    read_mtab();
}

void RootMount::goEvent( SProcessEvent * )
{
    go_prev();
}

void RootMount::go_prev()
{
    QStringList arguments;
        arguments << p->command;

    while( !p->queue.isEmpty() )
        arguments << p->queue.dequeue().arguments;

    QString used_command_str = p->sudo + " ";
    for( int i=0 ; i<arguments.count() ; i++ )
    {
        QString str = arguments.at(i);
        if( str.contains(" ") )
            str = "\"" + str + "\"";

        used_command_str = used_command_str + str + " ";
    }

    p->process->start( p->sudo , arguments );
}

void RootMount::stopEvent()
{
    p->process->terminate();
    dropProcess();
}

void RootMount::finish( int exitCode , QProcess::ExitStatus exitStatus )
{
    p->error_str = p->process->readAllStandardError();
    if( exitStatus == QProcess::CrashExit )
        p->error_str.append( "\nrootmount crashed" );

    QTest::qWait( 137 );
    read_mtab();

    while( !p->mounted.isEmpty() )
    {
        const QString & file = p->mounted.dequeue();

        if( isMount( file ) )
        {
            emit mounted( true );
        }
        else
        {
            emit mounted( false );
            p->error_str.append( "\n" + tr("Can't mount %1").arg(file) );
        }
    }

    while( !p->unmounted.isEmpty() )
    {
        const QString & file = p->unmounted.dequeue();

        if( !isMount( file ) )
        {
            emit unmounted( true );
        }
        else
        {
            emit unmounted( false );
            p->error_str.append( "\n" + tr("Can't unmount %1").arg(file) );
        }
    }

    emit finished( exitCode );

    if( !p->error_str.isEmpty() )
        emit error( p->error_str );

    if( p->queue.isEmpty() )
        dropProcess();
}

RootMount::~RootMount()
{
    delete p;
}
