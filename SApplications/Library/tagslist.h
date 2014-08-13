#ifndef TAGSLIST_H
#define TAGSLIST_H

#include <QStringList>
#include <QString>
#include <QObject>
#include <QWidget>

#include <sfilterlist.h>

#include "librarydb.h"

class TagsList : public SFilterList
{
    Q_OBJECT
public:
    TagsList(QWidget *parent = 0);
    void setLibrary( Librarydb *lib );

protected:
    QStringList filtersNames( const QString & path );

private:
    Librarydb *library;
};

#endif // TAGSLIST_H
