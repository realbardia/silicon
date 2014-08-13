#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "mkdiscfsengine.h"

class MkDiscFsPlugin : public SPlugin
{
public:
    MkDiscFsPlugin();
    ~MkDiscFsPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    MkDiscFsEngine *engine;
};

#endif // PLUGIN_H
