/*
 * File:      scontextbutton.cpp
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

#include "scontextbutton.h"

#include <QMoveEvent>

class MyMenu : public QMenu
{
public:
    MyMenu( QWidget *parent = 0);

protected:
    void moveEvent( QMoveEvent *event );
};

MyMenu::MyMenu( QWidget *parent ) : QMenu( parent )
{}

void MyMenu::moveEvent( QMoveEvent *event )
{
    int parent_pad = 0;
    int parent_pos = event->pos().x();
    if( parentWidget() != 0 )
    {
        parent_pad = parentWidget()->width();
        parent_pos = parentWidget()->mapToGlobal(QPoint()).x();
    }

    move( parent_pos + parent_pad - width() , event->pos().y()-2 );
}




/*================================*
 |                                |
 |      SContextButtonPrivate     |
 |                                |
 *================================*/

class SContextButtonPrivate
{
public:
    QMenu *menu;
    QPalette org_palette;
    bool colorized_stt;
};




/*================================*
 |                                |
 |         SContextButton         |
 |                                |
 *================================*/

SContextButton::SContextButton(QWidget *parent) : QPushButton( parent )
{
    p = new SContextButtonPrivate;
    p->org_palette = palette();

    p->menu = new QMenu( this );
        p->menu->setMinimumWidth( 253 );
        p->colorized_stt = false;

    connect( p->menu , SIGNAL(aboutToShow()) , this , SLOT(showMenu()) );
    setMenu( p->menu );
}

void SContextButton::addItem(QAction *action)
{
    p->menu->addAction( action );
}

void SContextButton::addSeparator()
{
    p->menu->addSeparator();
}

void SContextButton::addMenu(QMenu *menu)
{
    p->menu->addMenu( menu );
}

void SContextButton::addMenuPanel( SMenuPanel *menu )
{
    for( int i=0 ; i<menu->count() ; i++ )
        p->menu->addAction( menu->at(i) );
}

void SContextButton::removeItem(QAction *action)
{
    p->menu->removeAction( action );
}

void SContextButton::clear()
{
    p->menu->clear();
}

void SContextButton::setColorizedMenu( bool stt )
{
    p->colorized_stt = stt;
}

bool SContextButton::colorizedMenu() const
{
    return p->colorized_stt;
}

void SContextButton::paintEvent( QPaintEvent *event )
{
    QPushButton::paintEvent( event );

    if( p->colorized_stt )
    {
        QPalette plt = palette();
            plt.setColor( QPalette::Window     , plt.highlight().color()       );
            plt.setColor( QPalette::WindowText , plt.highlightedText().color() );

        p->menu->setPalette( plt );
    }
    else
    {
        p->menu->setPalette( p->org_palette );
    }
}

void SContextButton::showMenu()
{

}

SContextButton::~SContextButton()
{
    delete p->menu;
    delete p;
}
