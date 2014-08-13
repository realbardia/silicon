#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>

#include "sudisks.h"

class UDiskPlugin : public SPlugin
{
public:
    UDiskPlugin();
    ~UDiskPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    SUDisksDeviceNotifier *udisk;
};

#endif // PLUGIN_H
