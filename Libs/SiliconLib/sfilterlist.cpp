/*
 * File:      sfilterlist.cpp
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

#include "sfilterlist.h"

#include <QListWidget>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QList>

struct SFilterListItem
{
    QString name;
    QStringList list;
};

class SFilterListPrivate
{
public:
    ~SFilterListPrivate()
    {
        for( int i=0 ; i<list.count() ; i++ )
            delete list[i];
    }

    SFilterList *filter_list;

    QListWidget *list_widget;
    QHBoxLayout *layout;

    QList<SFilterListItem *> list;
};

SFilterList::SFilterList(QWidget *parent) :
    QWidget(parent)
{
    p = new SFilterListPrivate;
    p->filter_list = 0;

    p->list_widget = new QListWidget();
    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->list_widget );
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );

    QObject::connect( p->list_widget , SIGNAL(currentRowChanged(int)) , this , SLOT(filterChanged(int)) );
}

void SFilterList::setFilterList( SFilterList *f_list )
{
    p->filter_list = f_list;
}

void SFilterList::setItems( const QStringList & path_list )
{
    p->list.clear();
    p->list_widget->clear();

    this->addItems( path_list );

    if( p->list_widget->count() > 0 )
        p->list_widget->setCurrentRow( 0 );
}

void SFilterList::addItems( const QStringList & path_list )
{
    for( int i=0 ; i<path_list.count() ; i++ )
        this->addItem( path_list.at(i) );
}

void SFilterList::addItem( const QString & path )
{
    QStringList list = this->filtersNames( path );
    for( int i=0 ; i<list.count() ; i++ )
    {
        SFilterListItem *item = this->item( list.at(i) );
            item->list << path;
    }
}

void SFilterList::filterChanged( int row )
{
    if( row < 0 )
        return ;

    SFilterListItem *item = this->item( p->list_widget->item( row )->text() );

    if( p->filter_list != 0 )
        p->filter_list->setItems( item->list );

    emit this->rowChenged( item->list );
}

QStringList SFilterList::filtersNames( const QString & path )
{
    return QStringList()<< path;
}

SFilterListItem *SFilterList::item( const QString & name )
{
    for( int i=0 ; i<p->list.count() ; i++ )
        if( p->list.at(i)->name == name )
            return p->list.at(i);

    SFilterListItem *item = new SFilterListItem;
        item->name = name;

    if( p->filter_list != 0 )
        p->filter_list->addItem( name );

    p->list << item;
    p->list_widget->addItem( item->name );

    return item;
}

SFilterList::~SFilterList()
{
    delete p;
}
