/*
 * File:      silitab.cpp
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

#define SHADOW_SIZE     7
#define BASE_COLOR_1    "#ffffff"
#define BASE_COLOR_2    "#dddddd"
#define MAGNIFIER_ALPHA 173
#define ROUNDED_PIXEL   5
#define INPUT_SIZE      QSize(7,7)

#include "silitab.h"

#include <QColor>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QList>
#include <QHash>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QTimer>
#include <QDebug>

#include "sanimation.h"
#include "sstylesheet.h"
#include "swidgetpreview.h"




/*================================*
 |                                |
 |           Previewer            |
 |                                |
 *================================*/

class Previewer : public QWidget
{
public:
    Previewer( QWidget *parent = 0 );

    QPainterPath magnifierPath( const QSize & top , const QRect & rct , int padding ) const;

    SWidgetPreview *preview;

protected:
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
};

Previewer::Previewer( QWidget *parent ) : QWidget( parent )
{
    preview = new SWidgetPreview( this );
}

QPainterPath Previewer::magnifierPath( const QSize & top , const QRect & rct , int padding ) const
{
    QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        /*! ======= Draw Top of the Magnifier ======= */
        path.moveTo( (rct.width()-top.width())/2 , padding+top.height() );
        path.lineTo( rct.width()/2 , padding );
        path.lineTo( (rct.width()+top.width())/2 , padding+top.height() );
        /*! ========================================= */
        /*! ===== Draw Bottom of the Magnifier ====== */
        path.lineTo( rct.width()-padding-ROUNDED_PIXEL , padding+top.height() );
        path.quadTo( rct.width()-padding , padding+top.height() , rct.width()-padding , padding+top.height()+ROUNDED_PIXEL );
        path.lineTo( rct.width()-padding , rct.height()-padding-ROUNDED_PIXEL );
        path.quadTo( rct.width()-padding , rct.height()-padding , rct.width()-padding-ROUNDED_PIXEL , rct.height()-padding );
        path.lineTo( padding+ROUNDED_PIXEL , rct.height()-padding );
        path.quadTo( padding , rct.height()-padding , padding , rct.height()-padding-ROUNDED_PIXEL );
        path.lineTo( padding , padding+top.height()+ROUNDED_PIXEL );
        path.quadTo( padding , padding+top.height() , padding+ROUNDED_PIXEL , padding+top.height() );
        path.lineTo( (rct.width()-top.width())/2 , padding+top.height() );
        /*! ========================================= */

    return path;
}

void Previewer::paintEvent( QPaintEvent * )
{
    QSize input_size = INPUT_SIZE;
    const QRect & rct = rect();

    QPainter painter(this);
        painter.setRenderHint( QPainter::Antialiasing , true );

    QColor color1( BASE_COLOR_1 );
    QColor color2( BASE_COLOR_2 );
    QColor shadow_color( "#000000" );

    color1.setAlpha( MAGNIFIER_ALPHA );
    color2.setAlpha( MAGNIFIER_ALPHA );

    QLinearGradient gradiant(QPointF(0, rct.y()), QPointF(0,rct.y()+rct.height()));
        gradiant.setColorAt(0, color1);
        gradiant.setColorAt(1, color2);

    painter.fillPath( magnifierPath( input_size , rct , SHADOW_SIZE ) , gradiant );

    for( int i=SHADOW_SIZE-1 ; i>=0 ; i-- )
    {
        QSize top_size( input_size.width() + 2*(SHADOW_SIZE-i) , input_size.height() );

        shadow_color.setAlpha( ((double)i/SHADOW_SIZE)*MAGNIFIER_ALPHA/2 );

        painter.setPen( shadow_color );
        painter.drawPath( magnifierPath(top_size , rct , i) );
    }
}

void Previewer::resizeEvent( QResizeEvent * )
{
    QSize input_size = INPUT_SIZE;

    QRect bot_rect( SHADOW_SIZE+ROUNDED_PIXEL , input_size.height()+SHADOW_SIZE+ROUNDED_PIXEL ,
                   width()-2*(SHADOW_SIZE+ROUNDED_PIXEL) , height()-(input_size.height()+2*(SHADOW_SIZE+ROUNDED_PIXEL)) );

    preview->setGeometry( bot_rect );
}





/*================================*
 |                                |
 |         siliTabPrivate         |
 |                                |
 *================================*/

class siliTabPrivate
{
public:
    QHBoxLayout *layout;
    QHBoxLayout *baseLayout;
    QList<STabItem *> list;
    QHash<STabItem*,QWidget*> hash;

    SAnimation *animation;

    QString style;
    QString defaultStyle;
    QSize default_items_size;

    STabItem *current_item;
    bool at_spec_removing;          /*! This Special time happend when current item is at 0 postion
                                     *  and call remove(0) function. */

    Previewer *preview;
    int previewer_delay;
    int previewer_timer;
};




/*================================*
 |                                |
 |            siliTab             |
 |                                |
 *================================*/

