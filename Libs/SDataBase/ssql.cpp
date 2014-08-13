#include "ssql.h"
#include "sdatabase_prev.h"
#include "ssqlconnect.h"

#include <QQueue>
#include <QMutex>
#include <QFileInfo>
#include <QStringList>
#include <QDebug>

SSqlSignalControler *signal_controler = 0;
SThreadedSQLConnect *Thread           = 0;
QMutex mutex;

SSql *lock_obj = 0;
QString default_address;

// ============================================================================================
// ============================================================================================
// ==========================          SThreadedSQLConnect         ============================
// ============================================================================================
// ============================================================================================


class SThreadedSQLConnectPrivate
{
public:
    SSQLConnect *sql_connect;

    QQueue<QString>            fnc_queue;
    QQueue<SDiscInfo>          disc_queue;
    QQueue<SFileInfo>          file_queue;
    QQueue<QString>            disc_details_queue;
    QQueue<int>                file_details_queue;
    QQueue<STinyFileInfo>      tiny_file_queue;
    QQueue<QString>            search_keys_queue;
    QQueue<SFileInfo::FileCat> search_cats_queue;
    QQueue<QString>            remove_discs_queue;
};


SThreadedSQLConnect::SThreadedSQLConnect( QObject *parent )
    : QThread( parent )
{
    p = new SThreadedSQLConnectPrivate;
        p->sql_connect = 0;
}

