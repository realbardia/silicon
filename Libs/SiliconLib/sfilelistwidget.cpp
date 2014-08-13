/*
 * File:      sfilelistwidget.cpp
 * Author:    Bardia Daneshvar (bardia@getsilicon.org)
 * Copyright: (C) 2010 Bardia Daneshvar
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "sfilelistwidget.h"

#include <QFileInfo>
#include <QUrl>
#include <QListWidgetItem>
#include <QMimeData>




/*================================*
 |                                |
 |     SFileListWidgetPrivate     |
 |                                |
 *================================*/

class SFileListWidgetPrivate
{
public:
    ~SFileListWidgetPrivate()
    {
        delete root;
    }

    SFolderGraph *root;

    QIcon folders_icon;
    QIcon files_icon;
};




/*================================*
 |                                |
 |         SFileListWidget        |
 |                                |
 *================================*/

SFileListWidget::SFileListWidget(QWidget *parent) :
    QListWidget(parent)
{
    p = new SFileListWidgetPrivate;

    this->setAcceptDrops( true );

    p->root = new SFolderGraph();

    QObject::connect( this , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(folderDoubleClicked(QListWidgetItem*)) );
}

void SFileListWidget::reload()
{
    this->clear();

    // Start Adding folders to list widget
    QStringList folders_list( p->root->currentFolders() );
        folders_list.sort();

    for( int i=0 ; i<folders_list.count() ; i++ )
    {
        QListWidgetItem *item = new QListWidgetItem();
            item->setText( folders_list.at(i) );
            item->setToolTip( folders_list.at(i) );
            item->setIcon( p->folders_icon );

        this->addItem( item );
    }
    // END Adding folders to list widget


    // Start Adding files to list widget
    QStringList files_list( p->root->currentFiles() );
        files_list.sort();

    for( int i=0 ; i<files_list.count() ; i++ )
    {
        QFileInfo file( files_list.at(i) );

        QListWidgetItem *item = new QListWidgetItem();
            item->setText( file.fileName() );
            item->setToolTip( file.fileName() + " - " + QString::number(file.size()/1024) + "KB" );
            item->setStatusTip( p->root->fileAddress(files_list.at(i)) );
            item->setIcon( p->files_icon );

        this->addItem( item );
    }
    // END Adding files to list widget


    QString current_path( p->root->currentPath() );
    if( current_path.size() != 1 )
        current_path.remove( current_path.size()-1 , 1 );

    emit this->addressChanged( current_path );
}

void SFileListWidget::setFoldersIcon( const QIcon & icon )
{
    p->folders_icon = icon;
}

void SFileListWidget::setFilesIcon( const QIcon & icon )
{
    p->files_icon = icon;
}

QStringList SFileListWidget::gaugingRoot() const
{
    return p->root->gaugingRoot();
}

QStringList SFileListWidget::gaugingCurrent() const
{
    return p->root->gaugingCurrent();
}

void SFileListWidget::folderDoubleClicked( QListWidgetItem *item )
{
    if( !item->statusTip().isEmpty() ) // If it's File and is not Folder
        return ;

    p->root->cd( item->text() );
    reload();
}

void SFileListWidget::cdUp()
{
    p->root->up();
    reload();
}

void SFileListWidget::cd( const QString & address )
{
    p->root->cd( address );
    reload();
}

void SFileListWidget::addFile( const QString & file )
{
    p->root->addFile( file );
    reload();
    emit this->reloaded();
}

void SFileListWidget::addFiles( const QStringList & files )
{
    p->root->addFiles( files );
    reload();
    emit this->reloaded();
}

void SFileListWidget::addFolder( const QString & folder )
{
    emit this->busy( true );
    p->root->addFolder( folder );

    emit this->busy( false );
    reload();
    emit this->reloaded();
}

void SFileListWidget::newFolder( const QString & name )
{
    p->root->newFolder( name );
    reload();
    emit this->reloaded();
}

void SFileListWidget::rename( const QString & currentName , const QString & newName )
{
    p->root->rename( currentName , newName );
    reload();
    emit this->reloaded();
}

void SFileListWidget::setFolderGraph( SFolderGraph *folderGraph )
{
    delete p->root;
    p->root = folderGraph;
    reload();

    emit this->reloaded();
}

bool SFileListWidget::isEmpty() const
{
    return ( p->root->rootCount() == 0 );
}

bool SFileListWidget::isBusy() const
{
    return p->root->isBusy();
}

void SFileListWidget::removeCurrent()
{
    QList<QListWidgetItem *> list = this->selectedItems();
    int cnt = list.count();
    for( int i=0 ; i<cnt ; i++ )
    {
        if( list.at(i)->statusTip().isEmpty() )     // It's Folder
            p->root->removeFolder( list.at(i)->text() );
        else                                        // It's File
            p->root->removeFile( list.at(i)->text() );
    }

    reload();
    emit this->reloaded();
}

void SFileListWidget::dropEvent( QDropEvent *event )
{
    QList<QUrl> list = event->mimeData()->urls();

    emit busy( true );
    setAcceptDrops( false );

    for( int i=0 ; i< list.count() ; i++ )
    {
        QString url_path = list.at(i).path();

#ifdef Q_OS_WIN32
        if( url_path[0] == '/' )
            url_path.remove( 0 , 1 );
#endif

        QFileInfo file( url_path );

        if( file.isDir() )
            p->root->addFolder( url_path );
        else if( file.isFile() )
            p->root->addFile( url_path );
    }

    emit busy( false );
    setAcceptDrops( true );

    event->acceptProposedAction();

    reload();
    emit reloaded();
}

void SFileListWidget::dragEnterEvent( QDragEnterEvent *event )
{
    if( event->mimeData()->hasUrls() )
    {
        //qDebug( event->mimeData()->text().toStdString().c_str() );
        event->acceptProposedAction();
    }
}

void SFileListWidget::dragMoveEvent( QDragMoveEvent * )
{

}

void SFileListWidget::resizeEvent( QResizeEvent * )
{
    sortItems();
}

SFileListWidget::~SFileListWidget()
{
    delete p;
}
