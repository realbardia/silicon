#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    MkDiscFsPlugin *plugin = new MkDiscFsPlugin();
    return plugin;
}
