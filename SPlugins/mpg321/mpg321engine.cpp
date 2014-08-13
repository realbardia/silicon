#include "mpg321engine.h"
#include "mpg321.h"

SAbstractProcess *MPG321Engine::create( QObject *parent )
{
    return new MPG321( parent );
}
