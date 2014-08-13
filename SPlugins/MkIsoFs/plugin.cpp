#include "plugin.h"

MkIsoFsPlugin::MkIsoFsPlugin()
    : SPlugin( QIcon(":/plugins/mkisofs/icon.png") ,
          "MkIsoFs",
          tr("Create Iso Images using MkIsoFs") )
{
    engine = 0;
}

bool MkIsoFsPlugin::startEvent()
{
    engine = new MkIsoFsEngine();
    SiDiTools::setImageCreatorEngine( engine );
    return true;
}

bool MkIsoFsPlugin::stopEvent()
{
    SiDiTools::setImageCreatorEngine( 0 );
    return true;
}

MkIsoFsPlugin::~MkIsoFsPlugin()
{
}
