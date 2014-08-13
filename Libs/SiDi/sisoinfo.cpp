#include "sisoinfo.h"

#include <QDir>
#include <QRegExp>
#include <QCoreApplication>
#include <QProcess>

#include <limits.h>

class SIsoInfoPrivate
{
public:
    enum type {
        DEVICE
       ,FILE
    };

    int current_type;

    QString command;

    QString application_id_str;
    QString format_str;
    QString system_str;
    QString volume_str;
    QString publisher_str;
    QString data_preparer_str;
    QString copyright_str;
    QString abstract_str;
    int logical_block_size_int;
    int volume_size_int;

    QString file_path_str;
    SDeviceItem device_path_str;

    QString all_informations_str;

    QProcess *process;
};



SIsoInfo::SIsoInfo( QObject *parent )
    : QObject( parent )
{
    p = new SIsoInfoPrivate;

    p->process = new QProcess( this );

#ifdef Q_OS_WIN32
    QDir dir( QCoreApplication::applicationFilePath() );
        dir.cdUp();

    p->command = dir.path() + "/cdrtools/isoinfo.exe";
    p->command.replace( "/" , "\\" );
#else
    p->command = "isoinfo";
#endif

    QObject::connect( p->process , SIGNAL(finished(int)) , this , SLOT(readInfos_prev()) );
}

void SIsoInfo::setFile( const QString & file )
{
    p->current_type = SIsoInfoPrivate::FILE;
    this->p->file_path_str = file;
    this->readInfos();
}

void SIsoInfo::setDevice( const SDeviceItem & disc )
{
    p->current_type = SIsoInfoPrivate::DEVICE;
    p->device_path_str = disc;
    this->readInfos();
}

const QString & SIsoInfo::applicationID() const
{
    return this->p->application_id_str;
}

const QString & SIsoInfo::format() const
{
    return this->p->format_str;
}

const QString & SIsoInfo::system() const
{
    return this->p->system_str;
}

const QString & SIsoInfo::volume() const
{
    return this->p->volume_str;
}

const QString & SIsoInfo::publisher() const
{
    return this->p->publisher_str;
}

const QString & SIsoInfo::dataPreparer() const
{
    return this->p->data_preparer_str;
}

const QString & SIsoInfo::copyright() const
{
    return this->p->copyright_str;
}

const QString & SIsoInfo::abstract() const
{
    return this->p->abstract_str;
}

int SIsoInfo::logicalBlockSize() const
{
    return this->p->logical_block_size_int;
}

int SIsoInfo::volumeSize() const
{
    return this->p->volume_size_int;
}

const QString & SIsoInfo::allInformations() const
{
    return this->p->all_informations_str;
}

void SIsoInfo::readInfos()
{
    QString path;
    QStringList args;

    switch( p->current_type )
    {
    case SIsoInfoPrivate::DEVICE :
        path = "dev=" + p->device_path_str.toQString();
        break;
    case SIsoInfoPrivate::FILE :
        path = p->file_path_str;
        args << "-i";
        break;
    }

    args << path << "-d";

    delete p->process;
    p->process = new QProcess( this );
    QObject::connect( p->process , SIGNAL(finished(int)) , this , SLOT(readInfos_prev()) );

    p->process->start( p->command , args );
}

void SIsoInfo::readInfos_prev()
{
    p->all_informations_str = p->process->readAll();

    bool ok;
    this->p->application_id_str     = this->findItem("Application id");
    this->p->system_str             = this->findItem("System id");
    this->p->volume_str             = this->findItem("Volume id");
    this->p->publisher_str          = this->findItem("Publisher id");
    this->p->data_preparer_str      = this->findItem("Data preparer id");
    this->p->copyright_str          = this->findItem("Copyright File id");
    this->p->abstract_str           = this->findItem("Abstract File id");
    this->p->logical_block_size_int = this->findItem("Logical block size is").toInt(&ok);
    this->p->volume_size_int        = this->findItem("Volume size is").toInt(&ok);

    emit this->applicationIdReaded( p->application_id_str );
    emit this->systemUpdated( p->system_str );
    emit this->volumeUpdated( p->volume_str );
    emit this->publisherUpdated( p->publisher_str );
    emit this->dataPreparerUpdated( p->data_preparer_str );
    emit this->copyrightUpdated( p->copyright_str );
    emit this->abstractUpdated( p->abstract_str );
    emit this->logicalBlockSizeUpdated( p->logical_block_size_int );
    emit this->volumeSizeUpdated( p->volume_size_int );
    emit this->infosReaded( p->all_informations_str );
}

QString SIsoInfo::findItem( QString str )
{
    str = str + ": ";
    QStringList list = p->all_informations_str.split( '\n' );

    int str_size = str.size();
    int count = list.count();

    for( int i=0 ; i<count ; i++ )
    {
        QString on_process = list.at(i);

        if( on_process.left(str_size) == str )
            return on_process.right( on_process.size() - str_size );
    }
    return NULL;
}

SIsoInfo::~SIsoInfo()
{
    delete p;
}
