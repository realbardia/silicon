#include "plugin.h"

MkDiscFsPlugin::MkDiscFsPlugin()
    : SPlugin( QIcon(":/plugins/mkdiscfs/icon.png") ,
          "MkDiscFs",
          tr("Burn Data Discs using MkIsoFs and CdRecord") )
{
    engine = 0;
}

bool MkDiscFsPlugin::startEvent()
{
    engine = new MkDiscFsEngine();
    SiDiTools::setDataBurnerEngine( engine );
    return true;
}

bool MkDiscFsPlugin::stopEvent()
{
    SiDiTools::setDataBurnerEngine( 0 );
    return true;
}

MkDiscFsPlugin::~MkDiscFsPlugin()
{
}
