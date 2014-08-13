#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>

#include "shaldevicenotifier.h"

class HalPlugin : public SPlugin
{
public:
    HalPlugin();
    ~HalPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    SHALDeviceNotifier *hal;
};

#endif // PLUGIN_H
