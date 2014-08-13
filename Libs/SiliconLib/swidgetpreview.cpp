#include "swidgetpreview.h"

#include <QTimerEvent>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

class SWidgetPreviewPrivate
{
public:
    QWidget *widget;
    int monitoring;
    int timer;

    QPixmap pixmap;
};

SWidgetPreview::SWidgetPreview(QWidget *parent) :
    QWidget(parent)
{
    p = new SWidgetPreviewPrivate;
    p->widget = 0;
    p->monitoring = 137;
    p->timer = -1;
}

void SWidgetPreview::setWidget( QWidget *widget )
{
    if( p->widget )
        disconnect( p->widget , SIGNAL(destroyed()) , this , SLOT(widgetDestroyed()) );

    if( !widget )
    {
        setMonitoring( 0 );
        p->pixmap = QPixmap();
        p->widget = widget;
        update();
        return;
    }

    p->widget = widget;
    connect( p->widget , SIGNAL(destroyed()) , this , SLOT(widgetDestroyed()) );

    timerEvent( 0 );
    setMonitoring( p->monitoring );
}

QWidget *SWidgetPreview::widget() const
{
    return p->widget;
}

void SWidgetPreview::setMonitoring( int ms )
{
    if( p->timer != -1 )
    {
        killTimer( p->timer );
        p->timer = -1;
    }

    p->monitoring = ms;

    if( ms <= 0 )
        return;

    p->timer = startTimer( ms );
}

int SWidgetPreview::monitoring() const
{
    return p->monitoring;
}

void SWidgetPreview::timerEvent( QTimerEvent * )
{
    if( !p->widget )
        return;

    p->pixmap = QPixmap::grabWidget( p->widget );
    update();
}

void SWidgetPreview::paintEvent( QPaintEvent *event )
{
    const QRect   & rct    = rect();
    const QPixmap & pixmap = p->pixmap.scaled( rct.size() , Qt::IgnoreAspectRatio , Qt::SmoothTransformation );

    QPainter painter( this );
    if( p->pixmap.isNull() )
    {
        painter.eraseRect( rct );
        return;
    }

    painter.setRenderHint( QPainter::SmoothPixmapTransform , true );
    painter.drawPixmap( event->rect() , pixmap , event->rect() );
}

void SWidgetPreview::widgetDestroyed()
{
    setWidget( 0 );
}

SWidgetPreview::~SWidgetPreview()
{
    delete p;
}
