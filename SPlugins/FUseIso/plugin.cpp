#include "plugin.h"

FUseIsoPlugin::FUseIsoPlugin()
    : SPlugin( QIcon(":/plugins/fuseiso/icon.png") ,
          "FUseIso",
          tr("Mount Disc images using FUseIso.") )
{
    engine = 0;
}

bool FUseIsoPlugin::startEvent()
{
    engine = new FUseIsoEngine();
    SiDiTools::setImageMounterEngine( engine );
    return true;
}

bool FUseIsoPlugin::stopEvent()
{
    SiDiTools::setImageMounterEngine( 0 );
    return true;
}

FUseIsoPlugin::~FUseIsoPlugin()
{
}
