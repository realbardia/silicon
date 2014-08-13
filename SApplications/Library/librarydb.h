#ifndef PUBCONF_H
#define PUBCONF_H

#include <QApplication>
#include <QList>

#include <sdatabase.h>

class Librarydb : public SDataBase
{
public:
    Librarydb();
    Librarydb(const QString & path);

    void setPath(const QString & path);

    void setOption( const QString & fileName , int type , const QString & str );
    QString readOption( const QString & fileName , int type );

    enum type { TAGS , NAME , END };

private:
    QList<QString> options;
};

#endif // PUBCONF_H