siliTab::siliTab( QWidget *parent ) : QWidget( parent )
{
    p = new siliTabPrivate;

    p->preview = 0;
    p->previewer_delay = 1357;
    p->previewer_timer = 0;

    p->layout = new QHBoxLayout();
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->layout->setSpacing( 1 );

    p->baseLayout = new QHBoxLayout( this );
        p->baseLayout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->baseLayout->setSpacing( 0 );
        p->baseLayout->addLayout( p->layout );
        p->baseLayout->addSpacerItem( new QSpacerItem( 4 ,  2 ,  QSizePolicy::Expanding ,  QSizePolicy::Minimum ) );

    p->default_items_size = QSize( 137 , 26 );
    p->animation = 0;
    p->current_item = 0;

    p->at_spec_removing = false;

    setPreviewer( true );
    setAnimation( true );
}

STabItem *siliTab::addTab( const QString & name )
{
    return addTab( QIcon() , name );
}

STabItem *siliTab::addTab( const QIcon & icon , const QString & name )
{
    return insertTab( p->list.count() , icon , name );
}

STabItem *siliTab::insertTab( int index , const QString & name )
{
    return insertTab( index , QIcon() , name );
}

STabItem *siliTab::insertTab( int index , const QIcon & icon , const QString & name )
{
    if( isAnimated() && p->animation->isActive() )
        p->animation->finish();

    STabItem *item = new STabItem();
        item->setText( name );
        item->setIcon( icon );
        item->setFixedSize( 0 , 0 );
        item->setStyleSheet( p->defaultStyle );

    p->list.insert( index , item );
    p->layout->insertWidget( index , item );

    if( this->isAnimated() )
        p->animation->start( item , p->default_items_size , SAnimation::RESIZE , 10 , SAnimation::MIN_MAX );
    else
        this->anim_finished( item );


    connect( item , SIGNAL(clicked(STabItem*))      , SLOT(clicked_on_item(STabItem*)) );
    connect( item , SIGNAL(entered(STabItem*))      , SLOT(on_item(STabItem*))         , Qt::QueuedConnection );
    connect( item , SIGNAL(leaved(STabItem*))       , SLOT(close_on_item(STabItem*))   , Qt::QueuedConnection );
    connect( item , SIGNAL(closeRequest(STabItem*)) , SLOT(close_request(STabItem*))   );

    item->click();

    return item;
}

bool siliTab::removeTab( int index )
{
    if( isAnimated() && p->animation->isActive() )
        p->animation->finish();

    STabItem *item = p->list.at( index );

    if( p->current_item == item )
    {
        if( index == 0 && p->list.count() == 1 )
        {
            emit this->indexChange( -1 );
            p->current_item = 0;
        }
        if( index == 0 && p->list.count() != 1 )
        {
            p->at_spec_removing = true;
            p->list[ index+1 ]->click();
        }
        if( index != 0 )
            p->list[ index-1 ]->click();
    }

    close_on_item( item );
    pointTabToWidget( index , 0 );

    disconnect( item                , SLOT(click()) );
    disconnect( item->closeButton() , SLOT(click()) );

    p->preview->hide();

    if( this->isAnimated() )
        p->animation->start( item , QSize(0,0) , SAnimation::RESIZE , 10 , SAnimation::MIN_MAX );
    else
        anim_finished( item );

    return true;
}

void siliTab::moveTab( int from , int to )
{
    STabItem *item = p->list.at( from );

    p->layout->removeWidget( item );
    p->layout->insertWidget( to , item );
    p->list.move( from , to );

    emit this->moved( from , to );
}



QIcon siliTab::tabIcon( int index ) const
{
    return p->list.at( index )->icon();
}

QString siliTab::tabText( int index ) const
{
    return p->list.at( index )->text();
}

STabItem *siliTab::item( int index ) const
{
    return p->list.at( index );
}

int siliTab::index( STabItem *button ) const
{
    int index = p->list.indexOf( button );
    if( p->at_spec_removing && index > this->currentIndex() )
        index--;

    return index;
}

QString siliTab::styleSheet() const
{
    return p->style;
}

QSize siliTab::itemsSize() const
{
    return p->default_items_size;
}

int siliTab::itemsWidth() const
{
    return p->default_items_size.width();
}

int siliTab::itemsHeight() const
{
    return p->default_items_size.height();
}

int siliTab::currentIndex() const
{
    return p->list.indexOf( p->current_item ) - 1*p->at_spec_removing;
}



int siliTab::count() const
{
    return p->list.count() - 1*p->at_spec_removing;
}

void siliTab::setTabText( int index , const QString & text )
{
    p->list.at( index )->setText( text );
}

void siliTab::setTabIcon( int index , const QIcon & icon )
{
    p->list.at( index )->setIcon( icon );
}

