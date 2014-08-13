#ifndef SABSTRACTCONVERTER_H
#define SABSTRACTCONVERTER_H

#include "sabstractprocess.h"
#include "SiDi_global.h"

#include <QByteArray>
#include <QHash>
#include <QVariant>

class SAbstractConverterPrivate;
class SIDI_EXPORT SAbstractConverter : public SAbstractProcess
{
    Q_OBJECT
public:
    SAbstractConverter( QObject *parent = 0 );
    ~SAbstractConverter();

    void setSource( const QString & source );
    const QString & source() const;

    void setDestination( const QString & destination );
    const QString & destination() const;

signals:
    void preview( const QByteArray & bytes );
    void preview( const QVariant & variant );

private:
    SAbstractConverterPrivate *pp;
};

#endif // SABSTRACTCONVERTER_H
