#include "sscandisc.h"

#include <QCoreApplication>
#include <QProcess>
#include <QQueue>
#include <QDebug>
#include <QDir>

#include <SSql>
#include <SDiscInfo>
#include <SFileInfo>

#include "sdevicelist.h"
#include "sisoinfo.h"
#include "sabstractimagemounter.h"
#include "siditools.h"
#include "sidi_prev.h"


class SScanDiscPrivate
{
public:
    QProcess              *process_1;
    QProcess              *process_2;
    SSql                  *sql;
    SIsoInfo              *iso;
    SAbstractImageMounter *mounter;
    SDeviceList           *device_list;

    QString md5_temp;

    QQueue<SDeviceItem>      queue;
    QHash<QString,SDiscInfo> hash;

    SScanDiscThread *thread;
};

SScanDisc::SScanDisc(QObject *parent) :
    QObject(parent)
{
    p = new SScanDiscPrivate;
    p->process_1   = new QProcess( this );
    p->process_2   = new QProcess( this );
    p->sql         = new SSql( this );
    p->iso         = new SIsoInfo( this );
    p->mounter     = SiDiTools::createImageMounter( this );
    p->device_list = new SDeviceList( this );
    p->thread      = new SScanDiscThread( this );

    connect( this , SIGNAL(log(QString)) , SLOT(debug(QString)) );
    connect( p->thread , SIGNAL(fileScaned(SFileInfo)) , p->sql , SLOT(insertFile(SFileInfo)) , Qt::QueuedConnection );
}

void SScanDisc::scan( const SDeviceItem & device )
{
    p->queue.enqueue( p->device_list->deviceList().first() );
    if( p->queue.count() != 1 )
        return ;

    step_start();
}

void SScanDisc::debug( const QString & str )
{
    qDebug() << str;
}

void SScanDisc::step_start()
{
    if( p->queue.isEmpty() )
        return ;

    const SDeviceItem & device = p->queue.first();
    if( p->process_1->isOpen() )
        return ;

    emit log( tr("Step 0: Checking md5sum of the disc.") );

    QObject::connect( p->process_2 , SIGNAL(finished(int)) , SLOT(step_0_done()) , Qt::QueuedConnection );

    p->process_1->setStandardOutputProcess( p->process_2 );
    p->process_1->start( "dd" , QStringList()<< "if=" + device.deviceFeatures().device_block_str
                                             << "bs=1"
                                             << "count=1000000" );
    p->process_2->start( "md5sum" );
}

void SScanDisc::step_0_done()
{
    QString md5sum = p->process_2->readAll();
    QObject::disconnect( p->process_2 , SIGNAL(finished(int)) , this , SLOT(step_0_done()) );

    if( md5sum.isEmpty() )
    {
        finished_unsuccessfully( 0 );
        emit log( tr("Can't detect md5sum of the disc. Aborted.") );
        return ;
    }

    emit log( tr("Step 1: Checking Disc state on DataBase.") );

    QObject::connect( p->sql , SIGNAL(detailDetected(SDiscInfo)) , this , SLOT(step_1_done(SDiscInfo)) );

    p->sql->discDetails( md5sum , true );
    p->md5_temp = md5sum;
}

void SScanDisc::step_1_done( const SDiscInfo & disc )
{
    QObject::disconnect( p->sql , SIGNAL(detailDetected(SDiscInfo)) , this , SLOT(step_1_done(SDiscInfo)) );

    if( !disc.discId().isEmpty() )
    {
        finished_unsuccessfully( 1 );
        emit log( tr("This Disc added to DataBase before. Aborted") );
        return ;
    }

    emit log( tr("Step 2: Read Disc Details.") );

    const SDeviceItem & device = p->queue.first();

    //p->device_list->unmount( device );
    p->hash.insert( device.toQString() , disc );

    QObject::connect( p->iso , SIGNAL(infosReaded(QString)) , this , SLOT(step_2_done(QString)) );
    p->iso->setDevice( p->queue.first() );
}

