#include "sabstractdiscreader.h"

#include "sdeviceitem.h"

class SAbstractDiscReaderPrivate
{
public:
    SDeviceItem device;
    QString file;
    int start;
    int end;
};

SAbstractDiscReader::SAbstractDiscReader( QObject *parent ) :
    SAbstractProcess( parent )
{
    pp = new SAbstractDiscReaderPrivate;
    pp->start = 0;
    pp->end   = 0;
}


void SAbstractDiscReader::setDevice( const SDeviceItem & item )
{
    pp->device = item;
}

void SAbstractDiscReader::setPath( const QString & file )
{
    pp->file = file;
}

void SAbstractDiscReader::setSectors( int start , int end )
{
    if( start >= end )
        end = start + 1;

    pp->start = start;
    pp->end = end;
}

const SDeviceItem & SAbstractDiscReader::currentDevice() const
{
    return pp->device;
}

const QString & SAbstractDiscReader::file() const
{
    return pp->file;
}

int SAbstractDiscReader::startSector() const
{
    return pp->start;
}

int SAbstractDiscReader::endSector() const
{
    return pp->end;
}

SAbstractDiscReader::~SAbstractDiscReader()
{
    delete pp;
}