void siliTab::setStyleSheet( const QString & styleSheet )
{
    p->style = styleSheet;
    SStyleSheet stl;
        stl.addStyle( "SiliTab" , styleSheet );
        stl.replace( "item" , "QPushButton" );

    p->defaultStyle = stl.filter( "QPushButton" );

    for( int i=0 ; i<p->list.count() ; i++ )
        p->list.at( i )->setStyleSheet( p->defaultStyle );
}

void siliTab::setItemsSize( const QSize & size )
{
    p->default_items_size = size;
    for( int i=0;i<p->list.count();i++ )
    {
        p->list.at( i )->setFixedSize( p->default_items_size );
    }
}

void siliTab::setItemsWidth( int width )
{
    p->default_items_size.setWidth( width );
    for( int i=0;i<p->list.count();i++ )
    {
        p->list.at( i )->setFixedWidth( width );
    }
}

void siliTab::setItemHeight( int height )
{
    p->default_items_size.setHeight( height );
    for( int i=0;i<p->list.count();i++ )
    {
        p->list.at( i )->setFixedHeight( height );
    }
}

void siliTab::setAnimation( bool stt )
{
    if( stt && p->animation == 0 )
    {
        p->animation = new SAnimation( this );
        QObject::connect( p->animation , SIGNAL(finished(QWidget*)) , SLOT(anim_finished(QWidget*)) );
    }
    else if( !stt && p->animation != 0 )
    {
        delete p->animation;
        p->animation = 0;
    }
}

bool siliTab::isAnimated() const
{
    return p->animation != 0;
}

void siliTab::setCurrentIndex( int index )
{
    p->list.at(index)->click();
}

void siliTab::unselectAll( bool stt )
{
    if( p->current_item == 0 )
        return ;

    p->current_item->setChecked( !stt );
}

void siliTab::setPreviewer( bool stt )
{
    if( !stt && p->preview )
    {
        delete p->preview;
        p->preview = 0;
    }
    else if( stt && !p->preview )
    {
        p->preview = new Previewer( this );
            p->preview->resize( 237 , 157 );
            p->preview->setAttribute(Qt::WA_TranslucentBackground);
            p->preview->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
            p->preview->hide();
    }
}

bool siliTab::previewer() const
{
    return p->preview;
}

void siliTab::setPreviewerDelay( int ms )
{
    p->previewer_delay = ms;
}

int siliTab::previewerDelay() const
{
    return p->previewer_delay;
}

void siliTab::pointTabToWidget( int index , QWidget *widget )
{
    STabItem *item = p->list.at(index);
    if( widget )
        p->hash.insert( item , widget );
    else if( p->hash.contains(item) )
        p->hash.remove( item );
}

QWidget *siliTab::pointedWidgetToTab( int index ) const
{
    return p->hash.value( p->list.at(index) );
}

void siliTab::anim_finished( QWidget *widget )
{
    STabItem *item = static_cast<STabItem *>(widget);
    bool removed = item->size() == QSize(0,0);

    if( !this->isAnimated() )
        removed = !removed;

    if( removed )
    {
        p->list.removeOne( item );
        p->layout->removeWidget( item );
        delete item;

        if( p->list.isEmpty() )
            emit this->emptied();

        p->at_spec_removing = false;
    }
    else
    {
        item->setFixedSize( p->default_items_size );
        emit this->tabAdded( p->list.indexOf(item) );
    }
}

void siliTab::clicked_on_item( STabItem *item )
{
    killTimer( p->previewer_timer );
    p->previewer_timer = 0;
    p->preview->hide();

    if( p->current_item != 0 )
        p->current_item->setChecked( false );

    if( p->current_item == item )
    {
        item->setChecked( true );

        if( isAnimated() && p->animation->isActive() )
            return ;
    }

    p->current_item = item;

    emit this->itemChange( item );
    emit this->indexChange( p->list.indexOf(item) - 1*p->at_spec_removing );
}

void siliTab::on_item( STabItem *item )
{
    if( !previewer() || item->isChecked() )
        return;

    const QPoint & point = mapToGlobal(item->pos());

    p->preview->preview->setWidget( p->hash.value(item) );
    p->preview->move( point.x() - (p->preview->width()-item->width())/2 , point.y() + item->height() - SHADOW_SIZE );

    if( p->previewer_timer )
        killTimer( p->previewer_timer );

    p->previewer_timer = startTimer( p->previewer_delay );
}

void siliTab::close_on_item( STabItem *item )
{
    if( !previewer() )
        return;

    if( p->preview->preview->widget() != p->hash.value(item) )
        return;

    if( p->previewer_timer )
        killTimer( p->previewer_timer );

    p->preview->preview->setWidget( 0 );
    p->preview->hide();
}

void siliTab::close_request( STabItem *item )
{
    emit this->closeRequest( p->list.indexOf(item) );
}

void siliTab::timerEvent( QTimerEvent * )
{
    killTimer( p->previewer_timer );
    p->previewer_timer = 0;

    p->preview->show();
}

siliTab::~siliTab()
{
    delete p;
}
