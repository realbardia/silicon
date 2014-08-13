#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "audiocdrecordengine.h"

class AudioCdRecordPlugin : public SPlugin
{
public:
    AudioCdRecordPlugin();
    ~AudioCdRecordPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    AudioCdRecordEngine *engine;
};

#endif // PLUGIN_H
