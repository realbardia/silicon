#define TIMER_STEP 40

#include "sgraphictimer.h"
#include "scolor.h"

#include <QRect>
#include <QPainter>
#include <QTimer>
#include <QPointF>
#include <QFont>
#include <QRadialGradient>

class SGraphicTimerPrivate
{
public:
    int  steps;
    bool play;
};

SGraphicTimer::SGraphicTimer( QWidget *parent ) :
    QWidget(parent)
{
    p = new SGraphicTimerPrivate;
        p->steps = 0;
        p->play  = false;
}

void SGraphicTimer::start( int milisecond )
{
    if( milisecond < 0 )
    {
        p->steps = 0;
        p->play  = false;

        emit finished();
        emit finished( true );

        return ;
    }

    p->steps = milisecond;
    p->play  = true;

    QTimer::singleShot( TIMER_STEP , this , SLOT(timer_shot()) );
}

void SGraphicTimer::stop()
{
    p->play = false;
    update();

    emit finished( false );
}

void SGraphicTimer::timer_shot()
{
    update();
    if( !p->play )
        return;

    start( p->steps - TIMER_STEP );
}

void SGraphicTimer::paintEvent( QPaintEvent * )
{
    double ratio = p->steps % 1000;

    QRect rct = rect();
    if( rct.width() > rct.height() )
    {
        rct.setX( (rct.width() - rct.height()) / 2 );
        rct.setWidth( rct.height() );
    }
    else if( rct.width() < rct.height() )
    {
        rct.setY( (rct.height() - rct.width()) / 2 );
        rct.setHeight( rct.width() );
    }

    QColor color1( palette().highlight().color() );
    QColor color2( palette().highlight().color() );
    SColor color3( palette().highlight().color() );
    QColor color4( palette().highlight().color() );

    color1.setAlpha( 73  );
    color2.setAlpha( 255 );
    color3.setAlpha( 73  );
    color4.setAlpha( 0   );
    color3.invert();

    QPointF center( rct.x() + rct.width()/2 , rct.y() + rct.height()/2 );

    QLinearGradient gradiant(QPointF(0,0), QPointF(rct.width(),rct.height()));
        gradiant.setColorAt(0, color1);
        gradiant.setColorAt(1, color2);

    QRadialGradient text_shadow( center , 100 );
        text_shadow.setColorAt(0, color3);
        text_shadow.setColorAt(1, color4);

    int radius = -360 * (1000 - ratio ) / 1000;
    QPainterPath cyrcle_path;
        cyrcle_path.moveTo( center );
        cyrcle_path.arcTo( rct, 90, radius );

    QPainterPath shadow_path;
        shadow_path.moveTo( center );
        shadow_path.arcTo( rct, 90, 360 );

    QFont font = this->font();
        font.setWeight( 20 );
        font.setPointSize( rct.height() / 2 );

    QPainter painter(this);
        painter.setRenderHint( QPainter::Antialiasing );
        painter.fillPath( cyrcle_path , gradiant );
        //painter.fillPath( shadow_path , text_shadow );
        painter.setPen( palette().highlightedText().color() );
        painter.setFont( font );
        painter.drawText( rct , Qt::AlignCenter | Qt::AlignVCenter , QString::number( p->steps / 1000 ) );
}

SGraphicTimer::~SGraphicTimer()
{
    delete p;
}
