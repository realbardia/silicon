#include "sabstractdiscburner.h"
#include "sdeviceitem.h"


class SAbstractDiscBurnerPrivate
{
public:
    SAbstractDiscBurner::SourceType source_type;

    QString               src_image;
    SDeviceItem           src_device;
    SDeviceItem           dst_device;

    int burn_speed;
    int min_buffer;
    int ring_buffer;
    int pad_size;

    bool pad_stt;
    bool dummy_stt;
    bool iso_size_stt;
    bool public_read_stt;
    bool force_stt;
    bool session_at_one_stt;
    bool track_at_one_stt;
    bool multi_session_stt;
    bool eject_stt;
};

SAbstractDiscBurner::SAbstractDiscBurner(QObject *parent) :
    SAbstractProcess(parent)
{
    pp = new SAbstractDiscBurnerPrivate;

    pp->source_type = SAbstractDiscBurner::Unknown;

    pp->burn_speed  = 1;
    pp->min_buffer  = -1;
    pp->ring_buffer = -1;
    pp->pad_size    = -1;

    pp->pad_stt            = false;
    pp->dummy_stt          = false;
    pp->iso_size_stt       = false;
    pp->public_read_stt    = false;
    pp->force_stt          = false;
    pp->session_at_one_stt = false;
    pp->track_at_one_stt   = false;
    pp->multi_session_stt  = false;
    pp->eject_stt          = false;

}

void SAbstractDiscBurner::setDestinationDevice( const SDeviceItem & item )
{
    pp->dst_device = item;
}

void SAbstractDiscBurner::setSourceDevice( const SDeviceItem & item )
{
    pp->src_device = item;
    pp->source_type = SAbstractDiscBurner::DiscToDisc;
}

void SAbstractDiscBurner::setImage( const QString & file )
{
    pp->src_image = file;
    pp->source_type = SAbstractDiscBurner::ImageToDisc;
}

void SAbstractDiscBurner::setSpeed( int value )
{
    pp->burn_speed = value;
}

void SAbstractDiscBurner::setMinimumBuffer( int value )
{
    pp->min_buffer = value;
}

void SAbstractDiscBurner::setRingBuffer( int value )
{
    pp->ring_buffer = value;
}

void SAbstractDiscBurner::setPadSize( int value )
{
    pp->pad_size = value;
}

void SAbstractDiscBurner::setPad( bool stt )
{
    pp->pad_stt = stt;
}

void SAbstractDiscBurner::setDummy( bool stt )
{
    pp->dummy_stt = stt;
}

void SAbstractDiscBurner::setIsoSize( bool stt )
{
    pp->iso_size_stt = stt;
}

void SAbstractDiscBurner::setPublicRead( bool stt )
{
    pp->public_read_stt = stt;
}

void SAbstractDiscBurner::setForce( bool stt )
{
    pp->force_stt = stt;
}

void SAbstractDiscBurner::setSessionAtOnce( bool stt )
{
    pp->session_at_one_stt = stt;
}

void SAbstractDiscBurner::setTrackAtOnce( bool stt )
{
    pp->track_at_one_stt = stt;
}

void SAbstractDiscBurner::setMultiSession( bool stt )
{
    pp->multi_session_stt = stt;
}

void SAbstractDiscBurner::setEject( bool stt )
{
    pp->eject_stt = stt;
}

const SDeviceItem & SAbstractDiscBurner::destinationDevice() const
{
    return pp->dst_device;
}

const SDeviceItem & SAbstractDiscBurner::sourceDevice() const
{
    return pp->src_device;
}

const QString & SAbstractDiscBurner::image() const
{
    return pp->src_image;
}

int SAbstractDiscBurner::speed() const
{
    return pp->burn_speed;
}

int SAbstractDiscBurner::minimumBuffer() const
{
    return pp->min_buffer;
}

int SAbstractDiscBurner::ringBuffer() const
{
    return pp->ring_buffer;
}

int SAbstractDiscBurner::padSize() const
{
    return pp->pad_size;
}

int SAbstractDiscBurner::imageSize() const
{
    return 0;
}

bool SAbstractDiscBurner::pad() const
{
    return pp->pad_stt;
}

bool SAbstractDiscBurner::dummy() const
{
    return pp->dummy_stt;
}

bool SAbstractDiscBurner::isoSize() const
{
    return pp->iso_size_stt;
}

bool SAbstractDiscBurner::publicRead() const
{
    return pp->public_read_stt;
}

bool SAbstractDiscBurner::force() const
{
    return pp->force_stt;
}

bool SAbstractDiscBurner::sessionAtOnce() const
{
    return pp->session_at_one_stt;
}

bool SAbstractDiscBurner::trackAtOnce() const
{
    return pp->track_at_one_stt;
}

bool SAbstractDiscBurner::multiSession() const
{
    return pp->multi_session_stt;
}

bool SAbstractDiscBurner::eject() const
{
    return pp->eject_stt;
}

SAbstractDiscBurner::SourceType SAbstractDiscBurner::sourceType() const
{
    return pp->source_type;
}

SAbstractDiscBurner::~SAbstractDiscBurner()
{
    delete pp;
}
