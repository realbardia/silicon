/*
 * File:      smenupanel.cpp
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

#include "smenupanel.h"

#include <QList>



/*================================*
 |                                |
 |        SMenuPanelPrivate       |
 |                                |
 *================================*/

class SMenuPanelPrivate
{
public:
    QList<QAction *> list;
    bool enabled;
};




/*================================*
 |                                |
 |           SMenuPanel           |
 |                                |
 *================================*/

SMenuPanel::SMenuPanel( QObject *parent ) : QObject(parent)
{
    p = new SMenuPanelPrivate;
    p->enabled = true;
}

SMenuPanel::~SMenuPanel()
{
    delete p;
}

void SMenuPanel::addAction( QAction *action )
{
    this->insertAction( this->count() , action );
}

QAction *SMenuPanel::addMenu( QMenu *menu )
{
    return this->insertMenu( this->count() , menu );
}

QAction *SMenuPanel::addSeparator()
{
    return this->insertSeparator( this->count() );
}

void SMenuPanel::insertAction( int i , QAction *action )
{
    if( p->list.contains(action) )
        return ;

    p->list.insert( i ,action );
    this->update();
}

QAction *SMenuPanel::insertMenu( int i , QMenu *menu )
{
    QAction *new_act = new QAction( menu->icon() , menu->title() , this );
        new_act->setMenu( menu );

    p->list.insert( i , new_act );
    this->update();

    return new_act;
}

QAction *SMenuPanel::insertSeparator( int i )
{
    QAction *sep_act = new QAction( this );
        sep_act->setSeparator( true );

    p->list.insert( i ,sep_act );
    this->update();

    return sep_act;
}

QAction *SMenuPanel::at( int i ) const
{
    return p->list.at(i);
}

int SMenuPanel::indexOf( QAction *action ) const
{
    return p->list.indexOf(action);
}

int SMenuPanel::count() const
{
    return p->list.count();
}

bool SMenuPanel::isEmpty() const
{
    return count() == 0;
}

void SMenuPanel::removeAt( int i )
{
    p->list.removeAt( i );
    this->update();
}

void SMenuPanel::remove( QAction *action )
{
    p->list.removeOne( action );
    this->update();
}

void SMenuPanel::clear()
{
    for( int i=0 ; i<p->list.count() ; i++ )
        p->list.removeAt( 0 );

    this->update();
}

QMenu *SMenuPanel::toQMenu()
{
    QMenu *menu = new QMenu();

    for( int i=0 ; i<p->list.count() ; i++ )
        menu->addAction( p->list.at(i) );

    return menu;
}

void SMenuPanel::setEnabled( bool stt )
{
    p->enabled = stt;
    for( int i=0 ; i<p->list.count() ; i++ )
        p->list.at(i)->setEnabled( stt );

    emit enabled( p->enabled );
    emit disabled( !p->enabled );
    emit updated();
    emit updated( this );
}

bool SMenuPanel::isEnabled() const
{
    return p->enabled;
}

bool SMenuPanel::isDisabled() const
{
    return !isEnabled();
}

void SMenuPanel::setDisabled( bool stt )
{
    setEnabled( !stt );
}
