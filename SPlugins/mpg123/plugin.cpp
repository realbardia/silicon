#include "plugin.h"

#include <SConverter>

MPG123Plugin::MPG123Plugin()
    : SPlugin( QIcon(":/plugins/MPG123/icon.png") ,
          "MPG123",
          tr("mpg123 plugin to convert mpeg formats.") )
{
    engine = 0;

    mpg123_types.insert( "ogg" , "wav" );
    mpg123_types.insert( "mp3" , "wav" );
}

bool MPG123Plugin::startEvent()
{
    if( !engine )
        engine = new MPG123Engine;

    SConverter::addConverterEngine( engine , mpg123_types );
    return true;
}

bool MPG123Plugin::stopEvent()
{
    if( !engine )
        return false;

    SConverter::removeConverterEngine( engine , mpg123_types );

    delete engine;
    engine = 0;

    return true;
}

MPG123Plugin::~MPG123Plugin()
{
}
