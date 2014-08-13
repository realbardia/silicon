#ifndef SINIREADER_H
#define SINIREADER_H

#include <QString>
#include <QStringList>
#include <QList>

#include "sfilestringlist.h"
#include "sdatabase_global.h"

class SDATABASESHARED_EXPORT SIniReader
{
public:
    SIniReader( const QString & path );
    ~SIniReader();

    void addHead( const QString & head );
    void addChild( const QString & head , const QString & child );
    void set( const QString & head , const QString & child , const QString & str , bool AutoAdd = true );

    void remove( const QString & head );
    void remove( const QString & head , const QString & child );

    void rename( const QString & head , const QString & newHead );
    void rename( const QString & head , const QString & child , const QString & newChild );

    QString head( int index );
    QString child( const QString & head , int index );
    QString read( const QString & head , const QString & child );

    int headsCount();
    int childCounts( const QString & head );

    void save();
    void reload();

private:
    SFileStringList *list;

    void loadOffsets();
    bool isHead( const QString & head );
    bool isChild( const QString & child );
    int findHead( const QString & head );
    int findChild( const QString & head , const QString & child );

    QList<int> heads;
};

#endif // SINIREADER_H
