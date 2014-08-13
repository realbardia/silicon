#include "sautohidewidget.h"

#include <QResizeEvent>
#include <QTimer>

class SAutoHideWidgetPrivate
{
public:
    QWidget *widget;
    QTimer  *timer;

    int  timer_value;
    bool active;
};

SAutoHideWidget::SAutoHideWidget(QWidget *parent) :
    QWidget(parent)
{
    p = new SAutoHideWidgetPrivate;
    p->widget = 0;
    p->timer_value  = 0;
    p->active = true;

    p->timer = new QTimer( this );
        p->timer->setSingleShot( true );
}

void SAutoHideWidget::setWidget( QWidget *widget )
{
    if( p->widget != 0 )
        disconnect( p->widget , SIGNAL(destroyed()) , this , SLOT(widgetDestroyed()) );

    p->widget = widget;
    if( p->widget == 0 )
        return;

    p->widget->setParent( this );

    connect( p->widget , SIGNAL(destroyed()) , this      , SLOT(widgetDestroyed()) );
    connect( p->timer  , SIGNAL(timeout())   , p->widget , SLOT(hide())            );
}

QWidget *SAutoHideWidget::widget() const
{
    return p->widget;
}

void SAutoHideWidget::setHiddenTimer( int ms )
{
    p->timer_value = ms;
}

int SAutoHideWidget::hiddenTimer() const
{
    return p->timer_value;
}

void SAutoHideWidget::setActive( bool stt )
{
    p->active = stt;

    if( !p->active && p->widget != 0 )
        p->widget->show();
}

void SAutoHideWidget::setDeactive( bool stt )
{
    setActive( !stt );
}

bool SAutoHideWidget::isActive() const
{
    return p->active;
}

bool SAutoHideWidget::isDeactive() const
{
    return !isActive();
}

void SAutoHideWidget::showWidget()
{
    if( p->widget == 0 )
        return;

    p->widget->show();

    if( p->active )
        p->timer->start( p->timer_value );
}

void SAutoHideWidget::hideWidget()
{
    if( p->widget == 0 || p->active )
        return;

    p->widget->hide();
}

void SAutoHideWidget::resizeEvent( QResizeEvent *event )
{
    if( p->widget == 0 )
        return;

    p->widget->setGeometry( 0 , 0 , event->size().width() , event->size().height() );
}

void SAutoHideWidget::enterEvent( QEvent * )
{
    if( p->widget == 0 || !p->active )
        return;

    p->timer->stop();
    p->widget->show();
}

void SAutoHideWidget::leaveEvent( QEvent * )
{
    if( p->widget == 0 || !p->active )
        return;

    p->timer->start( p->timer_value );
}

void SAutoHideWidget::widgetDestroyed()
{
    p->widget = 0;
}

SAutoHideWidget::~SAutoHideWidget()
{
    disconnect( p->widget , SIGNAL(destroyed()) , this , SLOT(widgetDestroyed()) );
    delete p;
}
