#include "sprocessevent.h"

#include <QHash>

class SProcessEventPrivate
{
public:
    SDeviceItem device;
    QString address;
    SProcessEvent::Type type;
    bool accept;
};

QHash<SProcessEvent *,SProcessEventPrivate *> hash;

SProcessEvent::SProcessEvent()
{
    SProcessEventPrivate *p = new SProcessEventPrivate;
    hash.insert( this , p );

    p->type   = SProcessEvent::Unknown;
    p->accept = true;
}

void SProcessEvent::setDevice( const SDeviceItem & device )
{
    SProcessEventPrivate *p = hash.value(this);

    p->device = device;
    p->type   = SProcessEvent::Device;
}

void SProcessEvent::setAddress( const QString & address )
{
    SProcessEventPrivate *p = hash.value(this);

    p->address = address;
    p->type    = SProcessEvent::Address;
}

const QString & SProcessEvent::address()
{
    return hash.value(this)->address;
}

const SDeviceItem & SProcessEvent::device()
{
    return hash.value(this)->device;
}

SProcessEvent::Type SProcessEvent::currentType()
{
    return hash.value(this)->type;
}

void SProcessEvent::accept()
{
    hash.value(this)->accept = true;
}

void SProcessEvent::reject()
{
    hash.value(this)->accept = false;
}

bool SProcessEvent::isAccepted()
{
    return hash.value(this)->accept;
}

bool SProcessEvent::isRejected()
{
    return !isAccepted();
}

SProcessEvent::~SProcessEvent()
{
    delete hash.value( this );
    hash.remove( this );
}
