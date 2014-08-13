#include "systray.h"

#include <QMainWindow>
#include <QMenu>
#include <QAction>

#include <Silicon>
#include <SMasterIcons>

class SysTrayPrivate
{
public:
    QMainWindow *main;

    QMenu *menu;
    QAction *quit_act;
};

SysTray::SysTray(QObject *parent) :
    QSystemTrayIcon( QIcon(":/files/Pixs/icon.png"), parent)
{
    p = new SysTrayPrivate;
    p->main = Silicon::mainWindow();

    p->quit_act = new QAction(SMasterIcons::icon( QSize(48,48) , "application-exit.png" ), tr("Quit"), this);
    p->menu = new QMenu();
        p->menu->addAction( p->quit_act );

    connect( this , SIGNAL(activated(QSystemTrayIcon::ActivationReason)) , SLOT(activate_changed(QSystemTrayIcon::ActivationReason)) );

    connect( p->quit_act , SIGNAL(triggered()) , p->main , SLOT(close()) );

    setContextMenu( p->menu );
}

void SysTray::activate_changed( QSystemTrayIcon::ActivationReason active )
{
    switch( static_cast<int>(active) )
    {
    case QSystemTrayIcon::Trigger :
        toggleVisiblity();
        break;
    }
}

void SysTray::toggleVisiblity()
{
    if( p->main->isVisible() )
        p->main->hide();
    else
        p->main->show();
}

SysTray::~SysTray()
{
    delete p->menu;
    delete p;
}
