#define SHADOW_SIZE     7
#define BASE_COLOR_1    "#ffffff"
#define BASE_COLOR_2    "#dddddd"
#define MAGNIFIER_ALPHA 223
#define ROUNDED_PIXEL   5
#define POINTER_SIZE    QSize(10,10)
#define DEFAULT_HEIGHT  317
#define DEFAULT_WIDTH   273
#define SHADOW_COLOR    "#0d80ec"
#define PADDING         4

#include "sdialogpointer.h"

#include <QPainterPath>
#include <QPaintEvent>
#include <QSize>
#include <QPainter>
#include <QRect>
#include <QTimer>
#include <QApplication>
#include <QScrollArea>
#include <QDebug>

class SDialogPointerPrivate
{
public:
    enum Type{
        Left,
        Top,
        Right,
        Bottom
    };

    QPainterPath magnifierPath( const QSize & pntr , const QRect & rct , int padding, Type type = Left ) const;

    Type type;
    SDialogPointer::Type oriention_type;

    int pointer_transform;
    int radius;
    int alpha;
    int pad;

    QSize pointer_size;
    QPoint point_to;

    QWidget *widget;
    QScrollArea *area;
};

QPainterPath SDialogPointerPrivate::magnifierPath( const QSize & pntr , const QRect & rct , int padding , Type type ) const
{
    QPainterPath path;
        path.setFillRule( Qt::WindingFill );

    switch( static_cast<int>(type) )
    {
    case Left:
        /*! ====== Draw Left of the Magnifier ======= */
        path.moveTo( padding+pntr.width() , (rct.height()+pntr.width())/2+pointer_transform );
        path.lineTo( padding , rct.height()/2+pointer_transform );
        path.lineTo( padding+pntr.width() , (rct.height()-pntr.width())/2+pointer_transform );
        /*! ========================================= */
        /*! ====== Draw Right of the Magnifier ====== */
        path.lineTo( padding+pntr.width() , padding+radius );
        path.quadTo( padding+pntr.width() , padding , padding+radius+pntr.width() , padding );
        path.lineTo( rct.width()-padding-radius , padding );
        path.quadTo( rct.width()-padding , padding , rct.width()-padding , padding+radius );
        path.lineTo( rct.width()-padding , rct.height()-padding-radius );
        path.quadTo( rct.width()-padding , rct.height()-padding , rct.width()-padding-radius , rct.height()-padding );
        path.lineTo( padding+radius+pntr.width() , rct.height()-padding );
        path.quadTo( padding+pntr.width() , rct.height()-padding , padding+pntr.width() , rct.height()-padding-radius );
        path.lineTo( padding+pntr.width() , (rct.height()+pntr.width())/2+pointer_transform );
        /*! ========================================= */
        break;

    case Right:
        /*! ====== Draw Right of the Magnifier ====== */
        path.moveTo( rct.width()-padding-pntr.width() , (rct.height()-pntr.width())/2+pointer_transform );
        path.lineTo( rct.width()-padding , rct.height()/2+pointer_transform );
        path.lineTo( rct.width()-padding-pntr.width() , (rct.height()+pntr.width())/2+pointer_transform );
        /*! ========================================= */
        /*! ====== Draw Left of the Magnifier ======= */
        path.lineTo( rct.width()-padding-pntr.width() , rct.height()-padding-radius );
        path.quadTo( rct.width()-padding-pntr.width() , rct.height()-padding , rct.width()-padding-radius-pntr.width() , rct.height()-padding );
        path.lineTo( padding+radius , rct.height()-padding );
        path.quadTo( padding , rct.height()-padding , padding , rct.height()-padding-radius );
        path.lineTo( padding , padding+radius );
        path.quadTo( padding , padding , padding+radius , padding );
        path.lineTo( rct.width()-padding-pntr.width()-radius , padding );
        path.quadTo( rct.width()-padding-pntr.width() , padding , rct.width()-padding-pntr.width() , padding+radius );
        path.lineTo( rct.width()-padding-pntr.width() , (rct.height()-pntr.width())/2+pointer_transform );
        /*! ========================================= */
        break;

    case Top:
        /*! ======= Draw Top of the Magnifier ======= */
        path.moveTo( (rct.width()-pntr.width())/2+pointer_transform , padding+pntr.height() );
        path.lineTo( rct.width()/2+pointer_transform , padding );
        path.lineTo( (rct.width()+pntr.width())/2+pointer_transform , padding+pntr.height() );
        /*! ========================================= */
        /*! ===== Draw Bottom of the Magnifier ====== */
        path.lineTo( rct.width()-padding-radius , padding+pntr.height() );
        path.quadTo( rct.width()-padding , padding+pntr.height() , rct.width()-padding , padding+pntr.height()+radius );
        path.lineTo( rct.width()-padding , rct.height()-padding-radius );
        path.quadTo( rct.width()-padding , rct.height()-padding , rct.width()-padding-radius , rct.height()-padding );
        path.lineTo( padding+radius , rct.height()-padding );
        path.quadTo( padding , rct.height()-padding , padding , rct.height()-padding-radius );
        path.lineTo( padding , padding+pntr.height()+radius );
        path.quadTo( padding , padding+pntr.height() , padding+radius , padding+pntr.height() );
        path.lineTo( (rct.width()-pntr.width())/2+pointer_transform , padding+pntr.height() );
        /*! ========================================= */
        break;

    case Bottom:
        /*! ===== Draw Bottom of the Magnifier ====== */
        path.moveTo( (rct.width()+pntr.width())/2+pointer_transform , rct.height()-(padding+pntr.height()) );
        path.lineTo( rct.width()/2+pointer_transform , rct.height()-padding );
        path.lineTo( (rct.width()-pntr.width())/2+pointer_transform , rct.height()-(padding+pntr.height()) );
        /*! ========================================= */
        /*! ======= Draw Top of the Magnifier ======= */
        path.lineTo( padding+radius , rct.height()-padding-pntr.height() );
        path.quadTo( padding , rct.height()-padding-pntr.height() , padding , rct.height()-padding-radius-pntr.height() );
        path.lineTo( padding , padding+radius );
        path.quadTo( padding , padding , padding+radius , padding );
        path.lineTo( rct.width()-padding-radius , padding );
        path.quadTo( rct.width()-padding , padding , rct.width()-padding , padding+radius );
        path.lineTo( rct.width()-padding , rct.height()-padding-radius-pntr.height() );
        path.quadTo( rct.width()-padding , rct.height()-padding-pntr.height() , rct.width()-padding-radius , rct.height()-padding-pntr.height() );
        path.lineTo( (rct.width()+pntr.width())/2+pointer_transform , rct.height()-padding-pntr.height() );
        /*! ========================================= */
        break;

    }

    return path;
}




