#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    RootMountPlugin *plugin = new RootMountPlugin();
    return plugin;
}
