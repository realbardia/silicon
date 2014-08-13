/*
 * File:      sinnerprogressbar.cpp
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

#include "sinnerprogressbar.h"

#include <QPalette>
#include <QPainter>
#include <QFont>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QAction>

#include "QProgressIndicator/QProgressIndicator.h"
#include "scolor.h"




/*================================*
 |                                |
 |    SInnerProgressBarPrivate    |
 |                                |
 *================================*/

class SInnerProgressBarPrivate
{
public:
    QProgressIndicator *progress_indicator;
    QProgressBar *progress_bar;
    QLabel *label;

    QVBoxLayout *vlayout;
    QHBoxLayout *hlayout;

    QWidget *bar_widget;
    QWidget *progress_indicator_parent;
    QHBoxLayout *progress_indicator_layout;

    QColor color_obj;
};




/*================================*
 |                                |
 |        SInnerProgressBar       |
 |                                |
 *================================*/

SInnerProgressBar::SInnerProgressBar(QWidget *parent) :
    QWidget(parent)
{
    p = new SInnerProgressBarPrivate;

    p->bar_widget = 0;
    p->color_obj.setAlpha( 0 );

    p->progress_indicator = new QProgressIndicator();

    p->progress_indicator_parent = new QWidget();
    p->progress_indicator_layout = new QHBoxLayout( p->progress_indicator_parent );
        p->progress_indicator_layout->addSpacerItem( new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum) );
        p->progress_indicator_layout->addWidget( p->progress_indicator );
        p->progress_indicator_layout->addSpacerItem( new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum) );

    p->progress_bar = new QProgressBar();
        p->progress_bar->setValue( 0 );

    QFont font = this->font();
        //font.setBold( true );
        font.setItalic( true );

    p->label = new QLabel();
        p->label->setFont( font );
        p->label->setAlignment( Qt::AlignHCenter );

    p->vlayout = new QVBoxLayout();
        p->vlayout->addSpacerItem( new QSpacerItem(2, 4, QSizePolicy::Minimum, QSizePolicy::Expanding) );
        p->vlayout->addWidget( p->progress_indicator_parent );
        p->vlayout->addWidget( p->progress_bar );
        p->vlayout->addWidget( p->label );
        p->vlayout->addSpacerItem( new QSpacerItem(2, 4, QSizePolicy::Minimum, QSizePolicy::Expanding) );

    p->hlayout = new QHBoxLayout( this );
        p->hlayout->addSpacerItem( new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum) );
        p->hlayout->addLayout( p->vlayout );
        p->hlayout->addSpacerItem( new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum) );

    this->setWidth( 250 );
}

void SInnerProgressBar::setWidth( int width )
{
    p->progress_bar->setFixedWidth( width );
    p->label->setFixedWidth( width );
    p->progress_indicator_parent->setFixedWidth( width );
}

int SInnerProgressBar::width() const
{
    return p->progress_bar->width();
}

void SInnerProgressBar::setBarWidget( QWidget *widget )
{
    p->bar_widget = widget;
}

QWidget *SInnerProgressBar::barWidget() const
{
    return p->bar_widget;
}

void SInnerProgressBar::setColor( const QColor & color )
{
    p->color_obj = color;
    SColor invert_color( p->color_obj );
        invert_color.invert();
        invert_color.setAlpha( 255 );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText , invert_color );

    p->label->setPalette( palette );
    p->progress_indicator->setColor( invert_color );
}

QColor SInnerProgressBar::color() const
{
    return p->color_obj;
}

void SInnerProgressBar::setProgress( int percent )
{
    p->progress_bar->setValue( percent );
    emit this->valueChanged( percent );
}

void SInnerProgressBar::start()
{
    p->progress_indicator->startAnimation();
    emit this->started();
}

void SInnerProgressBar::stop()
{
    p->progress_indicator->stopAnimation();
    emit this->stopped();
}

void SInnerProgressBar::setText( const QString & str )
{
    p->label->setText( str );
    emit this->textChanged( str );
}

void SInnerProgressBar::paintEvent( QPaintEvent *event )
{
    QPainter painter(this);
        painter.setRenderHint( QPainter::SmoothPixmapTransform , true );
        painter.fillRect( event->rect() , p->color_obj );
}

SInnerProgressBar::~SInnerProgressBar()
{
    delete p;
}
