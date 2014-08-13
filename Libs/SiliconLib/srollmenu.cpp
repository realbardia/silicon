/*
 * File:      srollmenu.cpp
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

#include <QPushButton>
#include <QHBoxLayout>

#include "sanimation.h"
#include "sstylesheet.h"

#include "srollmenu.h"



/*================================*
 |                                |
 |        SRollMenuPrivate        |
 |                                |
 *================================*/

class SRollMenuPrivate
{
public:
    QPushButton *button;
    QWidget *wgt;               // hold pointer of widget
    QWidget *fake_wgt;          // A widget that use insted of wgt on Animations.

    QHBoxLayout *layout;

    SAnimation *animation;

    bool animStt;               // hold animation status
    QSize tSize;                // hold target size
    QSize sSize;                // hold start size
    QString style;
};



/*================================*
 |                                |
 |            SRollMenu           |
 |                                |
 *================================*/

SRollMenu::SRollMenu( QWidget *parent ) : QFrame( parent )
{
    p = new SRollMenuPrivate;

    this->setSizePolicy( QSizePolicy::Minimum , QSizePolicy::Minimum );
    p->sSize = QSize(0,0);

    p->button = new QPushButton();
        p->button->setFocusPolicy( Qt::NoFocus );
        QObject::connect( p->button , SIGNAL(clicked()) , this , SLOT(autoExpand()) );

    p->fake_wgt = new QWidget();
        p->fake_wgt->hide();

    p->wgt = new QWidget();
        p->wgt->hide();

    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->button );
        p->layout->addWidget( p->fake_wgt );
        p->layout->addWidget( p->wgt );
        p->layout->setContentsMargins(0,0,0,0);
        p->layout->setSpacing(0);

    p->animation = new SAnimation( this );
        QObject::connect( p->animation , SIGNAL(finished()) , this , SLOT(animationFinished()) );

    p->animStt = false;
}

void SRollMenu::setText( const QString & name )
{
    p->button->setText( name );
}

void SRollMenu::setWidget( QWidget *widget )
{
    p->layout->removeWidget( p->wgt );
    delete p->wgt;

    p->wgt = widget;
    p->layout->addWidget( widget );
    widget->hide();
}

void SRollMenu::setAnimation(bool stt)
{
    p->animStt = stt;
}

void SRollMenu::setTargetSize( QSize size )
{
    p->tSize = size;
}

void SRollMenu::setStartSize( QSize size )
{
    p->sSize = size;
}

void SRollMenu::setButtonSize( QSize size )
{
    p->button->setFixedSize( size );
}

void SRollMenu::setStyleSheet( const QString & style )
{
    p->style = style;

    SStyleSheet css;
        css.addStyle( style );
        css.enter( "SRollMenu" );
        css.replace( "button" , "QPushButton" );

    p->button->setStyleSheet( css.filter("QPushButton") );

    css.setStyle( css.filter("widget") );
        css.enter( "widget" );

    QString widget_str = css.style();

    p->wgt->setStyleSheet( widget_str );
    p->fake_wgt->setStyleSheet( widget_str );
}

QString SRollMenu::text() const
{
    return p->button->text();
}

QWidget *SRollMenu::widget() const
{
    return p->wgt;
}

bool SRollMenu::animationStt() const
{
    return p->animStt;
}

QSize SRollMenu::targetSize() const
{
    return p->tSize;
}

QSize SRollMenu::startSize() const
{
    return p->sSize;
}

QSize SRollMenu::buttonSize() const
{
    return p->button->size();
}

QString SRollMenu::styleSheet() const
{
    return p->style;
}

bool SRollMenu::isExpanded() const
{
    return !p->wgt->isHidden();
}

void SRollMenu::autoExpand()
{
    this->expand( p->wgt->isHidden() );
}

void SRollMenu::expand(bool stt)
{
    if( p->animation->isActive() )
        return ;

    emit this->expanded( stt );

    if( p->animStt && stt )
    {
        p->fake_wgt->setFixedSize( p->sSize );
        p->fake_wgt->show();

        p->wgt->setFixedSize( p->tSize );

        p->animation->start( p->fake_wgt , p->tSize , SAnimation::RESIZE , 7 , SAnimation::MIN_MAX );
        return ;
    }
    else if( p->animStt && !stt )
    {
        p->fake_wgt->setFixedSize( p->tSize );
        p->fake_wgt->show();

        p->wgt->hide();
        p->wgt->setFixedSize( p->sSize );

        p->animation->start( p->fake_wgt , p->sSize , SAnimation::RESIZE , 7 , SAnimation::MIN_MAX );
        return ;
    }

    if( stt )
    {
        p->wgt->show();
        p->wgt->setFixedSize( p->tSize );
        p->wgt->setFocus();
    }
    else
    {
        p->wgt->hide();
        p->wgt->setFixedSize( p->sSize );
    }
}

void SRollMenu::animationFinished()
{
    if( p->fake_wgt->size() == p->sSize )
    {
        p->wgt->hide();
        p->fake_wgt->hide();
        return ;
    }

    p->fake_wgt->hide();
    p->wgt->show();
    p->wgt->setFocus();
}

SRollMenu::~SRollMenu()
{
    delete p;
}
