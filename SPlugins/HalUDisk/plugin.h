#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>

#include "haludisk.h"

class HalUDiskPlugin : public SPlugin
{
public:
    HalUDiskPlugin();
    ~HalUDiskPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    HalUDisk *hal;
};

#endif // PLUGIN_H
