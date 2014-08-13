#include "sabstractdisceraser.h"

class SAbstractDiscEraserPrivate
{
public:
    bool force_stt;
    bool lead_out_stt;

    SDeviceItem current_device_item;
};

SAbstractDiscEraser::SAbstractDiscEraser( QObject *parent )
    : SAbstractProcess( parent )
{
    pp = new SAbstractDiscEraserPrivate;
    pp->force_stt    = false;
    pp->lead_out_stt = false;
}

void SAbstractDiscEraser::setDevice( const SDeviceItem & item )
{
    pp->current_device_item = item;
}

void SAbstractDiscEraser::setForce( bool stt )
{
    pp->force_stt = stt;
}

void SAbstractDiscEraser::setLeadOut( bool stt )
{
    pp->lead_out_stt = stt;
}

const SDeviceItem & SAbstractDiscEraser::device() const
{
    return pp->current_device_item;
}

bool SAbstractDiscEraser::force() const
{
    return pp->force_stt;
}

bool SAbstractDiscEraser::leadOut() const
{
    return pp->lead_out_stt;
}

SAbstractDiscEraser::~SAbstractDiscEraser()
{
    delete pp;
}
