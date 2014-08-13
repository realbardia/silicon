#include "fuseisoengine.h"
#include "fuseiso.h"

SAbstractProcess *FUseIsoEngine::create( QObject *parent )
{
    return new FUseIso( parent );
}
