#include "plugin.h"
#include <SDeviceList>

UDiskPlugin::UDiskPlugin()
    : SPlugin( QIcon(":/plugins/udisks/icon.png") ,
          "UDisks",
          tr("Detect Devices using UDisks") )
{
    setConflicts( QStringList()<<"HAL" );
    udisk = 0;
}

bool UDiskPlugin::startEvent()
{
    udisk = new SUDisksDeviceNotifier( this );
    SDeviceList::setNotifier( udisk );

    udisk->refresh();
    return true;
}

bool UDiskPlugin::stopEvent()
{
    SDeviceList::setNotifier( 0 );
    return true;
}

UDiskPlugin::~UDiskPlugin()
{

}