void SThreadedSQLConnect::insertDisc( const SDiscInfo & disc )
{
    mutex.lock();
    p->fnc_queue.enqueue( "insertDisc" );
    p->disc_queue.enqueue( disc );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::insertFile( const SFileInfo & file )
{
    mutex.lock();
    p->fnc_queue.enqueue( "insertFile" );
    p->file_queue.enqueue( file );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::removeDisc( const QString & disc_id )
{
    mutex.lock();
    p->fnc_queue.enqueue( "removeDisc" );
    p->remove_discs_queue.enqueue( disc_id );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::discsID()
{
    mutex.lock();
    p->fnc_queue.prepend( "discsID" );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::discDetails( const QString & id , bool md5 )
{
    mutex.lock();
    if( md5 )
        p->fnc_queue.prepend( "discDetailsFromMd5" );
    else
        p->fnc_queue.prepend( "discDetails" );

    p->disc_details_queue.enqueue( id );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::fileDetails( int id )
{
    mutex.lock();
    p->fnc_queue.prepend( "fileDetails" );
    p->file_details_queue.prepend( id );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::filesOf( const STinyFileInfo & dir )
{
    mutex.lock();
    p->fnc_queue.prepend( "filesOf" );
    p->tiny_file_queue.prepend( dir );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::search( const QString & key , SFileInfo::FileCat cat = SFileInfo::Normal )
{
    mutex.lock();
    p->fnc_queue.prepend( "search" );
    p->search_keys_queue.prepend( key );
    p->search_cats_queue.prepend( cat );
    mutex.unlock();

    start();
}

void SThreadedSQLConnect::run()
{
    if( p->sql_connect == 0 )
        p->sql_connect = new SSQLConnect( default_address );

    while( !p->fnc_queue.isEmpty() )
    {
        mutex.lock();
        QString fnc = p->fnc_queue.dequeue();

        if( fnc == "insertDisc" )
        {
            const SDiscInfo & disc = p->disc_queue.dequeue();
            mutex.unlock();

            bool result = p->sql_connect->insertDisc( disc );
            emit this->discInserted( disc , result );
        }

        else if( fnc == "insertFile" )
        {
            const SFileInfo & file = p->file_queue.dequeue();
            mutex.unlock();

            bool result = p->sql_connect->insertFile( file );
            emit this->fileInserted( file , result );
        }

        else if( fnc == "removeDisc" )
        {
            const QString & disc_id = p->remove_discs_queue.dequeue();
            mutex.unlock();

            bool result = p->sql_connect->removeDisc( disc_id );
            emit this->discRemoved( disc_id , result );
        }

        else if( fnc == "discsID" )
        {
            mutex.unlock();

            QStringList result = p->sql_connect->discsID();
            emit this->discsIdDetected( result );
        }

        else if( fnc == "discDetails" )
        {
            const QString & id = p->disc_details_queue.dequeue();
            mutex.unlock();

            SDiscInfo result = p->sql_connect->discDetails( id );
            emit this->detailDetected( result );
        }

        else if( fnc == "discDetailsFromMd5" )
        {
            const QString & md5 = p->disc_details_queue.dequeue();
            mutex.unlock();

            SDiscInfo result = p->sql_connect->discDetailsFromMd5( md5 );
            emit this->detailDetected( result );
        }

        else if( fnc == "fileDetails" )
        {
            int id = p->file_details_queue.dequeue();
            mutex.unlock();

            SFileInfo result = p->sql_connect->fileDetails( id );
            emit this->detailDetected( result );
        }

        else if( fnc == "filesOf" )
        {
            const STinyFileInfo & dir = p->tiny_file_queue.dequeue();
            mutex.unlock();

            SFileInfoList result = p->sql_connect->filesOf( dir );
            emit this->detectedFilesOf( dir , result );
        }

        else if( fnc == "search" )
        {
            const QString &    key = p->search_keys_queue.dequeue();
            SFileInfo::FileCat cat = p->search_cats_queue.dequeue();
            mutex.unlock();

            SFileInfoList result = p->sql_connect->search( key , cat );
            emit this->founded( key , result );
        }
    }
}

SThreadedSQLConnect::~SThreadedSQLConnect()
{
    delete p->sql_connect;
    delete p;
}

// ============================================================================================
// ============================================================================================
// =======================            SSqlSignalControler            ==========================
// ============================================================================================
// ============================================================================================


class SSqlSignalControlerPrivate
{
public:
    QQueue<SSql *> queue;
};


SSqlSignalControler::SSqlSignalControler( QObject *parent )
    : QObject( parent )
{
    p = new SSqlSignalControlerPrivate;
}

void SSqlSignalControler::enqueueObject( SSql *obj , bool highPriority  )
{
    if( highPriority && !p->queue.isEmpty() )
        p->queue.insert( 1 , obj );
    else
        p->queue.enqueue( obj );
}

void SSqlSignalControler::destroyed( SSql *obj )
{
    for( int i=0 ; i<p->queue.count() ; i++ )
        if( p->queue.at(i) == obj )
            p->queue.replace( i , 0 );
}

void SSqlSignalControler::discInserted( const SDiscInfo & disc , bool result )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->discInserted( disc , result );
}

void SSqlSignalControler::fileInserted( const SFileInfo & file , bool result )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->fileInserted( file , result );
}

void SSqlSignalControler::discRemoved( const QString & disc_id , bool result )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->discRemoved( disc_id , result );
}

void SSqlSignalControler::discsIdDetected( const QStringList & list )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->discsIdDetected( list );
}

void SSqlSignalControler::detailDetected( const SDiscInfo & disc )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->detailDetected( disc );
}

void SSqlSignalControler::detailDetected( const SFileInfo & file )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->detailDetected( file );
}

void SSqlSignalControler::detectedFilesOf( const STinyFileInfo & dir , const SFileInfoList & files )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->detectedFilesOf( dir , files );
}

void SSqlSignalControler::founded( const QString & key , const SFileInfoList & list )
{
    SSql *sql = p->queue.dequeue();
    if( sql == 0 )
        return ;

    emit sql->founded( key , list );
}

SSqlSignalControler::~SSqlSignalControler()
{
    delete p;
}


// ============================================================================================
// ============================================================================================
// ======================                      SSql                    ========================
// ============================================================================================
// ============================================================================================

