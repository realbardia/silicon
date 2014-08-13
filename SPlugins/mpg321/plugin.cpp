#include "plugin.h"

#include <SConverter>

MPG321Plugin::MPG321Plugin()
    : SPlugin( QIcon(":/plugins/MPG321/icon.png") ,
          "MPG321",
          tr("mpg321 plugin to convert mpeg formats.") )
{
    setConflicts( QStringList()<<"MPG123" );
    engine = 0;

    mpg123_types.insert( "ogg" , "wav" );
    mpg123_types.insert( "mp3" , "wav" );
}

bool MPG321Plugin::startEvent()
{
    if( !engine )
        engine = new MPG321Engine;

    SConverter::addConverterEngine( engine , mpg123_types );
    return true;
}

bool MPG321Plugin::stopEvent()
{
    if( !engine )
        return false;

    SConverter::removeConverterEngine( engine , mpg123_types );

    delete engine;
    engine = 0;

    return true;
}

MPG321Plugin::~MPG321Plugin()
{
}
