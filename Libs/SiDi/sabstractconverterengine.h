#ifndef SABSTRACTCONVERTERENGINE_H
#define SABSTRACTCONVERTERENGINE_H

#include "sabstractprocessengine.h"
#include "SiDi_global.h"

#include <QHash>
#include <QVariant>

class SIDI_EXPORT SAbstractConverterEngine : public SAbstractProcessEngine
{
public:
    virtual QHash<QString,QVariant> optionsOf( const QString & src_type , const QString & dst_type ) const;
    virtual void setDefaultOption( const QString & src_type , const QString & dst_type , const QHash<QString,QVariant> & options );
};

#endif // SABSTRACTCONVERTERENGINE_H
