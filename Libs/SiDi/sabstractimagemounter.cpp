#include "sabstractimagemounter.h"

class SAbstractImageMounterPrivate
{
public:
};

SAbstractImageMounter::SAbstractImageMounter(QObject *parent) :
    SAbstractProcess(parent)
{
    pp = new SAbstractImageMounterPrivate;
}

bool SAbstractImageMounter::isMount( const QString & ) const
{
    return false;
}

QString SAbstractImageMounter::mountPoint( const QString & ) const
{
    return QString();
}

QString SAbstractImageMounter::file( const QString & ) const
{
    return QString();
}

QStringList SAbstractImageMounter::mountedList() const
{
    return QStringList();
}

void SAbstractImageMounter::mount( const QString & , const QString & )
{

}

void SAbstractImageMounter::unmount( const QString & )
{

}

void SAbstractImageMounter::reload()
{

}

SAbstractImageMounter::~SAbstractImageMounter()
{
    delete pp;
}
