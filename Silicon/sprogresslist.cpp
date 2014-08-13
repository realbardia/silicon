/*
 * File:      sprogresslist.cpp
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

#include "sprogresslist.h"

#include <QVBoxLayout>
#include <SWidgetList>



/*================================*
 |                                |
 |      SProgressListPrivate      |
 |                                |
 *================================*/

class SProgressListPrivate
{
public:
    SWidgetList *view;
    QVBoxLayout *baseLayout;

    int public_value;
};



/*================================*
 |                                |
 |         SProgressList          |
 |                                |
 *================================*/

SProgressList::SProgressList( QWidget *parent ) : QWidget( parent )
{
    p = new SProgressListPrivate;
        p->public_value = 0;

    p->view = new SWidgetList();

    p->baseLayout = new QVBoxLayout( this );
        p->baseLayout->addWidget( p->view );
        p->baseLayout->setContentsMargins(0,0,0,0);

    connect( p->view , SIGNAL(clicked(QWidget*)) , SLOT(expand(QWidget*)) );
}

void SProgressList::add( SProgressListItem *item )
{
    insert( count() , item );
}

void SProgressList::insert( int index , SProgressListItem *item )
{
    p->view->insertWidget( index , item );
    connect( item , SIGNAL(destroyed(SProgressListItem*)) , SLOT(itemDestroyed(SProgressListItem*)) );

    public_value_changed();
}

void SProgressList::remove( int index )
{
    delete p->view->widget( index );
    public_value_changed();
}

void SProgressList::remove( SProgressListItem *item )
{
    delete item;
    public_value_changed();
}

void SProgressList::take( SProgressListItem *item )
{
    p->view->remove( item );
    disconnect( item , SIGNAL(destroyed(SProgressListItem*)) , this , SLOT(itemDestroyed(SProgressListItem*)) );

    public_value_changed();
}

void SProgressList::take( int index )
{
    take( static_cast<SProgressListItem*>(p->view->widget(index)) );
}

int SProgressList::count() const
{
    return p->view->count();
}

int SProgressList::index( SProgressListItem *item ) const
{
    return p->view->indexOf(item);
}

int SProgressList::publicValue() const
{
    return p->public_value;
}

void SProgressList::expand( QWidget *item )
{
    static_cast<SProgressListItem*>(item)->autoExpand();
}

void SProgressList::itemDestroyed( SProgressListItem *item )
{
    p->view->remove( item );
}

void SProgressList::public_value_changed()
{
    p->public_value = 0;
    for( int i=0 ; i<p->view->count() ; i++ )
        p->public_value += static_cast<SProgressListItem*>(p->view->widget(i))->value() / p->view->count();
}

SProgressList::~SProgressList()
{
    delete p;
}

