#include "sabstractconverterengine.h"

QHash<QString,QVariant> SAbstractConverterEngine::optionsOf( const QString & , const QString & ) const
{
    return QHash<QString,QVariant>();
}

void SAbstractConverterEngine::setDefaultOption( const QString & , const QString & , const QHash<QString,QVariant> & )
{

}
