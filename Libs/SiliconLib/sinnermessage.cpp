#include "sinnermessage.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QColor>
#include <QPalette>
#include <QPainter>

#include "sanimation.h"
#include "scolor.h"

class SInnerMessagePrivate
{
public:
    QMessageBox *m_box;
    QFrame *base_frame;

    QGridLayout *layout;
    QHBoxLayout *base_layout;

    SAnimation *animation;

    QColor color;
    bool remove_in_deconstructor;
};

SInnerMessage::SInnerMessage( QMessageBox *messageBox , bool removeInDeconstructor , QWidget *parent ) :
    QWidget(parent)
{
    p = new SInnerMessagePrivate;
        p->remove_in_deconstructor = removeInDeconstructor;
        p->m_box = messageBox;
        p->m_box->hide();
        p->m_box->setStyleSheet( "QFrame{ background-color: transparent; }" );

    p->animation = 0;

    p->base_frame = new QFrame();
        p->base_frame->hide();
        p->base_frame->setStyleSheet(
            "QFrame{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
            "stop:0.136364 rgba(232, 232, 232, 255), stop:1 rgba(255, 255, 255, 255));"
            "border-radius: 6px;}" );

    p->base_layout = new QHBoxLayout( p->base_frame );
        p->base_layout->addWidget( messageBox );

    p->layout = new QGridLayout( this );
        p->layout->addItem( new QSpacerItem(4,2 , QSizePolicy::Minimum , QSizePolicy::Expanding) , 0 , 1 );
        p->layout->addItem( new QSpacerItem(4,2 , QSizePolicy::Expanding , QSizePolicy::Minimum) , 1 , 0 );
        p->layout->addItem( new QSpacerItem(4,2 , QSizePolicy::Expanding , QSizePolicy::Minimum) , 1 , 2 );
        p->layout->addItem( new QSpacerItem(4,2 , QSizePolicy::Minimum , QSizePolicy::Expanding) , 2 , 1 );
        p->layout->addWidget( p->base_frame , 1 , 1 );

    this->setAnimation( true );
}

void SInnerMessage::setAnimation( bool stt )
{
    if( !stt && p->animation != 0 )
    {
        delete p->animation;
        p->animation = 0;
    }
    else if( stt && p->animation == 0 )
    {
        p->animation = new SAnimation( this );
        QObject::connect( p->animation , SIGNAL(finished()) , SLOT(animation_finished()) );
    }
}

bool SInnerMessage::animation() const
{
    return p->animation != 0;
}

void SInnerMessage::showMessage( const QSize & size )
{
    if( this->animation() )
    {
        p->m_box->hide();
        p->base_frame->setFixedSize( 0 , 0 );
        p->base_frame->show();

        p->animation->start( p->base_frame , size , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
    }
    else
    {
        p->base_frame->setFixedSize( size );
        p->base_frame->show();
        p->m_box->show();
    }
}

void SInnerMessage::hideMessage()
{
    if( this->animation() )
    {
        p->m_box->hide();
        p->animation->start( p->base_frame , QSize(0,0) , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
    }
    else
    {
        p->base_frame->setFixedSize( 0,0 );
        p->base_frame->hide();
        p->m_box->hide();
    }
}

void SInnerMessage::setColor( const QColor & color )
{
    p->color = color;

    SColor invert_color( p->color );
        invert_color.invert();
        invert_color.setAlpha( 255 );

    QPalette palette = this->palette();
        palette.setColor( QPalette::Window     , color        );
        palette.setColor( QPalette::WindowText , invert_color );

    this->setPalette( palette );
}

const QColor & SInnerMessage::color() const
{
    return p->color;
}

QMessageBox *SInnerMessage::messageBox() const
{
    return p->m_box;
}

void SInnerMessage::animation_finished()
{
    if( p->base_frame->size() == QSize(0,0) )
        p->m_box->hide();
    else
        p->m_box->show();
}

void SInnerMessage::paintEvent( QPaintEvent *event )
{
    QPainter painter(this);
        painter.setRenderHint( QPainter::SmoothPixmapTransform , true );
        painter.fillRect( event->rect() , p->color );
}

SInnerMessage::~SInnerMessage()
{
    if( !p->remove_in_deconstructor )
        p->m_box->setParent( 0 );

    delete p;
}
