#include "mkisofsengine.h"
#include "mkisofs.h"

SAbstractProcess *MkIsoFsEngine::create( QObject *parent )
{
    return new MkIsoFs( parent );
}
