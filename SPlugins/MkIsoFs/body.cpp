#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    MkIsoFsPlugin *plugin = new MkIsoFsPlugin();
    return plugin;
}
