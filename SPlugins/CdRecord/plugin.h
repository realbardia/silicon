#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "cdrecordengine.h"

class CdRecordPlugin : public SPlugin
{
public:
    CdRecordPlugin();
    ~CdRecordPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    CdRecordEngine *engine;
};

#endif // PLUGIN_H
