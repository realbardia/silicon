/*
 * File:      sdrawer.cpp
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

#include "sdrawer.h"

#include <QImageReader>
#include <QSizePolicy>
#include <QSize>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPoint>
#include <QResizeEvent>
#include <QCursor>

#include "smastericons.h"
#include "spicturewidget.h"




/*================================*
 |                                |
 |         SDrawerPrivate         |
 |                                |
 *================================*/

class SDrawerPrivate
{
public:
    SPictureWidget *picture;

    bool original_size_stt;
    bool fit_size_stt;
    int zoom_precent;

    int zoom_max;
    bool scroll_zoom_stt;

    QSize image_size;
    QSize start_zoom_size;

    QScrollBar *v_scroll;
    QScrollBar *h_scroll;

    bool mouse_presed;
    QPoint last_mouse_pos;

    void setupSizes( int width , int height );
};

void SDrawerPrivate::setupSizes( int width , int height )
{
    if( this->image_size.height()>height || this->image_size.width()>width )
    {
        double proportion = (double)this->image_size.height() / (double)this->image_size.width();

        if( height > width*proportion )
        {
            int h = width*proportion;

            this->start_zoom_size.setWidth( width );
            this->start_zoom_size.setHeight( h );
        }
        else
        {
            int w = height/proportion;

            this->start_zoom_size.setWidth( w );
            this->start_zoom_size.setHeight( height );
        }
    }
    else
    {
        this->start_zoom_size = this->image_size;
    }
}




/*================================*
 |                                |
 |            SDrawer             |
 |                                |
 *================================*/

SDrawer::SDrawer(QWidget *parent) :
    QScrollArea(parent)
{
    p = new SDrawerPrivate;
    p->mouse_presed = false;

    p->v_scroll = new QScrollBar();
        this->setVerticalScrollBar( p->v_scroll );
        this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    p->h_scroll = new QScrollBar();
        this->setHorizontalScrollBar( p->h_scroll );
        this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );


    p->picture = new SPictureWidget( this );
        p->picture->setKeepProportion( true );
        p->picture->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Expanding );
        p->picture->setButton( true );
        p->picture->setMouseOverOpacity( 0 );
        p->picture->setReloadAfterResize( 371 );

    this->setWidget( p->picture );
    this->setWidgetResizable( true );
    this->setAlignment( Qt::AlignCenter	 );
    this->setFrameShadow( QFrame::Plain );
    this->setFrameShape( QFrame::NoFrame );
    this->viewport()->setAutoFillBackground( false );

    p->fit_size_stt = true;
    p->original_size_stt = false;
    p->zoom_precent = 0;

    p->zoom_max = 500;
    p->scroll_zoom_stt = true;
}

void SDrawer::draw( const QString & file )
{
    QImageReader image( file );
    p->image_size = image.size();

    p->setupSizes( this->width() , this->height() );

    if( p->original_size_stt )
        this->setOriginalSize();
    else if( p->fit_size_stt )
        this->setFitSize();
    else
        this->zoom( p->zoom_precent );

    p->picture->drawFile( file );
}

void SDrawer::zoom( int percent )
{
    if( percent > p->zoom_max )
        percent = p->zoom_max;

    emit this->zoomed( percent );
    emit this->originalSize( false );
    emit this->fitSize( false );

    p->zoom_precent = percent;
    p->original_size_stt = false;
    p->fit_size_stt = false;



    double h_prop = (double)p->image_size.height() / p->start_zoom_size.height();
    if( h_prop == 1 )
        h_prop = 3;

    double zoom = 1+ ( h_prop -1 ) *(double)percent/50;
    int width  = p->start_zoom_size.width()*zoom,
        height = p->start_zoom_size.height()*zoom;


    if( p->zoom_precent > 0 )
        this->setCursor(QCursor(Qt::OpenHandCursor));

    if( height<1 || width<1 )
        return;


    QPoint local_pos = QCursor::pos() - p->picture->mapToGlobal(QPoint(0,0));
    QPoint new_pos( local_pos.x() * width / p->picture->width() ,
                    local_pos.y() * height/ p->picture->height()
                  );

    p->picture->setFixedSize( width , height );

    p->v_scroll->setValue( new_pos.y()*p->v_scroll->maximum() / height );
    p->h_scroll->setValue( new_pos.x()*p->h_scroll->maximum() / width );
}

void SDrawer::setOriginalSize()
{
    int original_size = 50;
    if( p->image_size.height() < this->height() )
        original_size = 0;

    emit this->zoomed( original_size );
    emit this->originalSize( true );
    emit this->fitSize( false );

    p->zoom_precent = original_size;
    p->original_size_stt = true;
    p->fit_size_stt = false;

    if( p->image_size.height()<1 || p->image_size.width()<1 )
        return;

    p->picture->setFixedSize( p->image_size );
}

void SDrawer::setFitSize()
{
    int fit_size = 0;

    emit this->zoomed( fit_size );
    emit this->originalSize( false );
    emit this->fitSize( true );

    p->zoom_precent = fit_size;
    p->original_size_stt = false;
    p->fit_size_stt = true;

    if( p->start_zoom_size.height()<1 || p->start_zoom_size.width()<1 )
        return;

    p->picture->setFixedSize( p->start_zoom_size );

    this->setCursor(QCursor(Qt::ArrowCursor));
}

int SDrawer::zoomPercent() const
{
    return p->zoom_precent;
}

void SDrawer::setMaximumZoom( int percent )
{
    p->zoom_max = percent;
}

int SDrawer::maximumZoom() const
{
    return p->zoom_max;
}

void SDrawer::setDisableScrollZoom( bool stt )
{
    this->setEnableScrollZoom( !stt );
}

void SDrawer::setEnableScrollZoom( bool stt )
{
    p->scroll_zoom_stt = stt;
}

bool SDrawer::disableScrollZoom() const
{
    return !this->enableScrollZoom();
}

bool SDrawer::enableScrollZoom() const
{
    return p->scroll_zoom_stt;
}

void SDrawer::wheelEvent( QWheelEvent *event )
{
    if( !p->scroll_zoom_stt )
        return ;

    int value = p->zoom_precent + event->delta()/60;
    if( value < 0 )
        return;

    this->zoom( value );
}

void SDrawer::mouseMoveEvent( QMouseEvent *event )
{
    QPoint point = p->last_mouse_pos - event->pos();

    p->v_scroll->setValue( p->v_scroll->value()+point.y() );
    p->h_scroll->setValue( p->h_scroll->value()+point.x() );

    p->last_mouse_pos = event->pos();
}

void SDrawer::mousePressEvent( QMouseEvent *event )
{
    p->last_mouse_pos = event->pos();
}

void SDrawer::resizeEvent( QResizeEvent *event )
{
    p->setupSizes( event->size().width() , event->size().height() );

    if( p->fit_size_stt )
        this->setFitSize();
    else
        this->viewport()->updateGeometry();
}

void SDrawer::mouseDoubleClickEvent( QMouseEvent * )
{
    emit this->fullscreenRequest();
}

SDrawer::~SDrawer()
{
    delete p;
}
