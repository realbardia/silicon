#define SPLITTER_SIZE         5
#define MINIMUM_WIDGET_HEIGHT 7
#define TIMER_INTERVAL        p->animation_duration/p->animation_frame_rate
#define STEP_SIZE             width()/p->animation_frame_rate
#define MOVE_POINTS_WIDTH     3.0

#include "swidgetmanager.h"
#include "smastericons.h"
#include "scolor.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHash>
#include <QSignalMapper>
#include <QPushButton>
#include <QMouseEvent>
#include <QMetaObject>
#include <QPainterPath>
#include <QMenu>
#include <QAction>
#include <QVariantList>
#include <QPainter>
#include <QLabel>
#include <QDebug>

class SWidgetManagerPrivate
{
public:
    QHash<QWidget*,bool> widgets_status;
    QHash<QWidget*,int>  widgets_pixel;
    QHash<QWidget*,double>  widgets_percent;
    QList<QWidget*> widgets;
    QHash<QWidget*,QPushButton*> hide_buttons;
    QHash<int,QWidget*> timers;

    QSignalMapper *hide_signal_mapper;

    QWidget *on_splitter_widget;
    QWidget *on_move_widget;

    bool mouse_pressed;
    bool changed_lock;

    QPoint mouse_last_position;

    int sidebar_width;
    int show_button_height;
    int animation_duration;
    int animation_frame_rate;
};

SWidgetManager::SWidgetManager( QWidget *parent ) :
    QMainWindow(parent)
{
    p = new SWidgetManagerPrivate;
        p->mouse_pressed = false;
        p->changed_lock  = false;
        p->on_splitter_widget = 0;
        p->on_move_widget = 0;
        p->sidebar_width = 17;
        p->show_button_height = 73;
        p->animation_duration = 173;
        p->animation_frame_rate = 12;

    p->hide_signal_mapper = new QSignalMapper( this );

    connect( p->hide_signal_mapper   , SIGNAL(mapped(QWidget*)) , SLOT(switchWidgetVisiblity(QWidget*))  );

    setMouseTracking( true );
}

void SWidgetManager::refresh()
{
    const QList<QWidget*> & visible_widgets = p->widgets_status.keys(true);
    const QRect & rct = rect();

    double default_height    = height();
    double unsized_wgt_count = 0;

    for( int i=0 ; i<visible_widgets.count() ; i++ )
    {
        QWidget *w = visible_widgets.at(i);
        int height = p->widgets_pixel.value(w);
        if( !height )
            unsized_wgt_count += p->widgets_percent.value(w);
        else
            default_height -= height;
    }
    if( unsized_wgt_count != 0 )
        default_height = (default_height-SPLITTER_SIZE*visible_widgets.count())/unsized_wgt_count;


    QList<QPushButton*> bottom_hide_buttons;

    int y_pointer = 0;
    int hide_btn_y_pointer = 0;

    bool left_side_bar  = visible_widgets.count() != 0;
    bool right_side_bar = visible_widgets.count() != p->widgets.count();

    Q_UNUSED( left_side_bar )

    for( int i=0 ; i<p->widgets.count() ; i++ )
    {
        QWidget     *w          = p->widgets.at(i);
        QPushButton *hide_btn   = p->hide_buttons.value(w);
            hide_btn->raise();
            hide_btn->setFixedSize( p->sidebar_width , p->show_button_height );

        if( !p->widgets_status.value(w) )
        {
            w->hide();

            int hide_btn_y = std::max(y_pointer-p->show_button_height/2,hide_btn_y_pointer);
            if( hide_btn_y + p->show_button_height > rct.height()  )
            {
                hide_btn_y = rct.height() - p->show_button_height;

                for( int i=0 ; i<bottom_hide_buttons.count() ; i++ )
                {
                    QPushButton *b = bottom_hide_buttons.at(i);
                        b->move( b->x() , b->y()-p->show_button_height );
                }
                bottom_hide_buttons << hide_btn;
            }

            hide_btn->move( rct.width()-p->sidebar_width , hide_btn_y );
            hide_btn_y_pointer = hide_btn_y + p->show_button_height;

            continue;
        }

        int height = p->widgets_pixel.value(w);
        if( !height )
            height = default_height * p->widgets_percent.value(w);

        hide_btn->move( 0 , y_pointer + (height-p->show_button_height)/2 );
        hide_btn->show();

        w->resize( width()-(p->sidebar_width + right_side_bar*p->sidebar_width) , height );
        w->move( p->sidebar_width , y_pointer );
        w->show();

        y_pointer += height + SPLITTER_SIZE;
    }

    update();
}

QWidget *SWidgetManager::at( int index ) const
{
    return p->widgets.at(index);
}

int SWidgetManager::indexOf( QWidget *widget ) const
{
    return p->widgets.indexOf(widget);
}

void SWidgetManager::move( int from , int to )
{
    p->widgets.move( from , to );
    refresh();
}

