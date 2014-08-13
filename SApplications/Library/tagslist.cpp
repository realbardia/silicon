#include "tagslist.h"

TagsList::TagsList(QWidget *parent) :
    SFilterList(parent)
{
    library = 0;
}

void TagsList::setLibrary( Librarydb *lib )
{
    library = lib;
}

QStringList TagsList::filtersNames( const QString & path )
{
    if( library == 0 )
        return QStringList();

    QStringList list;
        list << tr("All") << library->readOption( path , Librarydb::TAGS ).toLower().split(",");

    return list;
}
