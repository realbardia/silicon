#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    FUseIsoPlugin *plugin = new FUseIsoPlugin();
    return plugin;
}
