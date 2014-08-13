#include "rootmountengine.h"
#include "rootmount.h"

SAbstractProcess *RootMountEngine::create( QObject *parent )
{
    return new RootMount( parent );
}