SDialogPointer::SDialogPointer(QWidget *parent) :
    QDialog(parent)
{
    p = new SDialogPointerPrivate;
        p->type = SDialogPointerPrivate::Bottom;
        p->pointer_transform = 0;
        p->radius = ROUNDED_PIXEL;
        p->alpha = MAGNIFIER_ALPHA;
        p->pointer_size = POINTER_SIZE;
        p->widget = 0;
        p->point_to = QPoint(0,0);
        p->oriention_type = Vertical;

    p->area = new QScrollArea( this );
        p->area->setWidgetResizable( true );
        p->area->setWidget( p->widget );
        p->area->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        p->area->setFrameShadow( QFrame::Plain );
        p->area->setFrameShape( QFrame::NoFrame );
        p->area->viewport()->setAutoFillBackground( false );
        p->area->setFocus();

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool );

    connect( static_cast<QApplication*>(QApplication::instance()) , SIGNAL(focusChanged(QWidget*,QWidget*)) , SLOT(focusChanged(QWidget*,QWidget*)) );
}

void SDialogPointer::setWidget( QWidget *widget )
{
    if( p->widget )
        p->widget->hide();

    p->widget = widget;

    if( widget )
        connect( p->widget , SIGNAL(destroyed()) , this , SLOT(widgetDestroyed()) );
    else
        disconnect( p->widget , SIGNAL(destroyed()) , this , SLOT(widgetDestroyed()) );

    if( p->widget )
        p->area->setWidget( p->widget );
}

QWidget *SDialogPointer::widget() const
{
    return p->widget;
}

const QPoint & SDialogPointer::pointedTo() const
{
    return p->point_to;
}

void SDialogPointer::pointTo( const QPoint & point )
{
    p->point_to = point;
    init_pos();
}

void SDialogPointer::setVisible( bool visible )
{
    QWidget::setVisible( visible );
    if( visible )
        p->area->setFocus();
}

void SDialogPointer::setType( SDialogPointer::Type type )
{
    p->oriention_type = type;
}

SDialogPointer::Type SDialogPointer::type() const
{
    return p->oriention_type;
}

void SDialogPointer::setRounded( int radius )
{
    p->radius = radius;
}

int SDialogPointer::rounded() const
{
    return p->radius;
}

void SDialogPointer::setAlpha( int alpha )
{
    p->alpha = alpha;
}

int SDialogPointer::alpha() const
{
    return p->alpha;
}

void SDialogPointer::setPointerSize( const QSize & size )
{
    p->pointer_size = size;
}

const QSize & SDialogPointer::pointerSize() const
{
    return p->pointer_size;
}

void SDialogPointer::setPadding( int pad )
{
    p->pad = pad;
}

int SDialogPointer::padding() const
{
    return p->pad;
}

