#ifndef SSTRINGLIST_H
#define SSTRINGLIST_H

#include <QList>

#include "libsilicon_global.h"
#include "sstring.h"

class LIBSILICON_EXPORT SStringList : public QList<SString>
{
public:
    SStringList ();
    SStringList ( const QString & str );
    SStringList ( const QStringList & other );
    SStringList ( const SStringList & other );
    SStringList ( const QList<QString> & other );

    QStringList toQStringList() const;

};

#endif // SSTRINGLIST_H