SSql::SSql(QObject *parent) :
    QObject(parent)
{
    if( Thread == 0 )
        Thread = new SThreadedSQLConnect();

    if( signal_controler == 0 )
    {
        signal_controler = new SSqlSignalControler();

        qRegisterMetaType<  SDiscInfo  >(  "SDiscInfo"  );
        qRegisterMetaType<  SFileInfo  >(  "SFileInfo"  );
        qRegisterMetaType<STinyFileInfo>("STinyFileInfo");
        qRegisterMetaType<SFileInfoList>("SFileInfoList");

        QObject::connect( Thread           , SIGNAL(discInserted(SDiscInfo,bool)) ,
                          signal_controler ,   SLOT(discInserted(SDiscInfo,bool)) , Qt::QueuedConnection );

        QObject::connect( Thread           , SIGNAL(fileInserted(SFileInfo,bool)) ,
                          signal_controler ,   SLOT(fileInserted(SFileInfo,bool)) , Qt::QueuedConnection );

        QObject::connect( Thread           , SIGNAL(discRemoved(QString,bool)) ,
                          signal_controler ,   SLOT(discRemoved(QString,bool)) , Qt::QueuedConnection );

        QObject::connect( Thread           , SIGNAL(discsIdDetected(QStringList)) ,
                          signal_controler ,   SLOT(discsIdDetected(QStringList)) , Qt::QueuedConnection );

        QObject::connect( Thread           , SIGNAL(detailDetected(SDiscInfo)) ,
                          signal_controler ,   SLOT(detailDetected(SDiscInfo)) , Qt::QueuedConnection );

        QObject::connect( Thread           , SIGNAL(detailDetected(SFileInfo)) ,
                          signal_controler ,   SLOT(detailDetected(SFileInfo)) , Qt::QueuedConnection );

        QObject::connect( Thread           , SIGNAL(detectedFilesOf(STinyFileInfo,SFileInfoList)) ,
                          signal_controler ,   SLOT(detectedFilesOf(STinyFileInfo,SFileInfoList)) , Qt::QueuedConnection );

        QObject::connect( Thread           , SIGNAL(founded(QString,SFileInfoList)) ,
                          signal_controler ,   SLOT(founded(QString,SFileInfoList)) , Qt::QueuedConnection );
    }
}

void SSql::lock()
{
    if( locked() && lock_obj != this )
    {
        qDebug() << "Error on SSql::unlock(): Can't unlock SSql, this object is not owner of lock.";
        return;
    }
    if( locked() )
        return;

    lock_obj = this;
}

void SSql::unlock()
{
    if( locked() && lock_obj != this )
    {
        qDebug() << "Error on SSql::unlock(): Can't unlock SSql, this object is not owner of lock.";
        return;
    }
    if( unlocked() )
        return;

    lock_obj = 0;
}

bool SSql::locked()
{
    return !unlocked();
}

bool SSql::unlocked()
{
    return lock_obj == 0;
}

void SSql::setDefaultSQLAddress( const QString & address )
{
    if( locked() && lock_obj != this )
    {
        qDebug() << "Error on SSql::unlock(): Can't unlock SSql, this object is not owner of lock.";
        return;
    }

    default_address = address;
}

const QString & SSql::SQLAddress() const
{
    return default_address;
}

void SSql::insertDisc( const SDiscInfo & disc )
{
    Thread->insertDisc( disc );
    signal_controler->enqueueObject( this );
}

void SSql::insertFile( const SFileInfo & file )
{
    Thread->insertFile( file );
    signal_controler->enqueueObject( this );
}

void SSql::removeDisc( const QString & disc_id )
{
    Thread->removeDisc( disc_id );
    signal_controler->enqueueObject( this );
}

void SSql::discsID()
{
    Thread->discsID();
    signal_controler->enqueueObject( this , true );
}

void SSql::discDetails( const QString & id , bool md5 )
{
    Thread->discDetails( id , md5 );
    signal_controler->enqueueObject( this , true );
}

void SSql::fileDetails( int id )
{
    Thread->fileDetails( id );
    signal_controler->enqueueObject( this , true );
}

void SSql::filesOf( const STinyFileInfo & dir )
{
    Thread->filesOf( dir );
    signal_controler->enqueueObject( this , true );
}

void SSql::search( const QString & key , SFileInfo::FileCat cat )
{
    Thread->search( key , cat );
    signal_controler->enqueueObject( this , true );
}

SSql::~SSql()
{
    if( lock_obj == this )
        lock_obj = 0;

    signal_controler->destroyed( this );
}
