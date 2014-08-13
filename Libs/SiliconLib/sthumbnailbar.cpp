/*
 * File:      sthumbnailbar.cpp
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

#include "sthumbnailbar.h"

#include <QUrl>
#include <QSpacerItem>
#include <QCoreApplication>
#include <QList>
#include <QtAlgorithms>
#include <QDir>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMimeData>

#include "spicturewidget.h"
#include "sstring.h"



/*================================*
 |                                |
 |        Extra Functions         |
 |                                |
 *================================*/

QList<SString> fromQStringList( const QStringList & list )
{
    QList<SString> res;
    for( int i=0 ; i<list.count() ; i++ )
        res << SString(list.at(i));

    return res;
}





/*================================*
 |                                |
 |      SThumbnailBarPrivate      |
 |                                |
 *================================*/

class SThumbnailBarPrivate
{
public:
    QList<SPictureWidget *> list;
    QStringList path_list;
    QDir::SortFlags sort_flag;

    QHBoxLayout *h_layout;
    QVBoxLayout *v_layout;
    QLayout *current_layout_type;
    QSize thumbs_size;
    int current_index_int;

    SPictureWidget *current_item;

    int process_numbers;
    int process_max_number;
};





/*================================*
 |                                |
 |         SThumbnailBar          |
 |                                |
 *================================*/

SThumbnailBar::SThumbnailBar( QWidget *parent ) : QWidget( parent )
{
    p = new SThumbnailBarPrivate;
    this->setAcceptDrops( true );

    p->current_item = 0;
    p->current_index_int = -1;
    p->process_max_number = 7;
    p->process_numbers = 0;

    p->sort_flag = QDir::Name;
    p->thumbs_size = QSize(82,66);

    p->h_layout = new QHBoxLayout( this );
        p->h_layout->addSpacerItem( new QSpacerItem( 4 , 2 , QSizePolicy::Expanding ,QSizePolicy::Minimum ) );
        p->h_layout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->h_layout->setSpacing( 0 );

    p->current_layout_type = p->h_layout;
}

SThumbnailBar::~SThumbnailBar()
{
    this->clear();
    delete p;
}

void SThumbnailBar::addPath( const QString & path )
{
    if( !QDir(path).exists() || this->check_path_added(path) )
        return ;

    p->path_list << path;
    this->refreshBase( QStringList()<<path );
}

void SThumbnailBar::addPathList( const QStringList & list )
{
    QStringList accepted_list;
    for( int i=0 ; i<list.count() ; i++ )
    {
        QString path = list.at(i);
        if( !QDir(path).exists() || this->check_path_added(path) )
            continue ;

        p->path_list << path;
        accepted_list << path;
    }

    this->refreshBase( accepted_list );
}

void SThumbnailBar::setPathList( const QStringList & list )
{
    this->clear();
    this->addPathList( list );
}

void SThumbnailBar::addFile( const QString & file )
{/*
    if( this->check_file_added(file) )
        return ;
*/
    SPictureWidget *picture = new SPictureWidget();
        picture->setKeepProportion( true );
        picture->setButton( true );
        picture->setCheckable( true );
        picture->setFixedSize( p->thumbs_size );
        picture->drawFile( file );
        picture->setMargins( 4 , 4 , 4 , 4 );

    p->list << picture;
    p->h_layout->insertWidget( p->h_layout->count()-1 , picture );

    QObject::connect( picture , SIGNAL(clicked(SPictureWidget*)) , this , SLOT(item_clicked(SPictureWidget*)) );
    //QObject::connect( picture , SIGNAL(drawed()) , this , SLOT(removeProcess()) );
}

void SThumbnailBar::removeAt( int index )
{
    SPictureWidget *item = p->list.at(index);

    if( p->current_item == item )
    {
        if( p->list.count() == 1 )
            p->current_item = 0;
        else if( p->list.first() == item )
            this->next();
        else
            this->previous();
    }

    p->h_layout->removeWidget( item );
    p->list.removeOne( item );

    delete item;
}

void SThumbnailBar::removeOne( const QString & file )
{
    for( int i=0 ; i<p->list.count() ; i++ )
        if( p->list.at(i)->currentPath() == file )
            this->removeAt( i );
}

void SThumbnailBar::refresh()
{
    this->refreshBase( p->path_list );
}

