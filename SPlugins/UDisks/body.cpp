#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    UDiskPlugin *plugin = new UDiskPlugin();
    return plugin;
}
