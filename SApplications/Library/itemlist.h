#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QWidget>
#include <QListWidget>
#include <QStringList>

#include "librarydb.h"

class ItemList : public QListWidget
{
    Q_OBJECT
public:
    ItemList(QWidget *parent = 0);
    void setLibrary( Librarydb *library );

public slots:
    void setItems( const QStringList & list );

protected:

private:
    Librarydb *library;
};

#endif // ITEMLIST_H
