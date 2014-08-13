#ifndef SARGUMENTLIST_H
#define SARGUMENTLIST_H

#include <QString>
#include <QStringList>
#include <QList>

#include "libsilicon_global.h"

struct LIBSILICON_EXPORT SArgument
{
    QString arg;
    QStringList list;
};

class LIBSILICON_EXPORT SArgumentList : public QList<SArgument>
{
public:
    SArgumentList();
    SArgumentList( const QList<SArgument> & other );
};

#endif // SARGUMENTLIST_H
