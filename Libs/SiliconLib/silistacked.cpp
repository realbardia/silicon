/*
 * File:      silistacked.cpp
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

#include "silistacked.h"

#include <QPalette>
#include <QColor>
#include <QBrush>
#include <QStackedWidget>
#include <QHBoxLayout>

#include <math.h>




/*================================*
 |                                |
 |       siliStackedPrivate       |
 |                                |
 *================================*/

class siliStackedPrivate
{
public:
    QStackedWidget *stackedWidget;
    QHBoxLayout *layout;
};




/*================================*
 |                                |
 |           siliStacked          |
 |                                |
 *================================*/

siliStacked::siliStacked( QWidget *parent ) : QWidget( parent )
{
    p = new siliStackedPrivate;

    p->stackedWidget = new QStackedWidget();
    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->stackedWidget );
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );

    this->removePage( 0 );
}

void siliStacked::addPage()
{
    QWidget *page = new QWidget();
    p->stackedWidget->addWidget( page );
}

void siliStacked::insertPage( int index )
{
    QWidget *page = new QWidget();
    p->stackedWidget->insertWidget( index , page );
}

void siliStacked::removePage( int index )
{
    QWidget *page = p->stackedWidget->widget( index );
    p->stackedWidget->removeWidget( page );
}

void siliStacked::move( int from , int to )
{
    QWidget *widget=p->stackedWidget->widget( from );
    p->stackedWidget->removeWidget( widget );
    if( from<to )
        p->stackedWidget->insertWidget( to-1 , widget );
    else
        p->stackedWidget->insertWidget( to , widget );
}

QWidget *siliStacked::page( int index ) const
{
    return p->stackedWidget->widget( index );
}

QWidget *siliStacked::currentPage() const
{
    return p->stackedWidget->currentWidget();
}

int siliStacked::currentIndex() const
{
    return p->stackedWidget->currentIndex();
}

int siliStacked::count() const
{
    return p->stackedWidget->count();
}

void siliStacked::setCurrentWidget( QWidget *page )
{
    p->stackedWidget->setCurrentWidget( page );
}

void siliStacked::setCurrentIndex( int index )
{
    p->stackedWidget->setCurrentIndex( index );
}

siliStacked::~siliStacked()
{
    delete p;
}
