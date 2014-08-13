#ifndef SSTRING_H
#define SSTRING_H

#include <QString>
#include <QChar>
#include <QLatin1String>
#include <QByteArray>

#include "libsilicon_global.h"

class LIBSILICON_EXPORT SString : public QString
{
public:
    SString ();
    SString ( QChar ch );
    SString ( int size, QChar ch );
    SString ( const QLatin1String & str );
    SString ( const QString & other );
    SString ( const char * str );
    SString ( const QByteArray & ba );
    ~SString();

    bool operator< ( const QString & other ) const;
    bool operator< ( const QLatin1String & other ) const;
    bool operator< ( const QByteArray & other ) const;
    bool operator< ( const char * other ) const;
    bool operator<= ( const QString & other ) const;
    bool operator<= ( const QLatin1String & other ) const;
    bool operator<= ( const QByteArray & other ) const;
    bool operator<= ( const char * other ) const;

    bool operator> ( const QString & other ) const;
    bool operator> ( const QLatin1String & other ) const;
    bool operator> ( const QByteArray & other ) const;
    bool operator> ( const char * other ) const;
    bool operator>= ( const QString & other ) const;
    bool operator>= ( const QLatin1String & other ) const;
    bool operator>= ( const QByteArray & other ) const;
    bool operator>= ( const char * other ) const;

};

#endif // SSTRING_H
