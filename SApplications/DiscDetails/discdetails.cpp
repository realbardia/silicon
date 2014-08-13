#define ICON_SIZE     QSize(48,48)
#define WIDGET_WIDTH  413

#include "discdetails.h"
#include "deviceitem.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QFrame>
#include <QScrollArea>

#include <SMasterIcons>
#include <SSliderWidget>

class DiscDetailsPrivate
{
public:
    QVBoxLayout *layout;
    QScrollArea *scroll;

    QToolBar      *toolbar;
    SSliderWidget *slider;

    QAction *device_action;
    QAction *iso_action;
    QAction *disc_action;

    QWidget *spacer_widget;
};

DiscDetails::DiscDetails( SApplication *parent ) :
    SPage( tr("Disc Details") , parent )
{
    p = new DiscDetailsPrivate;

    p->spacer_widget = new QWidget();
        p->spacer_widget->setSizePolicy( QSizePolicy::MinimumExpanding , QSizePolicy::MinimumExpanding );

    p->slider = new SSliderWidget();
        p->slider->setShowingAll( true );
        p->slider->addWidget( p->spacer_widget );

    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );

    p->scroll = new QScrollArea();
        p->scroll->setWidget( p->slider );
        p->scroll->setWidgetResizable( true );
        p->scroll->setFrameShadow( QFrame::Plain );
        p->scroll->setFrameShape( QFrame::NoFrame );
        p->scroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
        p->scroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        p->scroll->viewport()->setAutoFillBackground( false );

    p->layout = new QVBoxLayout( this );
        //p->layout->addWidget( p->toolbar );
        p->layout->addWidget( p->scroll  );
        p->layout->setContentsMargins( 1 , 1 , 1 , 1 );
        p->layout->setSpacing( 1 );

    init_actions();
    setToolBar( p->toolbar );
    setMinimumSize( QSize(720,350) );
}

void DiscDetails::init_actions()
{
    p->device_action = new QAction( SMasterIcons::icon(ICON_SIZE,"drive-optical.png")          , tr("New Device")        , this );
    p->iso_action    = new QAction( SMasterIcons::icon(ICON_SIZE,"application-x-cd-image.png") , tr("New iso")           , this );
    p->disc_action   = new QAction( SMasterIcons::icon(ICON_SIZE,"server-database.png")        , tr("New DataBase Disc") , this );

    p->toolbar->addAction( p->device_action );
    p->toolbar->addAction( p->iso_action    );
    p->toolbar->addAction( p->disc_action   );

    menuPanel()->addAction( p->device_action );
    menuPanel()->addAction( p->iso_action    );
    menuPanel()->addAction( p->disc_action   );

    connect( p->device_action , SIGNAL(triggered()) , SLOT(addDevice()) );
}

void DiscDetails::addDevice()
{
    DeviceItem *item = new DeviceItem();
    p->slider->insertWidget( item , p->slider->count()-1 , WIDGET_WIDTH );

    connect( item , SIGNAL(closeRequest(QWidget*)) , SLOT(closeRequest(QWidget*)) , Qt::QueuedConnection );
}

void DiscDetails::closeRequest( QWidget *widget )
{
    p->slider->removeWidget( widget );
    delete widget;
}

DiscDetails::~DiscDetails()
{
    delete p;
}
