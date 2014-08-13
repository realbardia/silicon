/*
 * File:      sanimation.cpp
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

#include "sanimation.h"

#include <QTimer>
#include <QWidget>



/*================================*
 |                                |
 |       SAnimationPrivate        |
 |                                |
 *================================*/

class SAnimationPrivate
{
public:
    QTimer *timer;
    QWidget *widget;
    QSize target;

    int changeSizeType;
    SAnimation::FixesOptions fix_option;

    bool first;
    QSize dest;
    int frames;
};



/*================================*
 |                                |
 |           SAnimation           |
 |                                |
 *================================*/

SAnimation::SAnimation( QObject *parent ) : QObject( parent )
{
    p = new SAnimationPrivate;

    p->frames     = 20;
    p->timer      = new QTimer( this );
    p->fix_option = SAnimation::NO_FIX;
}

void SAnimation::start( QWidget *widget , QSize target
                        , AnimationType animType , int duration , ChangedSizeType changeSize )
{
    switch( animType )
    {
        case SAnimation::RESIZE :
            p->target = target;
            p->widget = widget;
            p->first = true;
            QObject::connect( p->timer , SIGNAL(timeout()) , this , SLOT(resizeAnimation()) );
            p->timer->start( duration );
            break;
        case SAnimation::OFF :
            p->target = target;
            p->widget = widget;
            this->finish();
            break;
    }
    p->changeSizeType = changeSize;
}

bool SAnimation::isActive() const
{
    return p->timer->isActive();
}

void SAnimation::setFrameRate( int rate )
{
    p->frames = rate;
}

int SAnimation::frameRate() const
{
    return p->frames;
}

void SAnimation::setFixOptions( FixesOptions fix_option )
{
    p->fix_option = fix_option;
}

SAnimation::FixesOptions SAnimation::fixOptions() const
{
    return p->fix_option;
}

void SAnimation::resizeAnimation()
{
    if( p->first )
    {
        // Put First Jobs Here
        p->dest=p->widget->size();
        p->first=false;
    }

    int widthSec=0;
    if( p->dest.width() < p->target.width() )
        widthSec= p->target.width() / p->frames;
    else if( p->dest.width() > p->target.width() )
        widthSec= -1*p->dest.width() / p->frames;

    int heightSec=0;
    if( p->dest.height() < p->target.height() )
        heightSec= p->target.height() / p->frames;
    else if( p->dest.height() > p->target.height() )
        heightSec=-1*p->dest.height() / p->frames;

    bool HCheck=( p->widget->height() >= p->target.height() ) - ( p->widget->height()+heightSec < p->target.height() )
         , WCheck=( p->widget->width()  >= p->target.width() ) - ( p->widget->width()+ widthSec  < p->target.width() );

    if( ( WCheck==0 || widthSec==0  || p->fix_option == SAnimation::WIDTH_FIX )
                                      &&
        ( HCheck==0 || heightSec==0 || p->fix_option == SAnimation::HEIGHT_FIX ) )
    {
        this->finish();
        return ;
    }

    if( WCheck==0 )
        widthSec=0;
    if( HCheck==0 )
        heightSec=0;

    this->autoResize( p->widget , QSize( p->widget->width()+widthSec , p->widget->height()+heightSec ) );
}

void SAnimation::finish()
{
    this->autoResize( p->widget , p->target );
    p->first=true;
    p->timer->stop();
    QObject::disconnect( p->timer , SIGNAL(timeout()) , this , SLOT(resizeAnimation()) );

    emit finished( p->widget );
    emit this->finished();

    return ;
}

void SAnimation::autoResize( QWidget *widget , const QSize & size )
{
    if( p->changeSizeType==SAnimation::SIZE )
        this->manualResize( widget , size );
    else if( p->changeSizeType==SAnimation::MIN_MAX )
        this->minmaxResize( widget , size );
}

void SAnimation::manualResize( QWidget *widget , const QSize & size )
{
    widget->resize( size );
}

void SAnimation::minmaxResize( QWidget *widget , const QSize & size )
{
    if( p->fix_option == SAnimation::HEIGHT_FIX )
        widget->setFixedWidth( size.width() );
    else if( p->fix_option == SAnimation::WIDTH_FIX )
        widget->setFixedHeight( size.height() );
    else
        widget->setFixedSize( size );
}

SAnimation::~SAnimation()
{
    delete p;
}
