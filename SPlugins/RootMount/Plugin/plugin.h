#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "rootmountengine.h"

class RootMountPlugin : public SPlugin
{
public:
    RootMountPlugin();
    ~RootMountPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    RootMountEngine *engine;
};

#endif // PLUGIN_H