void SScanDisc::step_2_done( const QString & str )
{
    QObject::disconnect( p->iso , SIGNAL(infosReaded(QString)) , this , SLOT(step_2_done(QString)) );

    if( str.isEmpty() )
    {
        finished_unsuccessfully( 2 );
        emit log( tr("Can't detect iso details. Aborted") );
        return ;
    }

    emit log( tr("Step 3: Inserting disc item to database...") );

    SDiscInfo disc;
        disc.setDiscId( "Ubuntu 7.10" /* change later*/    );
        disc.setApplicationId( p->iso->applicationID() );
        disc.setCopyRight(     p->iso->copyright()     );
        disc.setLabel(         p->iso->volume()        );
        disc.setPublisher(     p->iso->publisher()     );
        disc.setMd5Sum(        p->md5_temp             );
        disc.setSize(          p->iso->volumeSize()    );

    QObject::connect( p->sql , SIGNAL(discInserted(SDiscInfo,bool)) , this , SLOT(step_3_done(SDiscInfo,bool)) );
    p->sql->insertDisc( disc );
}

void SScanDisc::step_3_done( const SDiscInfo & disc , bool stt )
{
    QObject::disconnect( p->sql , SIGNAL(discInserted(SDiscInfo,bool)) , this , SLOT(step_3_done(SDiscInfo,bool)) );
    if( !stt )
    {
        finished_unsuccessfully( 3 );
        emit log( tr("Unknown database error. can't insert disc to database. Aborted") );
        return ;
    }

    emit log( tr("Step 4: Mounting disc to temp directory.") );

    const SDeviceItem & device = p->queue.first();
    p->hash.insert( device.toQString() , disc );

    if( !device.currentDiscFeatures().mount_point_str.isEmpty() )
    {
        step_4_done( device );
        return ;
    }

    QObject::connect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , this , SLOT(step_4_done(SDeviceItem)) );
    p->device_list->mount( device );
}

void SScanDisc::step_4_done( const SDeviceItem & arg )
{
    const SDeviceItem & device = p->queue.first();
    if( device.toQString() != arg.toQString() )
        return ;

    QObject::disconnect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , this , SLOT(step_4_done(SDeviceItem)) );
    if( arg.currentDiscFeatures().mount_point_str.isEmpty() )
    {
        finished_unsuccessfully( 4 );
        emit log( tr("Can't mount Device. Aborted") );
        return ;
    }

    emit log( tr("Step 5: Detecting files and folders map on disc.") );


    QDir folder(arg.currentDiscFeatures().mount_point_str);
    QString dirName = folder.dirName();
/*
    p->graph = new SFolderGraph();
        p->graph->addFolder( folder );
        p->graph->cd( folder.dirName() );


    const QMap<QString,QString> & tmp_map = p->graph->mapOfCurrent();
    QMapIterator<QString, QString> i(tmp_map);

    delete p->graph;

    QMap<QString,QString> map;
    while( i.hasNext() )
    {
        i.next();
        map.insert( QString(i.key()).remove(0,dirName.size()+1) , i.value() );
    }

    step_5_done( map );*/
}

void SScanDisc::step_5_done( const QMap<QString,QString> & map )
{
    const SDeviceItem & device = p->queue.first();
    const SDiscInfo   & disc   = p->hash.value( device.toQString() );

    if( map.isEmpty() )
    {
        finished_unsuccessfully( 5 );
        emit log( tr("Can't Detect file or folders maps. Aborted") );
        return ;
    }

    emit log( tr("Step 6: Detecting files details...") );

    QMapIterator<QString, QString> i(map);
    while( i.hasNext() )
    {
        i.next();
        p->thread->scan( disc.discId() , i.value() , i.key() );
    }

    emit log( tr("Done.") );

    emit this->finished( true );
    step_start();
}

void SScanDisc::finished_unsuccessfully( int on_step )
{
    switch( on_step )
    {
    case 5:
        p->device_list->unmount( p->queue.first() );
    case 4:
        p->sql->removeDisc( p->hash.value( p->queue.first().toQString() ).discId() );
        p->hash.remove( p->queue.first().toQString() );
    case 3:
    case 2:
    case 1:
    case 0:
        p->queue.dequeue();
        break;
    }

    emit this->log( tr("Recovery finished.") );
    emit this->finished( false );
    step_start();
}

SScanDisc::~SScanDisc()
{
    delete p;
}
