#include "unmounter.h"
#include "manager.h"

#include <iostream>

#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QQueue>
#include <QTest>

class UnmounterPrivate
{
public:
    QProcess *process;
    QString command;
    QQueue<QString> queue;
};

Unmounter::Unmounter(QObject *parent) :
    QObject(parent)
{
    p = new UnmounterPrivate;
    p->process = new QProcess( this );
    p->command = "umount";

    connect( p->process , SIGNAL(finished(int,QProcess::ExitStatus)) , SLOT(finished(int,QProcess::ExitStatus)) , Qt::QueuedConnection );
}

void Unmounter::unmount( const QString & mount_point )
{
    p->queue.enqueue( mount_point );
    if( p->queue.count() == 1 )
        unmount_prev( mount_point );

    Manager::increase();
}

void Unmounter::unmount_prev( const QString & mount_point )
{
    QStringList arguments;
        arguments << mount_point;

    p->process->start( p->command , arguments );
}

void Unmounter::finished( int , QProcess::ExitStatus state )
{
    QString mount_point = p->queue.dequeue();
    if( !p->queue.isEmpty() )
        unmount_prev( p->queue.dequeue() );

    if( state == QProcess::CrashExit )
    {
        qDebug() << "The umount process has unexpectedly finished.";
        Manager::decrease();
        return;
    }

    QDir dir( mount_point );
        dir.rmdir( mount_point );

    Manager::decrease();
}

Unmounter::~Unmounter()
{
    delete p;
}
