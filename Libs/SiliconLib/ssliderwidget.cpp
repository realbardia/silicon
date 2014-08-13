/*
 * File:      ssliderwidget.cpp
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

#include "ssliderwidget.h"

#include <QSpacerItem>
#include <QHBoxLayout>
#include <QList>
#include <QSizePolicy>
#include <QResizeEvent>
#include <QHash>



/*================================*
 |                                |
 |      SSliderWidgetPrivate      |
 |                                |
 *================================*/

class SSliderWidgetPrivate
{
public:
    QHBoxLayout *layout;
    QSpacerItem *spacerItem;

    bool space_item_added;          // boolian that show spacer item added or no
    int focused_item;               // current focused item
    bool show_all_stt;

    QList<QWidget *> list;          // list of added widgets
    QHash<QWidget *,int> width_list;
};



/*================================*
 |                                |
 |         SSliderWidget          |
 |                                |
 *================================*/

SSliderWidget::SSliderWidget( QWidget *parent ) : QWidget( parent )
{
    p = new SSliderWidgetPrivate;

    p->spacerItem = new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);

    p->layout = new QHBoxLayout( this );
        p->layout->addSpacerItem( p->spacerItem );
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->layout->setSpacing( 0 );

    p->space_item_added = true;
    p->show_all_stt = false;
}

void SSliderWidget::addWidget( QWidget *widget , int width )
{
    p->layout->insertWidget( p->list.count() , widget );
    p->list << widget;
    p->width_list.insert( widget , width );

    if( width > -1 )
        widget->setFixedWidth( width );
/*
    if( p->space_item_added )
    {
        p->layout->takeAt( p->layout->count()-1 );
        p->space_item_added = false;
    }
*/
    this->setFocus( widget );
}

void SSliderWidget::insertWidget( QWidget *widget , int index , int width )
{
    p->layout->insertWidget( index , widget );
    p->list.insert( index , widget );
    p->width_list.insert( widget , width );

    if( width > -1 )
        widget->setFixedWidth( width );
/*
    if( p->space_item_added )
    {
        p->layout->takeAt( p->layout->count()-1 );
        p->space_item_added = false;
    }
*/
    this->setFocus( widget );
}

void SSliderWidget::removeWidget( QWidget *widget )
{
    for( int i=0 ; i<p->list.count() ; i++ )
        if( widget == p->list.at(i) )
            this->removeAt( i );
}

void SSliderWidget::removeAt( int index )
{
    QWidget *item = p->list.at( index );

    p->layout->removeWidget( item );
    p->list.removeOne( item );
    p->width_list.remove( item );
}

void SSliderWidget::setFocus( int index )
{
    QWidget *item;
    for( int i=0 ; i<p->list.count() ; i++ )
    {
        item = p->list.at(i);
            item->setEnabled( i == index || p->show_all_stt );
            item->setHidden( i > index && !p->show_all_stt );
    }

    p->focused_item = index;

    if( ( p->focused_item == this->count()-1 || p->show_all_stt ) && p->space_item_added )
    {
        p->layout->takeAt( p->layout->count()-1 );
        p->space_item_added = false;
    }
    else if( !( p->focused_item == this->count()-1 || p->show_all_stt ) && !p->space_item_added )
    {
        p->layout->addSpacerItem( p->spacerItem );
        p->space_item_added = true;
    }

}

void SSliderWidget::setFocus( QWidget *widget )
{
    this->setFocus( p->list.indexOf( widget ) );
}

QWidget *SSliderWidget::widgetAt( int index ) const
{
    return p->list.at( index );
}

QWidget *SSliderWidget::focusedWidget() const
{
    return p->list.at( p->focused_item );
}

int SSliderWidget::focusedIndex() const
{
    return p->focused_item;
}

int SSliderWidget::count() const
{
    return p->list.count();
}

bool SSliderWidget::isShowingAll() const
{
    return p->show_all_stt;
}

void SSliderWidget::next()
{
    if( p->focused_item+1 == p->list.count() )
        return ;

    this->setFocus( p->focused_item+1 );
}

void SSliderWidget::previous()
{
    if( p->focused_item-1 < 0 )
        return ;

    this->setFocus( p->focused_item-1 );
}

void SSliderWidget::setShowingAll( bool stt )
{
    p->show_all_stt = stt;
    this->setFocus( this->focusedIndex() );
}

void SSliderWidget::resizeEvent( QResizeEvent *event )
{
    int full_size = event->size().width();
    int no_fixed_numbers = 0;

    QList<int> valus = p->width_list.values();
    for( int i=0 ; i<valus.count() ; i++ )
    {
        if( valus.at(i) == -1 )
            no_fixed_numbers ++;
        else
            full_size -= valus.at(i);
    }

    int fix_count = 0;
    for( int i=0 ; i<p->list.count() ; i++ )
    {
        int width = p->width_list.value( p->list.at(i) );
        if( width == -1 && fix_count < no_fixed_numbers-1 )
        {
            p->list.at(i)->setFixedWidth( full_size / no_fixed_numbers );
            fix_count++;
        }
    }
}

SSliderWidget::~SSliderWidget()
{
    delete p;
}
