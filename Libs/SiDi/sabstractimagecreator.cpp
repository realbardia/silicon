#include "sabstractimagecreator.h"

class SAbstractImageCreatorPrivate
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

    QString output_str;

    QString copyright_file;
    QString abstract_file;
    QString publisher_id;
    QString sys_id;
    QString application_id;
    QString biblio_graphic_file;
    QString volume_id;
};


SAbstractImageCreator::SAbstractImageCreator( QObject *parent ) :
    SAbstractProcess( parent )
{
    pp = new SAbstractImageCreatorPrivate;

    pp->size_detector_stt      = false;
    pp->joliet_dir_stt         = true;
    pp->rock_ridge_stt         = true;
    pp->graph_point_stt        = true;
    pp->quiet_stt              = false;
    pp->allow_leading_dots_stt = false;
    pp->deep_directory_stt     = true;
    pp->allow_lowercase_stt    = false;
}
void SAbstractImageCreator::setPathSpec( const QStringList & path )
{
    pp->path_spec.clear();
    addPathSpec( path );
}

void SAbstractImageCreator::addPathSpec( const QStringList & path )
{
    pp->path_spec << path;
    pathSpecChanged( pp->path_spec );
}

void SAbstractImageCreator::setOutPut( const QString & file )
{
    pp->output_str = file;
}

void SAbstractImageCreator::setJolietDirectory( bool stt )
{
    pp->joliet_dir_stt = stt;
}

void SAbstractImageCreator::setRockRidge( bool stt )
{
    pp->rock_ridge_stt = stt;
}

void SAbstractImageCreator::setGraphtPoint( bool stt )
{
    pp->graph_point_stt = stt;
}

void SAbstractImageCreator::setDeepDirectory( bool stt )
{
    pp->deep_directory_stt = stt;
}

void SAbstractImageCreator::setAllowLeadingDots( bool stt )
{
    pp->allow_leading_dots_stt = stt;
}

void SAbstractImageCreator::setAllowLowercase( bool stt )
{
    pp->allow_lowercase_stt = stt;
}

void SAbstractImageCreator::setCopyRight( const QString & copyright )
{
    pp->copyright_file = copyright;
}

void SAbstractImageCreator::setAbstract( const QString & abstract )
{
    pp->abstract_file = abstract;
}

void SAbstractImageCreator::setPublisher( const QString & publisher )
{
    pp->publisher_id = publisher;
}

void SAbstractImageCreator::setSystemID( const QString & sys )
{
    pp->sys_id = sys;
}

void SAbstractImageCreator::setApplicationID( const QString & application )
{
    pp->application_id = application;
}

void SAbstractImageCreator::setBiblioGraphic( const QString & biblio_graphic )
{
    pp->biblio_graphic_file = biblio_graphic;
}

void SAbstractImageCreator::setVolumeID( const QString & volume )
{
    pp->volume_id = volume;
}

bool SAbstractImageCreator::jolietDirState() const
{
    return pp->joliet_dir_stt;
}

bool SAbstractImageCreator::rockRidgeState() const
{
    return pp->rock_ridge_stt;
}

bool SAbstractImageCreator::graphPointState() const
{
    return pp->graph_point_stt;
}

bool SAbstractImageCreator::quietState() const
{
    return pp->quiet_stt;
}

bool SAbstractImageCreator::sizeDetectorState() const
{
    return pp->size_detector_stt;
}

bool SAbstractImageCreator::deepDirectoryState() const
{
    return pp->deep_directory_stt;
}

bool SAbstractImageCreator::allowLeadingDotsState() const
{
    return pp->allow_leading_dots_stt;
}

bool SAbstractImageCreator::allowLowercaseState() const
{
    return pp->allow_lowercase_stt;
}

const QStringList & SAbstractImageCreator::pathSpec() const
{
    return pp->path_spec;
}

const QString & SAbstractImageCreator::outPut() const
{
    return pp->output_str;
}

const QString & SAbstractImageCreator::copyright() const
{
    return pp->copyright_file;
}

const QString & SAbstractImageCreator::abstract() const
{
    return pp->abstract_file;
}

const QString & SAbstractImageCreator::publisher() const
{
    return pp->publisher_id;
}

const QString & SAbstractImageCreator::systemID() const
{
    return pp->sys_id;
}

const QString & SAbstractImageCreator::applicationID() const
{
    return pp->application_id;
}

const QString & SAbstractImageCreator::biblioGraphic() const
{
    return pp->biblio_graphic_file;
}

const QString & SAbstractImageCreator::volume() const
{
    return pp->volume_id;
}

int SAbstractImageCreator::imageSize() const
{
    return 0;
}

void SAbstractImageCreator::pathSpecChanged( const QStringList & )
{

}

SAbstractImageCreator::~SAbstractImageCreator()
{
    delete pp;
}
