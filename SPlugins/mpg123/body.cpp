#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    MPG123Plugin *plugin = new MPG123Plugin();
    return plugin;
}
