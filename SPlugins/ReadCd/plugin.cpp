#include "plugin.h"

ReadCdPlugin::ReadCdPlugin()
    : SPlugin( QIcon(":/plugins/readcd/icon.png") ,
          "ReadCd",
          tr("Read Discs using ReadCd/Readom") )
{
    engine = 0;
}

bool ReadCdPlugin::startEvent()
{
    engine = new ReadCdEngine();
    SiDiTools::setDiscReaderEngine( engine );
    return true;
}

bool ReadCdPlugin::stopEvent()
{
    SiDiTools::setDiscReaderEngine( 0 );
    return true;
}

ReadCdPlugin::~ReadCdPlugin()
{
}