void SWidgetManager::remove( QWidget *widget )
{
    removeAt( p->widgets.indexOf(widget) );
}

void SWidgetManager::removeAt( int index )
{
    takeAt( index );
}

QWidget *SWidgetManager::takeAt( int index )
{
    QWidget *w = p->widgets.at(index);

    p->widgets.removeAt( index );
    p->widgets_pixel.remove( w );
    p->widgets_status.remove( w );

    delete p->hide_buttons.take(w);

    refresh();
    return w;
}

void SWidgetManager::addWidget( QWidget *widget, int default_height, ResizeMode mode )
{
    insertWidget( count() , widget , default_height , mode );
}

void SWidgetManager::insertWidget( int index , QWidget *widget , int default_height, ResizeMode mode )
{
    widget->setParent( this );
    widget->setMouseTracking( true );
    widget->setCursor( Qt::ArrowCursor );

    p->widgets.insert( index , widget );
    p->widgets_status.insert( widget , true );

    switch( static_cast<int>(mode) )
    {
    case SWidgetManager::PIXEL:
        p->widgets_pixel.insert( widget , default_height );
        break;

    case SWidgetManager::PERCENT:
        p->widgets_percent.insert( widget , default_height );
        break;
    }

    QPushButton *hide_button = new QPushButton( this );
        hide_button->setIcon( SMasterIcons::icon(QSize(48,48),"arrow-right.png") );
        hide_button->setFixedSize( p->sidebar_width , p->show_button_height );
        hide_button->setCursor( Qt::ArrowCursor );
        hide_button->setFocusPolicy( Qt::NoFocus );

    p->hide_buttons.insert( widget , hide_button );
    p->hide_signal_mapper->setMapping( hide_button , widget );

    connect( hide_button   , SIGNAL(clicked()) , p->hide_signal_mapper   , SLOT(map()) );

    refresh();
}

bool SWidgetManager::widgetIsVisible( QWidget *widget ) const
{
    return p->widgets_status.value(widget);
}

bool SWidgetManager::widgetIsHidden( QWidget *widget ) const
{
    return !widgetIsVisible(widget);
}

void SWidgetManager::setSideBarWidth( int width )
{
    p->sidebar_width = width;
    refresh();
}

int SWidgetManager::sideBarWidth() const
{
    return p->sidebar_width;
}

void SWidgetManager::setShowButtonHeight( int height )
{
    p->show_button_height = height;
    refresh();
}

int SWidgetManager::showButtonHeight() const
{
    return p->show_button_height;
}

void SWidgetManager::setAnimationFrameRate( int rate )
{
    p->animation_frame_rate = rate;
}

int SWidgetManager::animationFrameRate() const
{
    return p->animation_frame_rate;
}

void SWidgetManager::setAnimationDuration( int dur )
{
    p->animation_duration = dur;
}

int SWidgetManager::animationDuration() const
{
    return p->animation_duration;
}

void SWidgetManager::hideWidget( QWidget *widget )
{
    if( !p->widgets_status[widget] )
        return;

    p->widgets_status[widget] = false;
    p->hide_buttons.value(widget)->setIcon( SMasterIcons::icon(QSize(48,48),"arrow-left.png") );
    p->hide_buttons.value(widget)->hide();

    p->timers.insert( startTimer(TIMER_INTERVAL) , widget );
}

void SWidgetManager::showWidget( QWidget *widget )
{
    if( p->widgets_status[widget] )
        return;

    p->widgets_status[widget] = true;
    p->hide_buttons.value(widget)->setIcon( SMasterIcons::icon(QSize(48,48),"arrow-right.png") );
    p->hide_buttons.value(widget)->hide();

    refresh();
    widget->move( width() , widget->y() );

    p->timers.insert( startTimer(TIMER_INTERVAL) , widget );
}

void SWidgetManager::switchWidgetVisiblity( QWidget *widget )
{
    if( widgetIsVisible(widget) )
        hideWidget(widget);
    else
        showWidget(widget);
}

int SWidgetManager::count() const
{
    return p->widgets.count();
}

void SWidgetManager::resizeEvent( QResizeEvent * )
{
    refresh();
}

