#include "plugin.h"

SystemTrayPlugin::SystemTrayPlugin()
    : SPlugin( QIcon(":/plugins/SystemTray/icon.png") ,
          "SystemTray",
          tr("System Tray Plugin.") )
{
    tray = 0;
}

bool SystemTrayPlugin::startEvent()
{
    if( tray )
        return false;

    tray = new SysTray( this );
        tray->show();

    return true;
}

bool SystemTrayPlugin::stopEvent()
{
    if( !tray )
        return false;

    tray->hide();
    delete tray;
    tray = 0;

    return true;
}

SystemTrayPlugin::~SystemTrayPlugin()
{
}
