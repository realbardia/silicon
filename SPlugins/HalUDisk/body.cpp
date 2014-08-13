#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    HalUDiskPlugin *plugin = new HalUDiskPlugin();
    return plugin;
}