void SThumbnailBar::refreshBase( const QStringList & list )
{
    QStringList filter_list;
        filter_list << "*.png" << "*.jpg" << "*.jpeg" << "*.gif" << "*.tif" ;

    QStringList files_list;
    for( int i=0 ; i<list.count() ; i++ )
    {
        QList<SString> str_list = fromQStringList( QDir(list.at(i)).entryList( filter_list , QDir::Files , p->sort_flag ) );
        qSort( str_list.begin() , str_list.end() );

        for( int j=0 ; j<str_list.count() ; j++ )
        {
            files_list.append( list.at(i) + '/' + str_list[j] );
        }
    }

    this->setUpdatesEnabled( false );

    for( int i=0 ; i<files_list.count() ; i++ )
    {
        this->addFile( files_list.at(i) );
        QCoreApplication::processEvents();
    }

    this->setUpdatesEnabled( true );
}

void SThumbnailBar::dropEvent( QDropEvent *event )
{
    QList<QUrl> urls_list = event->mimeData()->urls();
    if( urls_list.isEmpty() )
        return ;

    QStringList list;
    for( int i=0 ; i<urls_list.count() ; i++ )
    {
        QString path = urls_list.at(i).path();

#ifdef Q_OS_WIN32
        if( path[0] == '/' )
            path.remove( 0 , 1 );
#endif

        if( QFileInfo( path ).isDir() )
            list << path;
        else
            this->addFile( path );
    }

    this->addPathList( list );
    event->acceptProposedAction();
}

void SThumbnailBar::dragEnterEvent( QDragEnterEvent *event )
{
    QList<QUrl> urls_list = event->mimeData()->urls();
    if( urls_list.isEmpty() )
        return ;

    event->acceptProposedAction();
}

void SThumbnailBar::clear()
{
    p->current_item = 0;
    p->current_index_int = -1;

    for( int i=0 ; i<p->list.count() ; i++)
    {
        p->h_layout->removeWidget( p->list.at(i) );
        delete p->list.at(i);
    }

    p->list.clear();
    p->path_list.clear();
}

void SThumbnailBar::next()
{
    int next = this->currentIndex()+1;
    if( next >= this->count() )
        return ;

    this->setCurrentIndex( next );
}

void SThumbnailBar::previous()
{
    int prev = this->currentIndex()-1;
    if( prev < 0 )
        return ;

    this->setCurrentIndex( prev );
}

void SThumbnailBar::setCurrentIndex( const QString & path )
{
    for( int i=0 ; i<p->list.count() ; i++ )
        if( p->list[i]->currentPath() == path )
            this->setCurrentIndex( i );
}

void SThumbnailBar::setCurrentIndex( int i )
{
    p->list[i]->click();
}

int SThumbnailBar::currentIndex() const
{
    return p->current_index_int;
}

QString SThumbnailBar::currentItem() const
{
    if( p->current_item == 0 )
        return QString();

    return p->current_item->currentPath();
}

int SThumbnailBar::count() const
{
    return p->list.count();
}

QRect SThumbnailBar::currentItemRect()
{
    if( p->current_item == 0 )
        return QRect();

    QPoint point = p->current_item->mapTo( this , QPoint() );
    QRect rect = p->current_item->rect();
        rect.setX(    point.x() );
        rect.setY(    point.y() );
        rect.setLeft( point.x() );
        rect.setTop(  point.y() );
        rect.setSize( p->current_item->size() );

    return rect;
}

void SThumbnailBar::setThumbnailSize( const QSize & size )
{
    p->thumbs_size = size;

    for( int i=0 ; i<p->list.count() ; i++ )
        p->list.at(i)->setFixedSize(size);
}

const QSize & SThumbnailBar::thumbnailSize() const
{
    return p->thumbs_size;
}

void SThumbnailBar::item_clicked( SPictureWidget *picture_widget )
{
    if( p->current_item != 0 )
    {
        p->current_item->setChecked( false );
        //p->current_item->setFixedSize( p->thumbs_size );
    }

    p->current_item = picture_widget;
        p->current_item->setChecked( true );
        //p->current_item->setFixedSize( p->thumbs_size.width()+13 , p->thumbs_size.height()+13 );

    p->current_index_int = p->list.indexOf( p->current_item );

    emit this->currentItemChanged( picture_widget->currentPath() );
    emit this->currentItemChanged( picture_widget->currentPixmap() );
}

void SThumbnailBar::removeProcess()
{
    if( p->process_numbers <= 0 )
        return ;

    p->process_numbers--;
}

bool SThumbnailBar::addProcess()
{
    if( p->process_numbers+1 >= p->process_max_number )
        return false;

    p->process_numbers++;
    return true;
}

bool SThumbnailBar::check_path_added( const QString & path )
{
    for( int i=0 ; i<p->path_list.count() ; i++ )
        if( p->path_list.at(i) == path )
            return true;

    return false;
}

bool SThumbnailBar::check_file_added( const QString & file )
{
    for( int i=0 ; i<p->list.count() ; i++ )
        if( p->list[i]->currentPath() == file )
            return true;

    return false;
}
