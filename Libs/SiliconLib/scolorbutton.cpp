/*
 * File:      scolorbutton.cpp
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

#include "scolorbutton.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QColorDialog>
#include <QSpacerItem>
#include <QPaintEvent>
#include <QColor>
#include <QRect>
#include <QPainter>




/*================================*
 |                                |
 |           SpecButton           |
 |                                |
 *================================*/

class SpecButton : public QPushButton
{
public:
    SpecButton( QWidget *parent = 0 );
    void setColor( const QColor & color );

protected:
    void paintEvent( QPaintEvent *event );

private:
    QColor color;
};

SpecButton::SpecButton( QWidget *parent ) :
        QPushButton( parent )
{
}

void SpecButton::setColor( const QColor & color )
{
    this->color = color;
    this->update();
}

void SpecButton::paintEvent( QPaintEvent *event )
{
    QPushButton::paintEvent( event );

    QRect rect = event->rect();
        rect.setLeft(   7 );
        rect.setTop(    7 );
        rect.setWidth(  rect.width()  - 8 );
        rect.setHeight( rect.height() - 8 );

    QPainter painter( this );
        painter.fillRect( rect , color );
}






/*================================*
 |                                |
 |      SColorButtonPrivate       |
 |                                |
 *================================*/

class SColorButtonPrivate
{
public:
    QHBoxLayout *layout;
    QHBoxLayout *check_layout;
    SpecButton *button;
    QSpacerItem *spacer;
    QWidget *check_widget;
    QCheckBox *check;
    QColor current_color;

    bool checkable_stt;
};




/*================================*
 |                                |
 |          SColorButton          |
 |                                |
 *================================*/

SColorButton::SColorButton( const QString & name , QWidget *parent ) :
    QWidget( parent )
{
    p = new SColorButtonPrivate;
    p->button = new SpecButton();
        p->button->setColor( p->current_color );

    p->spacer = new QSpacerItem( 4 , 2 , QSizePolicy::Expanding , QSizePolicy::Minimum );
    p->check  = new QCheckBox( name );
        p->check->setChecked( true );

    p->check_widget = new QWidget();
    p->check_layout = new QHBoxLayout( p->check_widget );
        p->check_layout->addWidget( p->check );
        p->check_layout->addSpacerItem( p->spacer );
        p->check_layout->setContentsMargins( 0 , 0 , 0 , 0 );

    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->check_widget );
        p->layout->addWidget( p->button       );
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );

    QObject::connect( p->button , SIGNAL(clicked())     , this      , SLOT(colorDialog())    );
    QObject::connect( p->check  , SIGNAL(toggled(bool)) , p->button , SLOT(setEnabled(bool)) );
    QObject::connect( p->check  , SIGNAL(toggled(bool)) , this      , SLOT(setChecked(bool)) );

    this->setCheckable( false );
}

void SColorButton::setColor( const QColor & color )
{
    p->current_color = color;
    p->button->setColor( p->current_color );

    emit this->colorSelected( p->current_color );
    emit this->colorSelected( p->current_color.name() );
    emit this->colorSelected( this );
}

const QColor & SColorButton::color() const
{
    return p->current_color;
}

bool SColorButton::isChecked() const
{
    return p->check->isChecked();
}

void SColorButton::setCheckable( bool stt )
{
    this->setChecked( stt );
    p->checkable_stt = stt;
    p->check_widget->setVisible( p->checkable_stt );
}

bool SColorButton::isCheckable() const
{
    return p->checkable_stt;
}

void SColorButton::colorDialog()
{
    this->setColor( QColorDialog::getColor( p->current_color , this ) );
}

void SColorButton::setChecked( bool stt )
{
    if( !this->isCheckable() )
        return ;

    p->check->setChecked( stt );
    this->setColor( p->current_color );
    emit this->checked( stt );
}

SColorButton::~SColorButton()
{
    delete p;
}
