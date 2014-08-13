#include "sabstractconverter.h"

class SAbstractConverterPrivate
{
public:
    QString source;
    QString destination;
};

SAbstractConverter::SAbstractConverter( QObject *parent ) :
    SAbstractProcess( parent )
{
    pp = new SAbstractConverterPrivate;
}

void SAbstractConverter::setSource( const QString & source )
{
    pp->source = source;
}

const QString & SAbstractConverter::source() const
{
    return pp->source;
}

void SAbstractConverter::setDestination( const QString & destination )
{
    pp->destination = destination;
}

const QString & SAbstractConverter::destination() const
{
    return pp->destination;
}

SAbstractConverter::~SAbstractConverter()
{
    delete pp;
}
