 #ifndef CHARLIST_H
#define CHARLIST_H

#include <QStringList>
#include <QString>
#include <QObject>
#include <QWidget>

#include <sfilterlist.h>

#include "librarydb.h"

class CharList : public SFilterList
{
    Q_OBJECT
public:
    CharList(QWidget *parent = 0);
    void setLibrary( Librarydb *lib );

protected:
    QStringList filtersNames( const QString & path );

private:
    Librarydb *library;
};

#endif // CHARLIST_H
