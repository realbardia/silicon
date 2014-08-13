#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "mkisofsengine.h"

class MkIsoFsPlugin : public SPlugin
{
public:
    MkIsoFsPlugin();
    ~MkIsoFsPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    MkIsoFsEngine *engine;
};

#endif // PLUGIN_H
