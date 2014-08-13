#include "sfilemanager.h"

#include <QListWidgetItem>
#include <QList>
#include <QStringList>

#include "smastericons.h"

SFileManager::SFileManager(QWidget *parent) :
    QListWidget(parent)
{
}

void SFileManager::goTo( const QString & dir )
{
    current_dir.setPath( dir );

    QStringList dir_list = current_dir.entryList( QDir::Dirs );
    for( int i=0 ; i<dir_list.count() ; i++ )
    {
        QListWidgetItem *item = new QListWidgetItem();
            item->setText( dir_list.at( i ) );

        this->addItem( item );
    }

    QStringList file_list = current_dir.entryList( QDir::Files );
    for( int i=0 ; i<file_list.count() ; i++ )
    {
        QListWidgetItem *item = new QListWidgetItem();
            item->setText( file_list.at( i ) );

        this->addItem( item );
    }

}
