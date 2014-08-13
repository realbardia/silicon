/*
 * File:      stabitem.cpp
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

#include <QTimer>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

#include "stabitem.h"
#include "smastericons.h"



/*================================*
 |                                |
 |        STabItemPrivate         |
 |                                |
 *================================*/

class STabItemPrivate
{
public:
    QHBoxLayout *layout;
    QTimer *drag_timer;
    QPushButton *close_button;
};



/*================================*
 |                                |
 |            STabItem            |
 |                                |
 *================================*/

STabItem::STabItem(QWidget *parent)
{
    setup( parent );
}

STabItem::STabItem(const QString & name,QWidget *parent)
{
    setup( parent );
    setText( name );
}

STabItem::STabItem(const QIcon & icon,const QString & name,QWidget *parent)
{
    setup( parent );
    setIcon( icon );
    setText( name );
}

STabItem::~STabItem()
{
    delete p;
}

QPushButton *STabItem::closeButton() const
{
    return p->close_button;
}

void STabItem::setup(QWidget *parent)
{
    setFlat( true );
    setAcceptDrops( true );
    setCheckable( true );
    setFocusPolicy(Qt::NoFocus);

    p = new STabItemPrivate;
    p->drag_timer = 0;

    p->close_button = new QPushButton();
        p->close_button->setFixedSize( 14 , 14 );
        p->close_button->setIconSize( QSize(10,10) );
        p->close_button->setFocusPolicy( Qt::NoFocus );
        p->close_button->setIcon( SMasterIcons::icon( QSize(48,48) , "window-close.png" ).pixmap( 10,10 ) );

    p->layout = new QHBoxLayout();
        p->layout->addSpacerItem( new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum) );
        p->layout->addWidget( p->close_button );
        p->layout->setContentsMargins(3,3,3,3);

    this->setLayout( p->layout );
    this->setParent( parent );


    /*
     * Connect close button to sendCloseRequestSignal
     * When Use click on close button , STabItem emit closeRequest SIGNAL
     */
    QObject::connect( p->close_button , SIGNAL(clicked()) , this , SLOT(sendCloseRequestSignal()) );
    QObject::connect( this            , SIGNAL(clicked()) , this , SLOT(sendClickSignal())        );
}

void STabItem::sendClickSignal()
{
    emit this->clicked( this );
}

void STabItem::sendCloseRequestSignal()
{
    emit this->closeRequest( this );
}

void STabItem::dragTimeFinished()
{
    this->stopTimer();
    this->click();
}

void STabItem::stopTimer()
{
    if( p->drag_timer != 0 )
    {
        QObject::disconnect( p->drag_timer , SIGNAL(timeout()) , this , SLOT(dragTimeFinished()) );
        delete p->drag_timer;
        p->drag_timer = 0;
    }
}

void STabItem::dragEnterEvent( QDragEnterEvent *event )
{
    p->drag_timer = new QTimer( this );
    QObject::connect( p->drag_timer , SIGNAL(timeout()) , this , SLOT(dragTimeFinished()) );
    p->drag_timer->start( 1000 );

    event->acceptProposedAction();
}

void STabItem::dragLeaveEvent( QDragLeaveEvent * )
{
    this->stopTimer();
}

void STabItem::dragMoveEvent( QDragMoveEvent * )
{

}

void STabItem::dropEvent( QDropEvent * )
{
    this->stopTimer();
}

void STabItem::enterEvent( QEvent *event )
{
    QPushButton::enterEvent( event );
    emit entered( this );
}

void STabItem::leaveEvent( QEvent *event )
{
    QPushButton::leaveEvent( event );
    emit leaved( this );
}
