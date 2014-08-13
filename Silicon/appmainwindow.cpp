#define MENU_SIZE QSize(48,32)
#define MENU_ICON_SIZE QSize(26,26)
#define PAGE_HEAD_CONF "PageSizes"

#include "appmainwindow.h"

#include <SApplication>
#include <SAboutData>
#include <SContextButton>
#include <SMasterIcons>
#include <Silicon>
#include <SAutoHideWidget>

#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>

class AppMainWindowPrivate
{
public:
    QVBoxLayout     *layout;
    QHBoxLayout     *toolbar_layout;
    QStatusBar      *statusbar;
    QToolBar        *toolbar;
    SContextButton  *menu;
    SAutoHideWidget *hide_widget;

    QAction *switch_type_action;
    QAction *close_action;

    SPage *page;
    QString win_id;
};

AppMainWindow::AppMainWindow( SPage *page , QWidget *parent )
    : QWidget( parent )
{
    p = new AppMainWindowPrivate;
    p->page      = page;
    p->statusbar = 0;
    p->toolbar   = 0;

    const SAboutData & about = p->page->parentApp()->parent();
        p->win_id = about.name() + "_" + about.version() + "_" + p->page->title();
        p->win_id.remove("=").remove(" ");


    /*! --------- Recover last Size of the window ------------- */

    QString type_str = Silicon::readConf( PAGE_HEAD_CONF , p->win_id );
    if( type_str.contains(",") )
    {
        const QStringList & list = type_str.split(",");
        if( list.count() == 2 )
            resize( list.first().toInt() , list.last().toInt() );
    }
    else
        resize( p->page->size() );

    /*! ------------------------------------------------------ */

    p->switch_type_action = new QAction( SMasterIcons::icon(QSize(48,48) , "window-duplicate.png") , tr("Switch page type") , this );
        p->switch_type_action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_W ) );
        p->switch_type_action->setEnabled( p->page->menuPanel()->isEnabled() );

    p->close_action = new QAction( SMasterIcons::icon(QSize(48,48) , "window-close.png") , tr("Close Page") , this );
        p->close_action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Q ) );

    addAction( p->switch_type_action );
    addAction( p->close_action );


    p->toolbar_layout = new QHBoxLayout();
        p->toolbar_layout->setContentsMargins( 0 , 0 , 0 , 0 );

    p->layout = new QVBoxLayout( this );
        p->layout->addLayout( p->toolbar_layout );
        p->layout->addWidget( p->page );
        p->layout->setContentsMargins( 1 , 1 , 1 , 1 );
        p->layout->setSpacing( 1 );

    setWindowFlags( Qt::Dialog );
    setMouseTracking( true );

    p->menu = new SContextButton();
        p->menu->setIcon( SMasterIcons::icon( QSize(48,48) , "configure.png" ) );
        p->menu->setFlat( true );
        p->menu->setIconSize( MENU_ICON_SIZE );
        p->menu->addItem( p->switch_type_action );
        p->menu->addItem( p->close_action       );

    p->hide_widget = new SAutoHideWidget( this );
        p->hide_widget->setWidget( p->menu );
        p->hide_widget->setFixedSize( MENU_SIZE );
        p->hide_widget->setHiddenTimer( 1500 );

    connect( p->page->menuPanel()  , SIGNAL(updated())      , this    , SLOT(updated())         );
    connect( p->page               , SIGNAL(resized(QSize)) , this    , SLOT(resizeSlot(QSize)) , Qt::QueuedConnection );
    connect( p->switch_type_action , SIGNAL(triggered())    , p->page , SLOT(switchPageType())  , Qt::QueuedConnection );
    connect( p->close_action       , SIGNAL(triggered())    , p->page , SLOT(close())           , Qt::QueuedConnection ) ;

    refresh();
    updated();

    QTimer::singleShot( 123 , p->hide_widget , SLOT(showWidget()) );
}

QToolBar *AppMainWindow::toolBar() const
{
    return p->toolbar;
}

QStatusBar *AppMainWindow::statusBar() const
{
    return p->statusbar;
}