void SWidgetManager::mouseMoveEvent( QMouseEvent *event )
{
    QWidget::mouseMoveEvent( event );

    int y = event->pos().y();
    int x = event->pos().x();

    const QList<QWidget*> & visible_widgets = p->widgets_status.keys(true);

    if( event->buttons() != Qt::LeftButton )
    {
        p->on_splitter_widget     = 0;
        p->on_move_widget         = 0;

        for( int i=0 ; i<visible_widgets.count() ; i++ )
        {
            QWidget *w = visible_widgets.at(i);
            int wy = w->y()+w->height();

            if( abs(wy-y) <= SPLITTER_SIZE && x>p->sidebar_width )
            {
                setCursor( Qt::SizeVerCursor );
                if( p->widgets.indexOf(w) == count()-1 )
                    continue;

                p->on_splitter_widget = w;
                break;
            }
            else if( y < w->y()+w->height() && y > w->y() && x<=p->sidebar_width )
            {
                setCursor( Qt::SizeAllCursor );
                p->on_move_widget = w;
                p->on_move_widget->raise();
                break;
            }

            setCursor( Qt::ArrowCursor );
        }
    }
    else
    {
        if( p->on_splitter_widget )
        {
            int new_height = y - p->on_splitter_widget->y();
            if( new_height < MINIMUM_WIDGET_HEIGHT )
                new_height = MINIMUM_WIDGET_HEIGHT;

            int delta = new_height - p->on_splitter_widget->height();

            if( p->widgets_pixel.contains(p->on_splitter_widget) )
                p->widgets_pixel[ p->on_splitter_widget ] = new_height;
            else
                p->widgets_percent[ p->on_splitter_widget ] = p->widgets_percent.value(p->on_splitter_widget)*((double)new_height/p->on_splitter_widget->height());


            QWidget *next_widget = p->widgets.at( p->widgets.indexOf(p->on_splitter_widget)+1 );
            if( p->widgets_pixel.contains(next_widget) )
                p->widgets_pixel[ next_widget ] = next_widget->height()-delta;
            else
                p->widgets_percent[ next_widget ] = p->widgets_percent.value(next_widget)*((double)(next_widget->height()-delta)/next_widget->height());

            refresh();
        }
        else if( p->on_move_widget )
        {
            int new_y = p->on_move_widget->y() + (y-p->mouse_last_position.y());

            QWidget *w = 0;
            for( int i=0 ; i<visible_widgets.count() ; i++ )
            {
                QWidget *tmp = visible_widgets.at(i);
                if( tmp == p->on_move_widget )
                    continue;

                if( y > tmp->y() && y < tmp->y()+p->on_move_widget->height() )
                {
                    w = tmp;
                    break;
                }
            }

            if( w && w != p->on_move_widget && p->widgets.indexOf(w) >= 0 )
            {
                p->widgets.move( p->widgets.indexOf(p->on_move_widget) , p->widgets.indexOf(w) );
                refresh();
            }

            p->on_move_widget->move( p->on_move_widget->x() , new_y );
        }
    }

    p->mouse_last_position = event->pos();
}

void SWidgetManager::mousePressEvent( QMouseEvent * )
{
    p->mouse_pressed = true;
}

void SWidgetManager::mouseReleaseEvent( QMouseEvent * )
{
    p->on_splitter_widget = 0;
    p->on_move_widget = 0;
    p->mouse_pressed = false;
    refresh();
}

void SWidgetManager::timerEvent( QTimerEvent *event )
{
    QWidget *w = p->timers.value( event->timerId() );
    bool visible = !p->widgets_status.value(w);
    int new_x = w->x()+STEP_SIZE*(visible*2-1);

    if( new_x >= width() || new_x <= p->sidebar_width )
    {
        refresh();
        p->hide_buttons.value(w)->show();
        killTimer( event->timerId() );
        p->timers.remove( event->timerId() );
        return;
    }

    w->move( new_x , w->y() );
}

void SWidgetManager::paintEvent( QPaintEvent * )
{
    const QList<QWidget*> & visible_widgets = p->widgets_status.keys(true);

    SColor color1( palette().window().color() );
    SColor color2( palette().window().color() );
        color2 = color2 * 0.6;
        color1 = color1 * 0.9;

    QPainter painter( this );
        painter.setRenderHint( QPainter::Antialiasing );

    bool left_side_bar  = visible_widgets.count() != 0;
    bool right_side_bar = visible_widgets.count() != p->widgets.count();

    if( left_side_bar )
        painter.fillRect( 0 , 0 , p->sidebar_width , height() , color2 );
    if( right_side_bar )
        painter.fillRect( width()-p->sidebar_width , 0 , p->sidebar_width , height() , color2 );

    for( int i=0 ; i<p->sidebar_width-MOVE_POINTS_WIDTH ; i+= MOVE_POINTS_WIDTH+1 )
        for( int j=((i/((int)MOVE_POINTS_WIDTH+1))%2)*(MOVE_POINTS_WIDTH+1)/2 ; j<height()-MOVE_POINTS_WIDTH ; j+= MOVE_POINTS_WIDTH+1 )
        {
            QPainterPath path;
            if( left_side_bar )
                path.addEllipse( i , j , MOVE_POINTS_WIDTH , MOVE_POINTS_WIDTH );
            if( right_side_bar )
                path.addEllipse( i+(width()-p->sidebar_width) , j , MOVE_POINTS_WIDTH , MOVE_POINTS_WIDTH );

            painter.fillPath( path , color1 );
        }

}

SWidgetManager::~SWidgetManager()
{
    delete p;
}
