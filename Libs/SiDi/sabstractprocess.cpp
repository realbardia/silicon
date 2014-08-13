#include "sabstractprocess.h"
#include "sbuscontroller.h"

#include <QHash>
#include <QTimer>
#include <QDebug>

class SAbstractProcessPrivate
{
public:
    SBusController *controller;
    SProcessEvent  *event;
    bool started;
};

SAbstractProcess::SAbstractProcess(QObject *parent) :
    QObject(parent)
{
    ppp = new SAbstractProcessPrivate;

    ppp->controller  = new SBusController( this );
    ppp->event = 0;

    connect( ppp->controller , SIGNAL(go()) , SLOT(access_granted()) );
}

QString SAbstractProcess::logs() const
{
    return QString();
}

QString SAbstractProcess::usedCommand() const
{
    return QString();
}

QString SAbstractProcess::errorStr() const
{
    return QString();
}

bool SAbstractProcess::isFinished() const
{
    return ppp->controller->isDeactived();
}

bool SAbstractProcess::isStarted() const
{
    return ppp->controller->isActived();
}

STime SAbstractProcess::elapsedTime() const
{
    return STime();
}

STime SAbstractProcess::remainingTime() const
{
    return STime();
}

QString SAbstractProcess::application() const
{
    return QString();
}


void SAbstractProcess::setPassupAndWait( bool stt )
{
    ppp->controller->setPassUpAndWait( stt );
}

bool SAbstractProcess::passupAndWait() const
{
    return ppp->controller->passUpAndWait();
}

void SAbstractProcess::stop()
{
    emit stopSignal();
    stopEvent();
}

void SAbstractProcess::access_granted()
{
    go_prev();
    /*
    qDebug( "SAbstractProcess::access_granted() : 2713ms until starting Process ..." );
    QTimer::singleShot( 2713 , this , SLOT(go_prev()) );*/
}

void SAbstractProcess::go_prev()
{
    if( ppp->event == 0 )
        return ;

    emit goEventSignal( ppp->event );
    if( ppp->event->isRejected() )
    {
        stop();
        return;
    }

    qDebug() << this << "Process Started";
    goEvent( ppp->event );
}

void SAbstractProcess::start( const SDeviceItem & device )
{
    if( ppp->event != 0 )
        return ;

    startEvent();

    SProcessEvent *event = new SProcessEvent();
        event->setDevice( device );

    ppp->event = event;
    ppp->controller->getAccess( device );
}

void SAbstractProcess::start( const QString & address )
{
    if( ppp->event != 0 )
        return ;

    startEvent();
    SProcessEvent *event = new SProcessEvent();
        event->setAddress( address );

    ppp->event = event;
    ppp->controller->getAccess( address );
}

void SAbstractProcess::goEvent( SProcessEvent * )
{
    stop();
}

void SAbstractProcess::startEvent()
{
}

void SAbstractProcess::stopEvent()
{
}

void SAbstractProcess::dropProcess()
{
    drop_prev();
    /*
    qDebug( "SAbstractProcess::dropProcess() : 2713ms until starting Process ..." );
    QTimer::singleShot( 2713 , this , SLOT(drop_prev()) );*/
}

void SAbstractProcess::drop_prev()
{
    if( ppp->event == 0 )
        return ;

    switch( static_cast<int>(ppp->event->currentType()) )
    {
    case SProcessEvent::Address :
        ppp->controller->finish( ppp->event->address() );
        break;
    case SProcessEvent::Device :
        ppp->controller->finish( ppp->event->device() );
        break;
    }

    delete ppp->event;
    ppp->event = 0;
}

SAbstractProcess::~SAbstractProcess()
{
    if( ppp->event != 0 )
    {
        delete ppp->event;
        ppp->event = 0;
    }

    delete ppp;
}
