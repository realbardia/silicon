#include "mpg123engine.h"
#include "mpg123.h"

SAbstractProcess *MPG123Engine::create( QObject *parent )
{
    return new MPG123( parent );
}
