#include "formatlist.h"

FormatList::FormatList(QWidget *parent) :
    SFilterList(parent)
{
}

QStringList FormatList::filtersNames( const QString & path )
{
    QString Type;
    for( int i=1 ; i<path.size() && i<5 ; i++ )
        if( path.right( i )[0] == '.' )
        {
            Type = path.right( i - 1 );
            break;
        }

    if( Type.isEmpty() )
        Type = tr("unknown");

    return QStringList() << tr("All") << Type.toLower();
}
