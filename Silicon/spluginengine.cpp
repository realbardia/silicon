#include "spluginengine.h"

SPluginEngine::SPluginEngine()
{

}

SPluginEngine::SPluginEngine( const QString & fileName )
{
    this->setFileName( fileName );
}

void SPluginEngine::setFileName( const QString & fileName )
{
    file = fileName;
    library.setFileName( file );

    pluginFunction = (pluginProto) library.resolve( "plugin");
}

bool SPluginEngine::isPlugin()
{
    if( pluginFunction )
        return true;

    return false;
}

SPlugin *SPluginEngine::plugin()
{
    return pluginFunction();
}

SPluginEngine::~SPluginEngine()
{

}
