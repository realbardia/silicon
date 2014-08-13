#include "readcdengine.h"
#include "readcd.h"

SAbstractProcess *ReadCdEngine::create( QObject *parent )
{
    return new ReadCd( parent );
}
