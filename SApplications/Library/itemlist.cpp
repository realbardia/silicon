#include "itemlist.h"

#include <QListWidgetItem>
#include <QSize>
#include <QDrag>

#include <smastericons.h>

ItemList::ItemList(QWidget *parent) :
    QListWidget(parent)
{
    this->setSortingEnabled( true );
    library = 0;
}

void ItemList::setLibrary( Librarydb *library )
{
    this->library = library;
}

void ItemList::setItems( const QStringList & list )
{
    this->clear();

    QString Name;
    QListWidgetItem *item;

    for( int i=0 ; i<list.count() ; i++ )
    {
        Name.clear();
        if( library != 0 )
            Name = library->readOption( list.at(i) , Librarydb::NAME );

        item = new QListWidgetItem();
            item->setText( Name );
            item->setIcon( SMasterIcons::icon( QSize(48,48) , "application-x-cd-image.png" ) );
            item->setStatusTip( list.at(i) );
            item->setToolTip( list.at(i) );

        this->addItem( item );
    }

    this->sortItems();
}