void AppMainWindow::refresh()
{
    SApplication *application = p->page->parentApp();
    const SAboutData & about = application->parent();

    QString name = p->page->title();
    if( name.isEmpty() )
        name = about.name();

    QIcon icon = p->page->icon();
    if( icon.isNull() )
        icon = about.icon();

    setWindowTitle( name );
    setWindowIcon( icon );

    setToolBar(   p->page->toolBar()   );
    setStatusBar( p->page->statusBar() );
}

void AppMainWindow::setToolBar( QToolBar *toolbar )
{
    if( p->toolbar != 0 )
        p->toolbar_layout->removeWidget( p->toolbar );

    p->toolbar = toolbar;
    p->hide_widget->setActive( p->toolbar == 0 );
    if( p->toolbar == 0 )
        return;

    p->toolbar_layout->insertWidget( 0 , p->toolbar );
    p->toolbar->show();
}

void AppMainWindow::setStatusBar( QStatusBar *statusbar )
{
    if( p->statusbar != 0 )
        p->layout->removeWidget( p->statusbar );

    p->statusbar = statusbar;
    if( p->statusbar == 0 )
        return;

    p->layout->insertWidget( 2 , p->statusbar );
    p->statusbar->show();
}

void AppMainWindow::resizeEvent( QResizeEvent *event )
{
    p->hide_widget->move( event->size().width() - MENU_SIZE.width() , 0 );
}

void AppMainWindow::closeEvent( QCloseEvent *event )
{
    if( p->page->close() )
        event->accept();
    else
        event->ignore();
}

void AppMainWindow::updated()
{
    p->menu->clear();
    p->menu->addMenuPanel( p->page->menuPanel() );
    p->menu->addSeparator();
    p->menu->addItem( p->switch_type_action );
    p->menu->addItem( p->close_action       );

    p->switch_type_action->setEnabled( p->page->menuPanel()->isEnabled() );

    p->menu->setHidden( p->page->menuPanel()->isEmpty() );

    p->toolbar_layout->setContentsMargins( 0 , 0 , MENU_SIZE.width() , 0 );
}

void AppMainWindow::resizeSlot( const QSize & )
{
    int height = 0;
    int width  = 0;

    int min_height = 0;
    int min_width  = 0;

    int max_height = 0;
    int max_width  = 0;

    if( p->toolbar && !p->toolbar->isHidden() )
    {
        height += p->toolbar->height() + 1;

        if( p->toolbar->width() > width )
            width = p->toolbar->width() + 2;
    }

    if( p->statusbar && !p->statusbar->isHidden() )
    {
        height += p->statusbar->height() + 1;

        if( p->statusbar->width() > width )
            width = p->statusbar->width() + 2;
    }


    if( p->page && !p->page->isHidden() )
    {
        min_height = height + p->page->minimumHeight() + 2;
        max_height = height + p->page->maximumHeight() + 2;
        height     = height + p->page->height()        + 2;

        if( p->page->width() > width )
        {
            min_width = p->page->minimumWidth() + 2;
            max_width = p->page->maximumWidth() + 2;
            width     = p->page->width()        + 2;
        }
    }

    resize(  width , height );

    if( p->page->minimumHeight() != 0 )
        setMinimumHeight( min_height );
    if( p->page->maximumHeight() != 16777215 )
        setMaximumHeight( max_height );

    if( p->page->minimumWidth() != 0 )
        setMinimumWidth( min_width );
    if( p->page->maximumWidth() != 16777215 )
        setMaximumWidth( max_width );
}

AppMainWindow::~AppMainWindow()
{
    QString size_str = QString::number(size().width()) + "," + QString::number(size().height());
    Silicon::writeConf( PAGE_HEAD_CONF , p->win_id , size_str );

    if( p->toolbar != 0 )
    {
        p->toolbar->hide();
        p->toolbar->setParent( 0 );
    }

    if( p->statusbar != 0 )
    {
        p->statusbar->hide();
        p->statusbar->setParent( 0 );
    }

    p->page->setParent( 0 );

    delete p;
}
