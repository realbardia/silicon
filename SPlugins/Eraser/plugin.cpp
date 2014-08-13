#include "plugin.h"

EraserPlugin::EraserPlugin()
    : SPlugin( QIcon(":/plugins/Eraser/icon.png") ,
          "Eraser",
          tr("Erase Optical discs using dvd+rw-tools and cdrecord") )
{
    engine = 0;
}

bool EraserPlugin::startEvent()
{
    engine = new EraserEngine();
    SiDiTools::setDiscEraserEngine( engine );
    return true;
}

bool EraserPlugin::stopEvent()
{
    SiDiTools::setDiscEraserEngine( 0 );
    return true;
}

EraserPlugin::~EraserPlugin()
{
}
