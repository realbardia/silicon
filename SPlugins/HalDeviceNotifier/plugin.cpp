#include "plugin.h"
#include <SDeviceList>

HalPlugin::HalPlugin()
    : SPlugin( QIcon(":/plugins/hal-notifier/icon.png") ,
          "HAL",
          tr("Detect Devices using Hal Device (Hardware Abstract Layer Device)" ))
{
    setConflicts( QStringList()<<"UDisks" );
    hal = 0;
}

bool HalPlugin::startEvent()
{
    hal = new SHALDeviceNotifier( this );
    SDeviceList::setNotifier( hal );

    hal->refresh();
    return true;
}

bool HalPlugin::stopEvent()
{
    SDeviceList::setNotifier( 0 );
    return true;
}

HalPlugin::~HalPlugin()
{

}
