#include "sabstractaudioburner.h"

class SAbstractAudioBurnerPrivate
{
public:
    bool swab_stt;

    QStringList files;

    QString copyright_file;
    QString abstract_file;
    QString publisher_id;
    QString sys_id;
    QString application_id;
    QString biblio_graphic_file;
    QString volume_id;

    SDeviceItem dst_device;

    int burn_speed;
    int min_buffer;
    int ring_buffer;
    int pad_size;

    bool pad_stt;
    bool dummy_stt;
    bool public_read_stt;
    bool force_stt;
    bool session_at_one_stt;
    bool track_at_one_stt;
    bool multi_session_stt;
    bool eject_stt;
};

SAbstractAudioBurner::SAbstractAudioBurner( QObject *parent ) :
    SAbstractProcess( parent )
{
    pp = new SAbstractAudioBurnerPrivate;

    pp->burn_speed  = -1;
    pp->min_buffer  = -1;
    pp->ring_buffer = -1;
    pp->pad_size    = -1;

    pp->swab_stt            = true;
    pp->pad_stt             = true;
    pp->dummy_stt           = false;
    pp->public_read_stt     = false;
    pp->force_stt           = false;
    pp->multi_session_stt   = false;
    pp->session_at_one_stt  = false;
    pp->track_at_one_stt    = true;
    pp->eject_stt           = false;
}

void SAbstractAudioBurner::setFiles( const QStringList & files )
{
    pp->files.clear();
    addFiles( files );
}

void SAbstractAudioBurner::addFiles( const QStringList & files )
{
    pp->files << files;
}

void SAbstractAudioBurner::setSwab( bool stt )
{
    pp->swab_stt = stt;
}

void SAbstractAudioBurner::setCopyRight( const QString & copyright )
{
    pp->copyright_file = copyright;
}

void SAbstractAudioBurner::setAbstract( const QString & abstract )
{
    pp->abstract_file = abstract;
}

void SAbstractAudioBurner::setPublisher( const QString & publisher )
{
    pp->publisher_id = publisher;
}

void SAbstractAudioBurner::setSystemID( const QString & sys )
{
    pp->sys_id = sys;
}

void SAbstractAudioBurner::setApplicationID( const QString & application )
{
    pp->application_id = application;
}

void SAbstractAudioBurner::setBiblioGraphic( const QString & biblio_graphic )
{
    pp->biblio_graphic_file = biblio_graphic;
}

void SAbstractAudioBurner::setVolumeID( const QString & volume )
{
    pp->volume_id = volume;
}

void SAbstractAudioBurner::setDevice( const SDeviceItem & item )
{
    pp->dst_device = item;
}

void SAbstractAudioBurner::setSpeed( int value )
{
    pp->burn_speed = value;
}

void SAbstractAudioBurner::setMinimumBuffer( int value )
{
    pp->min_buffer = value;
}

void SAbstractAudioBurner::setRingBuffer( int value )
{
    pp->ring_buffer = value;
}

void SAbstractAudioBurner::setPadSize( int value )
{
    pp->pad_size = value;
}

void SAbstractAudioBurner::setPad( bool stt )
{
    pp->pad_stt = stt;
}

void SAbstractAudioBurner::setDummy( bool stt )
{
    pp->dummy_stt = stt;
}

void SAbstractAudioBurner::setPublicRead( bool stt )
{
    pp->public_read_stt = stt;
}

void SAbstractAudioBurner::setForce( bool stt )
{
    pp->force_stt = stt;
}

void SAbstractAudioBurner::setSessionAtOnce( bool stt )
{
    pp->session_at_one_stt = stt;
}

void SAbstractAudioBurner::setTrackAtOnce( bool stt )
{
    pp->track_at_one_stt = stt;
}

void SAbstractAudioBurner::setMultiSession( bool stt )
{
    pp->multi_session_stt = stt;
}

void SAbstractAudioBurner::setEject( bool stt )
{
    pp->eject_stt = stt;
}

const QStringList & SAbstractAudioBurner::files() const
{
    return pp->files;
}

bool SAbstractAudioBurner::swab() const
{
    return pp->swab_stt;
}

const QString & SAbstractAudioBurner::copyright() const
{
    return pp->copyright_file;
}

const QString & SAbstractAudioBurner::abstract() const
{
    return pp->abstract_file;
}

const QString & SAbstractAudioBurner::publisher() const
{
    return pp->publisher_id;
}

const QString & SAbstractAudioBurner::systemID() const
{
    return pp->sys_id;
}

const QString & SAbstractAudioBurner::applicationID() const
{
    return pp->application_id;
}

const QString & SAbstractAudioBurner::biblioGraphic() const
{
    return pp->biblio_graphic_file;
}

const QString & SAbstractAudioBurner::volume() const
{
    return pp->volume_id;
}

const SDeviceItem & SAbstractAudioBurner::currentDevice() const
{
    return pp->dst_device;
}

int SAbstractAudioBurner::speed() const
{
    return pp->burn_speed;
}

int SAbstractAudioBurner::minimumBuffer() const
{
    return pp->min_buffer;
}

int SAbstractAudioBurner::ringBuffer() const
{
    return pp->ring_buffer;
}

int SAbstractAudioBurner::padSize() const
{
    return pp->pad_size;
}

int SAbstractAudioBurner::imageSize() const
{
    return 0;
}

bool SAbstractAudioBurner::pad() const
{
    return pp->pad_stt;
}

bool SAbstractAudioBurner::dummy() const
{
    return pp->dummy_stt;
}

bool SAbstractAudioBurner::publicRead() const
{
    return pp->public_read_stt;
}

bool SAbstractAudioBurner::force() const
{
    return pp->force_stt;
}

bool SAbstractAudioBurner::sessionAtOnce() const
{
    return pp->session_at_one_stt;
}

bool SAbstractAudioBurner::trackAtOnce() const
{
    return pp->track_at_one_stt;
}

bool SAbstractAudioBurner::multiSession() const
{
    return pp->multi_session_stt;
}

bool SAbstractAudioBurner::eject() const
{
    return pp->eject_stt;
}

SAbstractAudioBurner::~SAbstractAudioBurner()
{
    delete pp;
}
