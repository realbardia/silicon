#include "sabstractdataburner.h"

class SAbstractDataBurnerPrivate
{
public:
    bool joliet_dir_stt;
    bool rock_ridge_stt;
    bool graph_point_stt;
    bool quiet_stt;
    bool size_detector_stt;
    bool deep_directory_stt;
    bool allow_leading_dots_stt;
    bool allow_lowercase_stt;

    QStringList path_spec;
    QString copyright_file;
    QString abstract_file;
    QString publisher_id;
    QString sys_id;
    QString application_id;
    QString biblio_graphic_file;
    QString volume_id;

    SDeviceItem           dst_device;

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

SAbstractDataBurner::SAbstractDataBurner( QObject *parent )
    : SAbstractProcess( parent )
{
    pp = new SAbstractDataBurnerPrivate;

    pp->size_detector_stt      = false;
    pp->joliet_dir_stt         = true;
    pp->rock_ridge_stt         = true;
    pp->graph_point_stt        = true;
    pp->quiet_stt              = false;
    pp->allow_leading_dots_stt = false;
    pp->deep_directory_stt     = true;
    pp->allow_lowercase_stt    = false;

    pp->burn_speed  = -1;
    pp->min_buffer  = -1;
    pp->ring_buffer = -1;
    pp->pad_size    = -1;

    pp->pad_stt             = false;
    pp->dummy_stt           = false;
    pp->public_read_stt     = false;
    pp->force_stt           = false;
    pp->multi_session_stt   = false;
    pp->session_at_one_stt  = false;
    pp->track_at_one_stt    = false;
    pp->eject_stt           = false;
}

void SAbstractDataBurner::setPathSpec( const QStringList & path )
{
    pp->path_spec.clear();
    addPathSpec( path );
}

void SAbstractDataBurner::addPathSpec( const QStringList & path )
{
    pp->path_spec << path;
    pathSpecChanged( pp->path_spec );
}

void SAbstractDataBurner::setJolietDirectory( bool stt )
{
    pp->joliet_dir_stt = stt;
}

void SAbstractDataBurner::setRockRidge( bool stt )
{
    pp->rock_ridge_stt = stt;
}

void SAbstractDataBurner::setGraphtPoint( bool stt )
{
    pp->graph_point_stt = stt;
}

void SAbstractDataBurner::setDeepDirectory( bool stt )
{
    pp->deep_directory_stt = stt;
}

void SAbstractDataBurner::setAllowLeadingDots( bool stt )
{
    pp->allow_leading_dots_stt = stt;
}

void SAbstractDataBurner::setAllowLowercase( bool stt )
{
    pp->allow_lowercase_stt = stt;
}

void SAbstractDataBurner::setCopyRight( const QString & copyright )
{
    pp->copyright_file = copyright;
}

void SAbstractDataBurner::setAbstract( const QString & abstract )
{
    pp->abstract_file = abstract;
}

void SAbstractDataBurner::setPublisher( const QString & publisher )
{
    pp->publisher_id = publisher;
}

void SAbstractDataBurner::setSystemID( const QString & sys )
{
    pp->sys_id = sys;
}

void SAbstractDataBurner::setApplicationID( const QString & application )
{
    pp->application_id = application;
}

void SAbstractDataBurner::setBiblioGraphic( const QString & biblio_graphic )
{
    pp->biblio_graphic_file = biblio_graphic;
}

void SAbstractDataBurner::setVolumeID( const QString & volume )
{
    pp->volume_id = volume;
}

void SAbstractDataBurner::setDevice( const SDeviceItem & item )
{
    pp->dst_device = item;
}

void SAbstractDataBurner::setSpeed( int value )
{
    pp->burn_speed = value;
}

void SAbstractDataBurner::setMinimumBuffer( int value )
{
    pp->min_buffer = value;
}

void SAbstractDataBurner::setRingBuffer( int value )
{
    pp->ring_buffer = value;
}

void SAbstractDataBurner::setPadSize( int value )
{
    pp->pad_size = value;
}

void SAbstractDataBurner::setPad( bool stt )
{
    pp->pad_stt = stt;
}

void SAbstractDataBurner::setDummy( bool stt )
{
    pp->dummy_stt = stt;
}

void SAbstractDataBurner::setPublicRead( bool stt )
{
    pp->public_read_stt = stt;
}

void SAbstractDataBurner::setForce( bool stt )
{
    pp->force_stt = stt;
}

void SAbstractDataBurner::setSessionAtOnce( bool stt )
{
    pp->session_at_one_stt = stt;
}

void SAbstractDataBurner::setTrackAtOnce( bool stt )
{
    pp->track_at_one_stt = stt;
}

void SAbstractDataBurner::setMultiSession( bool stt )
{
    pp->multi_session_stt = stt;
}

void SAbstractDataBurner::setEject( bool stt )
{
    pp->eject_stt = stt;
}

bool SAbstractDataBurner::jolietDirState() const
{
    return pp->joliet_dir_stt;
}

bool SAbstractDataBurner::rockRidgeState() const
{
    return pp->rock_ridge_stt;
}

bool SAbstractDataBurner::graphPointState() const
{
    return pp->graph_point_stt;
}

bool SAbstractDataBurner::quietState() const
{
    return pp->quiet_stt;
}

bool SAbstractDataBurner::sizeDetectorState() const
{
    return pp->size_detector_stt;
}

bool SAbstractDataBurner::deepDirectoryState() const
{
    return pp->deep_directory_stt;
}

bool SAbstractDataBurner::allowLeadingDotsState() const
{
    return pp->allow_leading_dots_stt;
}

bool SAbstractDataBurner::allowLowercaseState() const
{
    return pp->allow_lowercase_stt;
}

const QStringList & SAbstractDataBurner::pathSpec() const
{
    return pp->path_spec;
}

const QString & SAbstractDataBurner::copyright() const
{
    return pp->copyright_file;
}

const QString & SAbstractDataBurner::abstract() const
{
    return pp->abstract_file;
}

const QString & SAbstractDataBurner::publisher() const
{
    return pp->publisher_id;
}

const QString & SAbstractDataBurner::systemID() const
{
    return pp->sys_id;
}

const QString & SAbstractDataBurner::applicationID() const
{
    return pp->application_id;
}

const QString & SAbstractDataBurner::biblioGraphic() const
{
    return pp->biblio_graphic_file;
}

const QString & SAbstractDataBurner::volume() const
{
    return pp->volume_id;
}

const SDeviceItem & SAbstractDataBurner::currentDevice() const
{
    return pp->dst_device;
}

int SAbstractDataBurner::speed() const
{
    return pp->burn_speed;
}

int SAbstractDataBurner::minimumBuffer() const
{
    return pp->min_buffer;
}

int SAbstractDataBurner::ringBuffer() const
{
    return pp->ring_buffer;
}

int SAbstractDataBurner::padSize() const
{
    return pp->pad_size;
}

int SAbstractDataBurner::imageSize() const
{
    return 0;
}

bool SAbstractDataBurner::pad() const
{
    return pp->pad_stt;
}

bool SAbstractDataBurner::dummy() const
{
    return pp->dummy_stt;
}

bool SAbstractDataBurner::publicRead() const
{
    return pp->public_read_stt;
}

bool SAbstractDataBurner::force() const
{
    return pp->force_stt;
}

bool SAbstractDataBurner::sessionAtOnce() const
{
    return pp->session_at_one_stt;
}

bool SAbstractDataBurner::trackAtOnce() const
{
    return pp->track_at_one_stt;
}

bool SAbstractDataBurner::multiSession() const
{
    return pp->multi_session_stt;
}

bool SAbstractDataBurner::eject() const
{
    return pp->eject_stt;
}

void SAbstractDataBurner::pathSpecChanged( const QStringList & )
{

}

SAbstractDataBurner::~SAbstractDataBurner()
{
    delete pp;
}
