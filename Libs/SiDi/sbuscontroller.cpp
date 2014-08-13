#include "sbuscontroller.h"

#include <QQueue>
#include <QString>
#include <QList>
#include <QHash>
#include <QDebug>

QHash<QString,QQueue<SBusController*>* > *queued_buses = 0;
QHash<QString,SBusController*>      *active_buses = 0;


class SBusControllerPrivate
{
public:
    bool pass_up_and_wait;
};

SBusController::SBusController(QObject *parent) :
    QObject(parent)
{
    p = new SBusControllerPrivate;
        p->pass_up_and_wait = false;

    if( queued_buses == 0 )
        queued_buses = new QHash<QString,QQueue<SBusController*>* >();
    if( active_buses == 0 )
        active_buses = new QHash<QString,SBusController*>();
}

bool SBusController::isBusy( SDeviceItem device )
{
    return SBusController::isBusy( device.deviceFeatures().device_block_str );
}

bool SBusController::isBusy( const QString & bus )
{
    const QStringList & keys = active_buses->keys();
    if( keys.contains(bus) )
        return true;

    if( !queued_buses->contains(bus) )
        return false;

    QQueue<SBusController*> *list = queued_buses->value(bus);
    for( int i=0 ; i<list->count() ; i++ )
        if( !list->at(i)->passUpAndWait() )
            return true;

    return false;
}

bool SBusController::isActived() const
{
    const QList<SBusController*> & list = active_buses->values();
    for( int i=0 ; i<list.count() ; i++ )
        if( list.at(i) == this )
            return true;

    return false;
}

bool SBusController::isDeactived() const
{
    return !isActived();
}

bool SBusController::getAccess( SDeviceItem device , const QVariant & data )
{
    return getAccess( device.deviceFeatures().device_block_str , data );
}

bool SBusController::getAccess( const QString & bus , const QVariant & data )
{
    if( isBusy(bus) || passUpAndWait() )
    {
        if( !queued_buses->contains(bus) || queued_buses->value(bus) == 0 )
            queued_buses->insert( bus , new QQueue<SBusController*>() );

        (*queued_buses)[ bus ]->enqueue( this );

        return false;
    }
    else
    {
        active_buses->insert( bus , this );
        emit go( data );
        return true;
    }
}

void SBusController::setPassUpAndWait( bool state )
{
    if( p->pass_up_and_wait == state )
        return;

    p->pass_up_and_wait = state;

    if( state )
    {
        /*! ----------------- Find Active Buses ---------------- */
        QStringList keys = active_buses->keys();
        for( int i=0 ; i<keys.count() ; i++ )
        {
            const QString & bus = keys.at(i);
            if( !active_buses->contains(bus) )
                continue;

            SBusController *controller = active_buses->value( bus );
            if( controller == this )
            {
                finish( bus );
                getAccess( bus );
            }
        }
    }
    else
    {
        /*! ----------------- Find Disabled Buses ---------------- */
        QStringList keys = queued_buses->keys();
        for( int i=0 ; i<keys.count() ; i++ )
        {
            const QString & bus = keys.at(i);
            if( active_buses->contains(bus) )
                continue;

            /*! ---------- Find First Deactived Processes to Active --------- */
            if( !queued_buses->contains(bus) )
                continue;

            QQueue<SBusController*> *queue = queued_buses->value(bus);
            if( queue == 0 )
                continue;

            for( int j=0 ; j<queue->count() ; j++ )
            {
                if( queue->at(j) != this )
                    continue;

                queue->removeAt( j );
                if( queue->isEmpty() )
                    delete queued_buses->take( bus );

                active_buses->insert( bus , this );

                emit go();
                break;
            }
        }
    }

}

bool SBusController::passUpAndWait() const
{
    return p->pass_up_and_wait;
}

void SBusController::finish( SDeviceItem device )
{
    finish( device.deviceFeatures().device_block_str );
}

void SBusController::finish( const QString & bus )
{
    if( bus.isEmpty() )
        return;

    SBusController *core = (*active_buses)[bus];

    if( core != this )
    {
        qCritical() << QString("SBusController::finish : This SBusController Object can't remove \"%1\" from SBusController.").arg(bus);
        return ;
    }

    active_buses->remove(bus);


    /*! ---------- Dequeue first not pass up process ------------ */
    SBusController *item = 0;

    if( !queued_buses->contains(bus) )
        return;

    QQueue<SBusController*> *queue = queued_buses->value(bus);
    if( queue == 0 )
        return;

    for( int i=0 ; i<queue->count() ; i++ )
    {
        if( !queue->at(i)->passUpAndWait() )
        {
            item = queue->takeAt( i );
            break;
        }
    }

    if( item == 0 )
        return;
    /*! --------------------------------------------------------- */


    if( queued_buses->value(bus)->isEmpty() )
        delete queued_buses->take( bus );

    active_buses->insert( bus , item );

    emit item->go();
}

void SBusController::finish()
{
    QStringList keys;

    keys = queued_buses->keys();
    for( int i=0 ; i<keys.count() ; i++ )
    {
        const QString & bus = keys.at(i);
        if( !queued_buses->contains(bus) )
            continue;

        QQueue<SBusController*> *queue = queued_buses->value(bus);
        if( queue == 0 )
            continue;

        for( int j=0 ; j<queue->count() ; j++ )
            if( queue->at(j) == this )
            {
                queue->removeAt( j );
                j--;
            }

        if( queued_buses->value(bus)->isEmpty() )
            delete queued_buses->take( bus );
    }

    keys = active_buses->keys();
    for( int i=0 ; i<keys.count() ; i++ )
    {
        const QString & bus = keys.at(i);
        if( !active_buses->contains(bus) )
            continue;

        if( active_buses->value(bus) == this )
            finish( bus );
    }
}

SBusController::~SBusController()
{
    finish();

    delete p;
}
