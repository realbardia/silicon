#include "plugin.h"

AudioCdRecordPlugin::AudioCdRecordPlugin()
    : SPlugin( QIcon(":/plugins/audio-cdrecord/icon.png") ,
          "AudioCdRecord",
          tr("Burn Audio-Discs using AudioCdRecord/Wodim") )
{
    engine = 0;
}

bool AudioCdRecordPlugin::startEvent()
{
    engine = new AudioCdRecordEngine();
    SiDiTools::setAudioBurnerEngine( engine );
    return true;
}

bool AudioCdRecordPlugin::stopEvent()
{
    SiDiTools::setAudioBurnerEngine( 0 );
    return true;
}

AudioCdRecordPlugin::~AudioCdRecordPlugin()
{
}
