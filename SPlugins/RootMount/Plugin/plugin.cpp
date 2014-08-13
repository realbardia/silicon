#include "plugin.h"

RootMountPlugin::RootMountPlugin()
    : SPlugin( QIcon(":/plugins/fuseiso/icon.png") ,
          "RootMount",
          tr("Mount Disc images using root mount system.") )
{
    setConflicts( QStringList()<<"FUseIso" );
    engine = 0;
}

bool RootMountPlugin::startEvent()
{
    engine = new RootMountEngine();
    SiDiTools::setImageMounterEngine( engine );
    return true;
}

bool RootMountPlugin::stopEvent()
{
    SiDiTools::setImageMounterEngine( 0 );
    return true;
}

RootMountPlugin::~RootMountPlugin()
{
}
