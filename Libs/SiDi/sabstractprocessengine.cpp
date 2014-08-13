#include "sabstractprocessengine.h"
#include "sabstractprocess.h"

SAbstractProcess *SAbstractProcessEngine::create( QObject *parent )
{
    return new SAbstractProcess( parent );
}