void SDialogPointer::paintEvent( QPaintEvent * )
{
    const QRect & rct = rect();

    QPainter painter(this);
        painter.setRenderHint( QPainter::Antialiasing , true );

    QColor color1( BASE_COLOR_1 );
    QColor color2( BASE_COLOR_2 );
    QColor shadow_color( SHADOW_COLOR );

    color1.setAlpha( p->alpha );
    color2.setAlpha( p->alpha );

    QLinearGradient gradiant(QPointF(0, rct.y()), QPointF(0,rct.y()+rct.height()));
        gradiant.setColorAt(0, color1);
        gradiant.setColorAt(1, color2);

    painter.fillPath( p->magnifierPath( p->pointer_size , rct , SHADOW_SIZE , p->type ) , gradiant );

    for( int i=SHADOW_SIZE-1 ; i>=0 ; i-- )
    {
        int top_width_pad = 0;
        int top_height_pad = 0;
        switch( static_cast<int>(p->type) )
        {
        case SDialogPointerPrivate::Left:
        case SDialogPointerPrivate::Right:
            top_height_pad = 2*(SHADOW_SIZE-i);
            break;

        case SDialogPointerPrivate::Top:
        case SDialogPointerPrivate::Bottom:
            top_width_pad = 2*(SHADOW_SIZE-i);
            break;

        }

        QSize top_size( p->pointer_size.width() + top_width_pad , p->pointer_size.height() + top_height_pad );

        shadow_color.setAlpha( ((double)i/SHADOW_SIZE)*p->alpha/2 );

        painter.setPen( shadow_color );
        painter.drawPath( p->magnifierPath(top_size , rct , i+1 , p->type) );
    }
}

void SDialogPointer::resizeEvent( QResizeEvent * )
{
    QRect bot_rect;

    switch( static_cast<int>(p->type) )
    {
    case SDialogPointerPrivate::Left:
        bot_rect = QRect( p->pointer_size.width()+SHADOW_SIZE+ROUNDED_PIXEL+PADDING , SHADOW_SIZE+ROUNDED_PIXEL+PADDING ,
                                 width()-(p->pointer_size.width()+2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING)) , height()-2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING) );
        break;

    case SDialogPointerPrivate::Right:
        bot_rect = QRect( SHADOW_SIZE+ROUNDED_PIXEL+PADDING , SHADOW_SIZE+ROUNDED_PIXEL+PADDING ,
                                 width()-(p->pointer_size.width()+2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING)) , height()-2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING) );
        break;

    case SDialogPointerPrivate::Top:
        bot_rect = QRect( SHADOW_SIZE+ROUNDED_PIXEL+PADDING , p->pointer_size.width()+SHADOW_SIZE+ROUNDED_PIXEL+PADDING ,
                                 width()-2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING) , height()-(p->pointer_size.width()+2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING)) );
        break;

    case SDialogPointerPrivate::Bottom:
        bot_rect = QRect( SHADOW_SIZE+ROUNDED_PIXEL+PADDING , SHADOW_SIZE+ROUNDED_PIXEL+PADDING ,
                                 width()-2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING) , height()-(p->pointer_size.width()+2*(SHADOW_SIZE+ROUNDED_PIXEL+PADDING)) );
        break;

    }

    p->area->setGeometry( bot_rect );
}

void SDialogPointer::init_pos()
{
    QPoint result;

    switch(static_cast<int>(p->oriention_type))
    {
    case SDialogPointer::Horizontal:
        if( p->point_to.x() > width() )
        {
            p->type = SDialogPointerPrivate::Right;
            result = QPoint( p->point_to.x() - width() + SHADOW_SIZE ,
                             p->point_to.y() - height()/2 );
        }
        else
        {
            p->type = SDialogPointerPrivate::Left;
            result = QPoint( p->point_to.x() - SHADOW_SIZE ,
                             p->point_to.y() - height()/2 );
        }

        if( result.y() < 0 )
        {
            p->pointer_transform = result.y();
            result.setY( 0 );
        }
        else
            p->pointer_transform = 0;
        break;

    case SDialogPointer::Vertical:
        if( p->point_to.y() > height() )
        {
            p->type = SDialogPointerPrivate::Bottom;
            result = QPoint( p->point_to.x() - width()/2 ,
                             p->point_to.y() - height() + SHADOW_SIZE );
        }
        else
        {
            p->type = SDialogPointerPrivate::Top;
            result = QPoint( p->point_to.x() - width()/2 ,
                             p->point_to.y() - SHADOW_SIZE );
        }

        if( result.x() < 0 )
        {
            p->pointer_transform = result.x();
            result.setX( 0 );
        }
        else
            p->pointer_transform = 0;
        break;
    }

    move( result );
    resizeEvent(0);
}

void SDialogPointer::focusChanged( QWidget * , QWidget *now )
{
    QWidget *w = now;
    while( w && w != this )
        w = w->parentWidget();

    if( w != this )
    {
        if( p->widget )
        {
            p->widget->hide();
            p->widget->setParent( 0 );
        }

        close();
    }
}

void SDialogPointer::widgetDestroyed()
{
    p->widget = 0;
}

SDialogPointer::~SDialogPointer()
{
    delete p;
}
