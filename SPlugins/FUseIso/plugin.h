#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "fuseisoengine.h"

class FUseIsoPlugin : public SPlugin
{
public:
    FUseIsoPlugin();
    ~FUseIsoPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    FUseIsoEngine *engine;
};

#endif // PLUGIN_H
