#include "cdrecordengine.h"
#include "cdrecord.h"

SAbstractProcess *CdRecordEngine::create( QObject *parent )
{
    return new CdRecord( parent );
}
