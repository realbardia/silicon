#include "fuseiso.h"

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

class MItem
{
public:
    QString command;
    QStringList arguments;
    QString file;
};

class FUseIsoPrivate
{
public:

    QProcess *process;

    QString command;
    QString umount_command;
    QString mtab;

    QString current;
    QString error_str;

    QHash<QString,QString> hash;
    QQueue<MItem> queue;
};

FUseIso::FUseIso( QObject *parent )
    : SAbstractImageMounter( parent )
{
    p = new FUseIsoPrivate;
    p->process = new QProcess( this );

    p->command        = "fuseiso";
    p->umount_command = "fusermount";
    p->mtab           = QDir::homePath() + "/.mtab.fuseiso";

    read_mtab();
}

void FUseIso::read_mtab()
{
    // read mounts and mount points from $HOME/.mtab.fuseiso file

    p->hash.clear();

    QFile mtab_file( p->mtab );
        mtab_file.open( QFile::ReadOnly );

    QTextStream stream( &mtab_file );
    QStringList datas = stream.readAll().split("\n");

    QString extra_part( " fuseiso defaults 0 0" );
    for( int i=0 ; i<datas.count() ; i++ )
    {
        QString str = datas[i].left( datas[i].size() - extra_part.size() );
        if( str.isEmpty() )
            continue;

        QStringList record = str.split(" /");
        p->hash.insert( record.first().replace("\\040"," ") , "/" + record.last().replace("\\040"," ") );
    }

    mtab_file.close();
    // END read
}

bool FUseIso::isMount( const QString & file ) const
{
    return p->hash.contains(file);
}

QString FUseIso::mountPoint( const QString & file ) const
{
    return p->hash.value( file );
}

QString FUseIso::file( const QString & mountPoint ) const
{
    return p->hash.key( mountPoint );
}

QString FUseIso::usedCommand() const
{
    return p->command;
}

QString FUseIso::application() const
{
    return p->command;
}

QString FUseIso::errorStr() const
{
    return p->error_str;
}

QStringList FUseIso::mountedList() const
{
    return p->hash.keys();
}

void FUseIso::mount( const QString & file , const QString & mount_point )
{
    MItem item;
        item.command   =  p->command;
        item.file      =  file;
        item.arguments << "-p";
        item.arguments << file;
        item.arguments << mount_point;

    p->queue.enqueue( item );

    if( p->queue.count() == 1 )
        start( p->mtab );
}

void FUseIso::unmount( const QString & mount_point )
{
    MItem item;
        item.command   =  p->umount_command;
        item.file      =  file( mount_point );
        item.arguments << "-u";
        item.arguments << mount_point;

    p->queue.enqueue( item );

    if( p->queue.count() == 1 )
        start( p->mtab );
}

void FUseIso::reload()
{
    read_mtab();
}

void FUseIso::goEvent( SProcessEvent * )
{
    go_prev();
}

void FUseIso::go_prev()
{
    const MItem & item = p->queue.dequeue();

    if( item.command == p->command )
        connect( p->process , SIGNAL(finished(int,QProcess::ExitStatus)) , this , SLOT(mount_finished(int,QProcess::ExitStatus)) );
    else if( item.command == p->umount_command )
        connect( p->process , SIGNAL(finished(int,QProcess::ExitStatus)) , this , SLOT(umount_finished(int,QProcess::ExitStatus)) );

    p->current = item.file;
    p->process->start( item.command , item.arguments );
}

void FUseIso::stopEvent()
{
    p->process->terminate();
    dropProcess();
}

void FUseIso::mount_finished( int exitCode , QProcess::ExitStatus exitStatus )
{
    disconnect( p->process , SIGNAL(finished(int,QProcess::ExitStatus)) , this , SLOT(mount_finished(int,QProcess::ExitStatus)) );

    p->error_str = p->process->readAllStandardError();
    if( exitStatus == QProcess::CrashExit )
        p->error_str.append( "\nfuseiso crashed" );

    if( !p->error_str.isEmpty() )
        emit error( p->error_str );

    QTest::qWait( 137 );
    read_mtab();

    bool is_mount = isMount( p->current );

    emit mounted( is_mount );
    if( !p->queue.isEmpty() )
    {
        go_prev();
        return;
    }

    emit finished( exitCode );

    if( p->queue.isEmpty() )
        dropProcess();
}

void FUseIso::umount_finished( int exitCode , QProcess::ExitStatus exitStatus )
{
    disconnect( p->process , SIGNAL(finished(int,QProcess::ExitStatus)) , this , SLOT(umount_finished(int,QProcess::ExitStatus)) );

    p->error_str = p->process->readAllStandardError();
    if( exitStatus == QProcess::CrashExit )
        p->error_str.append( "\nfusermount crashed" );

    if( !p->error_str.isEmpty() )
        emit error( p->error_str );

    QTest::qWait( 137 );
    read_mtab();

    bool is_umount = !isMount( p->current );

    emit unmounted( is_umount );
    if( !p->queue.isEmpty() )
    {
        go_prev();
        return;
    }

    emit finished( exitCode );

    if( p->queue.isEmpty() )
        dropProcess();
}

FUseIso::~FUseIso()
{
    delete p;
}
