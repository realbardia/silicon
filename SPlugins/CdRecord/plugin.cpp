#include "plugin.h"

CdRecordPlugin::CdRecordPlugin()
    : SPlugin( QIcon(":/plugins/cdrecord/icon.png") ,
          "CdRecord",
          tr("Copy Disc to Disc or Burn Images to Discsr using CdRecord/Wodim") )
{
    engine = 0;
}

bool CdRecordPlugin::startEvent()
{
    engine = new CdRecordEngine();
    SiDiTools::setDiscBurnerEngine( engine );
    return true;
}

bool CdRecordPlugin::stopEvent()
{
    SiDiTools::setDiscBurnerEngine( 0 );
    return true;
}

CdRecordPlugin::~CdRecordPlugin()
{
}
