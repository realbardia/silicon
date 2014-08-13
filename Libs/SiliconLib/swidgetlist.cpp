#include "swidgetlist.h"
#include "swidgetlist_prev.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOptionViewItem>
#include <QStyle>


/*================================*
 |                                |
 |      SWidgetListItemPrivate    |
 |                                |
 *================================*/

class SWidgetListItemPrivate
{
public:
    QHBoxLayout *layout;

    bool  check_stt;
    bool  mouse_press;
    bool  on_moving;

    int mouse_first_y;
};




/*================================*
 |                                |
 |         SWidgetListItem        |
 |                                |
 *================================*/

SWidgetListItem::SWidgetListItem( QWidget *widget ) :
    QWidget()
{
    p = new SWidgetListItemPrivate;
        p->check_stt   = false;
        p->mouse_press = false;
        p->on_moving   = false;

    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( widget );
}

bool SWidgetListItem::isChecked() const
{
    return p->check_stt;
}

void SWidgetListItem::setChecked( bool stt )
{
    if( stt == p->check_stt )
        return;

    p->check_stt = stt;
    emit checked( stt );

    if( stt )
        emit selected( this );

    repaint();
}

void SWidgetListItem::enterEvent( QEvent *event )
{
    QWidget::enterEvent( event );
    repaint();
}

void SWidgetListItem::leaveEvent( QEvent *event )
{
    QWidget::leaveEvent( event );
    repaint();
}

void SWidgetListItem::paintEvent( QPaintEvent * )
{
    QPainter painter(this);

    QStyleOptionViewItemV4 option;
        option.initFrom(this);

    if( p->check_stt )
        option.state |= QStyle::State_Selected;

    style()->drawControl(QStyle::CE_ItemViewItem, &option, &painter, this );
}

void SWidgetListItem::mousePressEvent( QMouseEvent * )
{
    p->mouse_press   = true;
    p->mouse_first_y = mapFromGlobal( QCursor::pos() ).y();
    raise();
}

void SWidgetListItem::mouseReleaseEvent( QMouseEvent * )
{
    if( p->on_moving )
    {
        p->on_moving = false;
        emit movingFinished( this );
        return;
    }

    if( !isChecked() && rect().contains( mapFromGlobal(QCursor::pos()) ) )
        setChecked( true );

    p->mouse_press = false;

    emit clicked( this );
}

void SWidgetListItem::mouseMoveEvent( QMouseEvent *event )
{
    if( !p->mouse_press )
        return;

    p->on_moving = true;

    move( 0 , event->y() + mapToParent(QPoint()).y() - p->mouse_first_y );

    emit moved( this );
}

void SWidgetListItem::mouseDoubleClickEvent( QMouseEvent *event )
{
    QWidget::mouseDoubleClickEvent( event );
    emit doubleClicked( this );
}

SWidgetListItem::~SWidgetListItem()
{
    delete p;
}




/*================================*
 |                                |
 |      ViewPortWidgetPrivate     |
 |                                |
 *================================*/

class ViewPortWidgetPrivate
{
public:
    ViewPortWidgetPrivate( const QList<SWidgetListItem*> & l ) : list(l) {}

    const QList<SWidgetListItem*> & list;
};




/*================================*
 |                                |
 |         ViewPortWidget         |
 |                                |
 *================================*/

ViewPortWidget::ViewPortWidget( const QList<SWidgetListItem*> & list , QWidget *parent ) :
    QWidget(parent)
{
    p = new ViewPortWidgetPrivate( list );
}

void ViewPortWidget::paintEvent(QPaintEvent *)
{
    QPalette palette = QWidget::palette();
        palette.setColor( QPalette::Window     , palette.base().color() );
        palette.setColor( QPalette::WindowText , palette.text().color() );

    QPainter painter( this );
        painter.fillRect( rect() , palette.base() );
}

void ViewPortWidget::mouseMoveEvent( QMouseEvent * )
{
}

void ViewPortWidget::mouseReleaseEvent( QMouseEvent * )
{
    emit clicked();
}

ViewPortWidget::~ViewPortWidget()
{
    delete p;
}




/*================================*
 |                                |
 |       SWidgetListPrivate       |
 |                                |
 *================================*/

class SWidgetListPrivate
{
public:
    ViewPortWidget *base;
    QVBoxLayout *layout;

    SWidgetListItem *current;

    QList<SWidgetListItem*> list;
    QList<QWidget*>         widgets_list;
};




/*================================*
 |                                |
 |           SWidgetList          |
 |                                |
 *================================*/

SWidgetList::SWidgetList(QWidget *parent) :
    QScrollArea(parent)
{
    p = new SWidgetListPrivate;
    p->current = 0;

    p->base = new ViewPortWidget( p->list );

    p->layout = new QVBoxLayout( p->base );
        p->layout->addStretch();
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->layout->setSpacing( 0 );

    setSizePolicy( QSizePolicy::MinimumExpanding , QSizePolicy::MinimumExpanding );
    setWidgetResizable( true );
    setWidget( p->base );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    connect( p->base , SIGNAL(clicked()) , SLOT(deselectAll()) );
}

void SWidgetList::addWidget( QWidget *widget )
{
    insertWidget( count() , widget );
}

