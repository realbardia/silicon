#include "plugin.h"
#include <SDeviceList>

WdnPlugin::WdnPlugin()
    : SPlugin( QIcon(":/plugins/win32-device-notifier/icon.png") ,
          "Win32-Device-Notifier",
          tr("Plugin to detect Devices on the Microsoft Windows\nUsing WinEvents and Wodim/Cdrecord.") )
{
    setConflicts( QStringList()<<"UDisks"<<"Hal" );
    wdn = 0;
}

bool WdnPlugin::startEvent()
{
    wdn = new Win32DeviceNotifier( this );
    SDeviceList::setNotifier( wdn );

    wdn->refresh();
    return true;
}

bool WdnPlugin::stopEvent()
{
    SDeviceList::setNotifier( 0 );
    return true;
}

WdnPlugin::~WdnPlugin()
{

}
