#define CONF_HEAD_STR      "SingleInnerDialog"
#define CONF_POS_CHILD_STR "position"

#include "dialogwidget.h"

#include <QPainter>
#include <QRect>
#include <QHBoxLayout>
#include <QLinearGradient>
#include <QDebug>
#include <QHash>
#include <QMouseEvent>
#include <QStatusBar>
#include <QToolBar>

#include <SAnimation>
#include <SColor>
#include <Silicon>

#include <math.h>

#ifdef Q_OS_WIN32
#define ANIMATION_FRAME_RATE 1
#else
#define ANIMATION_FRAME_RATE 10
#endif

class DialogWidgetPrivate
{
public:
    SDialog     *current;
    SDialog     *current_windowed;
    QHBoxLayout *layout;

    SAnimation *animation;

    bool last_enable_stt;

    QHash<SPage*,bool> hash;

    int w_position;
    int mouse_first_x;

    bool moving_allow;
    QPoint last_cursor_point;
};

DialogWidget::DialogWidget(QWidget *parent) :
    QWidget(parent)
{
    p = new DialogWidgetPrivate;
        p->current = 0;
        p->current_windowed = 0;
        p->w_position = Silicon::readConf( CONF_HEAD_STR , CONF_POS_CHILD_STR ).toInt();
        p->moving_allow = false;

    p->animation = new SAnimation( this );
        p->animation->setFrameRate( ANIMATION_FRAME_RATE );

    p->layout  = new QHBoxLayout( this );
        p->layout->setContentsMargins( 20 , 40 , 20 , 20 );

    SColor color( this->palette().windowText().color() );
#ifdef Q_OS_WIN32
        color.invert();
#endif

    QPalette pallete = this->palette();
        pallete.setColor( QPalette::WindowText , color );

    setPalette( pallete );
    connect( p->animation , SIGNAL(finished()) , SLOT(anim_ended()) );
    hide();
}

void DialogWidget::set( SDialog *dialog )
{
    if( dialog == 0 || dialog->pageParent()->pageType() == SPage::TabedPage )
        setTabbedPageDialog( dialog );
    else
        setWindowedPageDialog( dialog );
}

void DialogWidget::setWindowedPageDialog( SDialog *dialog )
{
    dialog->setParent( this );
    dialog->setWindowFlags( Qt::Dialog );
    dialog->show();

    SPage *page = dialog->pageParent();
    p->hash.insert( page , page->isEnabled() );
    page->setDisabled( true );

    if( page->toolBar() != 0 )
        page->toolBar()->setDisabled( true );

    if( page->statusBar() != 0 )
        page->statusBar()->setDisabled( true );

    connect( dialog , SIGNAL(destroyed(SDialog*)) , SLOT(windowedDialogClosed(SDialog*)) );
}

void DialogWidget::setTabbedPageDialog( SDialog *dialog )
{
    if( p->current != 0 )
    {
        SPage *page = p->current->pageParent();
            page->setEnabled( p->hash.value(page) || page->isEnabled() );

        if( page->toolBar() != 0 )
            page->toolBar()->setEnabled( page->isEnabled() );

        if( page->statusBar() != 0 )
            page->statusBar()->setEnabled( page->isEnabled() );

        p->hash.remove( page );
        p->current->hide();
        p->current->setParent( 0 );
        p->layout->removeWidget( p->current );
    }

    p->current = dialog;
    if( dialog == 0 )
    {
        p->animation->start( this , QSize(width(),0) , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
    }
    else
    {
        SPage *page = dialog->pageParent();
        p->hash.insert( page , page->isEnabled() );
        page->setDisabled( true );

        if( page->toolBar() != 0 )
            page->toolBar()->setDisabled( true );

        if( page->statusBar() != 0 )
            page->statusBar()->setDisabled( true );

        show();
        setFixedSize(dialog->width()+40,0);
        p->animation->start( this , QSize(dialog->width()+40,dialog->height()+60) , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
    }
}

void DialogWidget::windowedDialogClosed( SDialog *dialog )
{
    SPage *page = dialog->pageParent();
        page->setEnabled( p->hash.value(page) || page->isEnabled() );

    if( page->toolBar() != 0 )
        page->toolBar()->setEnabled( p->hash.value(page) );

    if( page->statusBar() != 0 )
        page->statusBar()->setEnabled( p->hash.value(page) );

    p->hash.remove( page );
}

void DialogWidget::anim_ended()
{
    if( height() == 0 )
    {
        hide();
    }
    else
    {
        show();
        p->layout->addWidget( p->current );
        p->current->show();
    }
}

void DialogWidget::paintEvent( QPaintEvent * )
{
    QSize size = static_cast<QWidget*>(parent())->size();

    int new_width = (size.width()-width())/2 + p->w_position;
    if( new_width < 0 )
        new_width = 0;
    else if( new_width > size.width()-width() )
        new_width = size.width()-width();

    move( new_width , 0 );

    QRect rct = rect();

    QColor color1(137,137,137,255);
    SColor color2( palette().window().color() );
 #ifdef Q_OS_WIN32
        color2.invert();
#endif
        color2 = color2 * 2;
        color2.setAlpha( 213 );


    QLinearGradient gradiant(QPointF(0, 0), QPointF(0,37));
        gradiant.setColorAt(0, color1);
        gradiant.setColorAt(1, color2);

    QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundRect(10, 0, rct.width() - 20, rct.height() - 10, 7 );
        path.addRect(     10, 0, rct.width() - 20, rct.height() - 10 -13 );

    QPainter painter(this);
        painter.setRenderHint( QPainter::Antialiasing , true );
        painter.fillPath( path , gradiant );


    QColor trns( 0 , 0 , 0 , 60 );
    for( int i=0 ; i<10 ; i++ )
    {
        QPainterPath path;
            path.setFillRule( Qt::WindingFill );
            path.addRoundRect( 10-i , -20 , rct.width() - (10-i)*2 , rct.height() + 10 + i , 7 );

        trns.setAlpha( 120 - sqrt(i)*40 );

        painter.setPen( trns );
        painter.drawPath( path );
    }
}

void DialogWidget::mousePressEvent( QMouseEvent * )
{
    p->moving_allow  = true;
    p->mouse_first_x = width()/2 - mapFromGlobal( QCursor::pos() ).x();

    setCursor( Qt::SizeAllCursor );
}

void DialogWidget::mouseMoveEvent( QMouseEvent *event )
{
    if( !p->moving_allow )
        return;

    QSize size = static_cast<QWidget*>(parent())->size();
    p->w_position = event->x() + mapToParent(QPoint()).x() - size.width()/2 + p->mouse_first_x;

    if( -37 < p->w_position && p->w_position < 37 )
        p->w_position = 0;

    repaint();
    p->last_cursor_point = event->pos();
}

void DialogWidget::mouseReleaseEvent( QMouseEvent * )
{
    p->moving_allow = false;
    setCursor( Qt::ArrowCursor );

    Silicon::writeConf( CONF_HEAD_STR , CONF_POS_CHILD_STR , QString::number(p->w_position) );
}

DialogWidget::~DialogWidget()
{
    if( p->current != 0 )
        p->layout->removeWidget( p->current );

    delete p;
}
