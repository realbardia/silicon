#include "plugin.h"
#include <SDeviceList>

HalUDiskPlugin::HalUDiskPlugin()
    : SPlugin( QIcon(":/plugins/HalUDisk/icon.png") ,
          "HalUDisks",
          tr("Hal+UDisks Plugin, It's a Hybrid Device Notifier Plugin to get better performance." ))
{
    setConflicts( QStringList()<<"UDisks"<<"HAL" );
    hal = 0;
}

bool HalUDiskPlugin::startEvent()
{
    hal = new HalUDisk( this );
    SDeviceList::setNotifier( hal );

    hal->refresh();
    return true;
}

bool HalUDiskPlugin::stopEvent()
{
    SDeviceList::setNotifier( 0 );
    return true;
}

HalUDiskPlugin::~HalUDiskPlugin()
{

}
