#define SHADOW_HEIGHT    4
#define MAX_SHADOW_ALPHA 173
#define MAXIMUM_HEIGHT   44

#include "toolbarmanager.h"

#include <math.h>

#include <QVBoxLayout>
#include <QLinearGradient>
#include <QPainter>
#include <QToolBar>

#include <SColor>
#include <SAnimation>
#include <SPage>

class ToolBarManagerPrivate
{
public:
    QToolBar *current;
    SPage    *current_page;

    QVBoxLayout *layout;

    SAnimation *animation;
    bool added_to_layout;
};

ToolBarManager::ToolBarManager(QWidget *parent) :
    QWidget(parent)
{
    p = new ToolBarManagerPrivate;
    p->current         = 0;
    p->added_to_layout = false;

    p->animation = new SAnimation( this );
        p->animation->setFrameRate( 5 );
        p->animation->setFixOptions( SAnimation::WIDTH_FIX );

    p->layout = new QVBoxLayout( this );
        p->layout->setContentsMargins( 0 , 0 , 0 , SHADOW_HEIGHT );

    connect( p->animation , SIGNAL(finished()) , SLOT(animation_finished()) );
}

QToolBar *ToolBarManager::current() const
{
    return p->current;
}

int ToolBarManager::panelHeight() const
{
    return height() - SHADOW_HEIGHT;
}

void ToolBarManager::setCurrent( SPage *page , QToolBar *toolbar )
{
    if( p->current != 0 && p->added_to_layout )
    {
        p->layout->takeAt(0)->widget()->hide();
        p->added_to_layout = false;
    }

    p->current = toolbar;
    if( p->current == 0 )
    {
        p->layout->setContentsMargins( 0 , 0 , 0 , SHADOW_HEIGHT );
        p->animation->start( this , QSize(0,SHADOW_HEIGHT) , SAnimation::RESIZE , 10 , SAnimation::MIN_MAX );
        return;
    }

    p->current_page = page;
        p->current_page->setUpdatesEnabled( false );

    p->current->hide();
    p->layout->setContentsMargins( 4 , 4 , 4 , SHADOW_HEIGHT + 4 );

    p->animation->start( this , QSize(0,MAXIMUM_HEIGHT) , SAnimation::RESIZE , 10 , SAnimation::MIN_MAX );

}

void ToolBarManager::animation_finished()
{
    if( p->current == 0 )
        return;

    p->layout->addWidget( p->current );
    p->added_to_layout = true;

    p->current->show();
    p->current_page->setUpdatesEnabled( true );
    p->current_page->update();
}

void ToolBarManager::setAnimation( bool stt )
{
    p->animation->setFrameRate( 4*stt + 1 );
}

bool ToolBarManager::animation() const
{
    return p->animation->frameRate() == 5;
}

void ToolBarManager::paintEvent( QPaintEvent * )
{
    QPainter painter(this);

    QRect rct = rect();
        rct.setY( rct.height() - SHADOW_HEIGHT );
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

        QLinearGradient gradiant(QPointF(0,rct.y()), QPointF(0,rct.y()+SHADOW_HEIGHT));
            gradiant.setColorAt(0, color1);
            gradiant.setColorAt(1, color2);

        painter.fillRect( i , rct.y() , 1 , rct.height() , gradiant );
    }
}

ToolBarManager::~ToolBarManager()
{
    delete p;
}
