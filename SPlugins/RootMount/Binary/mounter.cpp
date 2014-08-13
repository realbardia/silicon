#include "mounter.h"
#include "manager.h"

#include <iostream>

#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QQueue>

class MounterItem
{
public:
    MounterItem( const QString & f , const QString & m )
    { file = f ; mount_point = m; }

    QString file;
    QString mount_point;
};

class MounterPrivate
{
public:
    QProcess *process;
    QString command;

    QQueue<MounterItem> queue;
};

Mounter::Mounter(QObject *parent) :
    QObject(parent)
{
    p = new MounterPrivate;
    p->process = new QProcess( this );
    p->command = "mount";

    connect( p->process , SIGNAL(finished(int,QProcess::ExitStatus)) , SLOT(finished(int,QProcess::ExitStatus)) , Qt::QueuedConnection );
}

void Mounter::mount( const QString & file , const QString & mount_point )
{
    p->queue.enqueue( MounterItem( file , mount_point ) );
    if( p->queue.count() == 1 )
        mount_prev( file , mount_point );

    Manager::increase();
}

void Mounter::mount_prev( const QString & file , const QString & mount_point )
{
    QDir dir( mount_point );
        dir.mkpath( mount_point );

    QStringList arguments;
        arguments << "-o";
        arguments << "loop";
        arguments << file;
        arguments << mount_point;

    p->process->start( p->command , arguments );
}

void Mounter::finished( int exit_code , QProcess::ExitStatus state )
{
    p->queue.dequeue();
    if( !p->queue.isEmpty() )
    {
        const MounterItem & item = p->queue.dequeue();
        mount_prev( item.file , item.mount_point );
    }

    if( state == QProcess::CrashExit )
    {
        qDebug() << "The mount process has unexpectedly finished.";
        Manager::decrease();
        return;
    }

    if( exit_code == 0  )
    {
        Manager::decrease();
        return;
    }

    if( exit_code &  1  )    qDebug() << "Incorrect invocation or permissions.";
    if( exit_code &  2  )    qDebug() << "System error (out of memory, cannot fork, no more loop devices).";
    if( exit_code &  4  )    qDebug() << "Internal mount bug.";
    if( exit_code &  8  )    qDebug() << "User interrupt.";
    if( exit_code &  16 )    qDebug() << "Problems writing or locking /etc/mtab .";
    if( exit_code &  32 )    qDebug() << "Mount failure.";
    if( exit_code &  64 )    qDebug() << "Some mount succeeded.";

    Manager::decrease();
}

Mounter::~Mounter()
{
    delete p;
}
