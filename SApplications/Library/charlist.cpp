#include "charlist.h"

#include <QFileInfo>

CharList::CharList(QWidget *parent) :
    SFilterList(parent)
{
    library = 0;
}

void CharList::setLibrary( Librarydb *lib )
{
    library = lib;
}

QStringList CharList::filtersNames( const QString & path )
{
    QString fileName = library->readOption( path , Librarydb::NAME );
    QChar first_char = fileName[0];

    if( !first_char.isLetterOrNumber() )
        first_char = '.';

    return QStringList()<< tr("All") << first_char.toUpper();
}
