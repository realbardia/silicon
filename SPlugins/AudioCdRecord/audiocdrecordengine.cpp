#include "audiocdrecordengine.h"
#include "audiocdrecord.h"

SAbstractProcess *AudioCdRecordEngine::create( QObject *parent )
{
    return new AudioCdRecord( parent );
}
