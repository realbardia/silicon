#define SHADOW_HEIGHT    4
#define MAX_SHADOW_ALPHA 173
#define MAXIMUM_HEIGHT   26

#include "statusbarmanager.h"

#include <math.h>

#include <QVBoxLayout>
#include <QLinearGradient>
#include <QPainter>
#include <QStatusBar>

#include <SColor>

class StatusBarManagerPrivate
{
public:
    QStatusBar *current;
    QVBoxLayout *layout;
};

StatusBarManager::StatusBarManager(QWidget *parent) :
    QWidget(parent)
{
    p = new StatusBarManagerPrivate;
    p->current = 0;

    p->layout = new QVBoxLayout( this );
        p->layout->setContentsMargins( 0 , SHADOW_HEIGHT , 0 , 0 );
}

QStatusBar *StatusBarManager::current() const
{
    return p->current;
}

int StatusBarManager::panelHeight() const
{
    return height() - SHADOW_HEIGHT;
}

void StatusBarManager::setCurrent( QStatusBar *widget )
{
    if( p->current != 0 )
        p->layout->takeAt(0)->widget()->hide();

    p->current = widget;
    if( p->current == 0 )
    {
        p->layout->setContentsMargins( 0 , SHADOW_HEIGHT , 0 , 0 );
        return;
    }

    p->layout->setContentsMargins( 4 , SHADOW_HEIGHT+4 , 4 , 4 );
    p->layout->addWidget( p->current );

    p->current->show();
}

void StatusBarManager::paintEvent( QPaintEvent * )
{
    QPainter painter(this);

    QRect rct = rect();
        rct.setY( 0 );
        rct.setHeight( SHADOW_HEIGHT );

    SColor color1( palette().window().color() );
        color1.invert();
        color1.setAlpha(173);

    SColor color2( palette().window().color() );
        color2.invert();
        color2.setAlpha(0);

    for( int i=0 ; i<rct.width() ; i++ )
    {
        double ratio = 3.1415 * i / rct.width();

        color1.setAlpha( MAX_SHADOW_ALPHA * sin(ratio) );
        color2.setAlpha(0);

        QLinearGradient gradiant(QPointF(0,rct.y()+SHADOW_HEIGHT), QPointF(0,rct.y()));
            gradiant.setColorAt(0, color1);
            gradiant.setColorAt(1, color2);

        painter.fillRect( i , rct.y() , 1 , rct.height() , gradiant );
    }
}

StatusBarManager::~StatusBarManager()
{
    delete p;
}
