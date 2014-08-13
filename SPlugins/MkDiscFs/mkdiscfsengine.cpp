#include "mkdiscfsengine.h"
#include "mkdiscfs.h"

SAbstractProcess *MkDiscFsEngine::create( QObject *parent )
{
    return new MkDiscFs( parent );
}