void SWidgetList::insertWidget( int i , QWidget *widget )
{
    SWidgetListItem *item = new SWidgetListItem( widget );

    connect( item , SIGNAL(clicked(SWidgetListItem*))        , SLOT(itemClicked(SWidgetListItem*))       );
    connect( item , SIGNAL(selected(SWidgetListItem*))       , SLOT(itemSelected(SWidgetListItem*))      );
    connect( item , SIGNAL(moved(SWidgetListItem*))          , SLOT(itemMoved(SWidgetListItem*))         );
    connect( item , SIGNAL(movingFinished(SWidgetListItem*)) , SLOT(movingFinished(SWidgetListItem*))    );
    connect( item , SIGNAL(doubleClicked(SWidgetListItem*))  , SLOT(itemDoubleClicked(SWidgetListItem*)) );

    p->layout->insertWidget( i , item );
    p->list.insert( i , item );
    p->widgets_list.insert( i , widget );
}

void SWidgetList::remove( QWidget *widget )
{
    removeAt( p->widgets_list.indexOf(widget) );
}

void SWidgetList::removeAt( int index )
{
    p->widgets_list.at( index )->setParent( 0 );

    delete p->layout->takeAt( index );
    delete p->list.at( index );

    SWidgetListItem *item = p->list.takeAt( index );
    p->widgets_list.removeAt( index );

    if( p->current == item )
        p->current = 0;
}

void SWidgetList::clear()
{
    while( !isEmpty() )
        removeAt( 0 );
}

int SWidgetList::indexOf( QWidget *widget ) const
{
    return p->widgets_list.indexOf( widget );
}

QWidget *SWidgetList::widget( int index ) const
{
    return p->widgets_list.at( index );
}

QWidget *SWidgetList::current() const
{
    return p->widgets_list.at( currentIndex() );
}

int SWidgetList::currentIndex() const
{
    return p->list.indexOf( p->current );
}

int SWidgetList::count() const
{
    return p->list.count();
}

int SWidgetList::isEmpty() const
{
    return p->list.isEmpty();
}

void SWidgetList::setSpacing( int a )
{
    p->layout->setSpacing( a );
}

int SWidgetList::spacing() const
{
    return p->layout->spacing();
}

void SWidgetList::setSelected( QList<QWidget*> list )
{
    for( int i=0 ; i<p->list.count() ; i++ )
    {
        SWidgetListItem *item = p->list.at(i);
            item->setChecked( list.contains(p->widgets_list.at(i)) );
    }
}

QList<QWidget*> SWidgetList::currentSelected() const
{
    QList<QWidget*> result;
    for( int i=0 ; i<p->list.count() ; i++ )
    {
        SWidgetListItem *item = p->list.at(i);
        if( !item->isChecked() )
            continue;

        result << p->widgets_list.at(i);
    }

    return result;
}

void SWidgetList::deselectAll()
{
    if( !p->list.isEmpty() && p->current != 0 )
        p->current->setChecked( false );

    p->current = 0;
}

void SWidgetList::click( int index )
{
    itemClicked( p->list.at( index ) );
}

void SWidgetList::click( QWidget *widget )
{
    click( p->widgets_list.indexOf(widget) );
}

void SWidgetList::doubleClick( int index )
{
    itemDoubleClicked( p->list.at( index ) );
}

void SWidgetList::doubleClick( QWidget *widget )
{
    doubleClick( p->widgets_list.indexOf(widget) );
}

void SWidgetList::itemClicked( SWidgetListItem *item )
{
    int index = p->list.indexOf( item );

    emit clicked( index );
    emit clicked( widget(index) );
}

void SWidgetList::itemSelected( SWidgetListItem *item )
{
    if( !p->list.isEmpty() && p->current != 0 )
        p->current->setChecked( false );

    p->current = item;
    int index = p->list.indexOf( item );

    ensureWidgetVisible( item );
    emit selected( widget(index) );
    emit rowChanged( index );
}

void SWidgetList::itemDoubleClicked( SWidgetListItem *item )
{
    int index = p->list.indexOf( item );
    emit doubleClicked( widget(index) );
}

void SWidgetList::itemMoved( SWidgetListItem *item )
{
    int y = item->y() + item->height()/2;

    if( p->list.contains( item ) )
    {
        int index = p->list.indexOf(item);
        p->list.replace( index , 0 );
        p->layout->removeWidget( item );
        p->layout->insertSpacing( index , item->height() + spacing() );
    }

    int index = p->list.indexOf(0);

    SWidgetListItem *on_item;
    for( int i=0 ; i<p->list.count() ; i++ )
    {
        on_item = p->list.at(i);
        if( on_item == 0     ||
            y < on_item->y() ||
            y > on_item->y() + on_item->height() )
            continue;

        delete p->layout->takeAt( index );

        p->layout->insertWidget( index-(index>i) , p->layout->takeAt( i-(index<i) )->widget() );
        p->layout->insertSpacing( i , item->height() + spacing() );

        p->list.swap( index , i );
        p->widgets_list.swap( index , i );
        emit moved( index , i );

        break;
    }
}

void SWidgetList::movingFinished( SWidgetListItem *item )
{
    if( !p->list.contains(0) )
        return;

    int index = p->list.indexOf(0);
    delete p->layout->takeAt( index );

    p->list.replace( index , item );
    p->layout->insertWidget( index , item );
}

void SWidgetList::resizeEvent( QResizeEvent *event )
{
    QWidget::resizeEvent( event );
    p->base->setFixedWidth(  event->size().width() );
    p->base->resize( event->size().width() , event->size().height() );
}

SWidgetList::~SWidgetList()
{
    delete p;
}
