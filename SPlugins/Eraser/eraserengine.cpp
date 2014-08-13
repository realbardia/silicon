#include "eraserengine.h"
#include "eraser.h"

SAbstractProcess *EraserEngine::create( QObject *parent )
{
    return new Eraser( parent );
}
