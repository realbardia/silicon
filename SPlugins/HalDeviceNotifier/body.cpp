#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    HalPlugin *plugin = new HalPlugin();
    return plugin;
}
