#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    SystemTrayPlugin *plugin = new SystemTrayPlugin();
    return plugin;
}
