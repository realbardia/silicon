#include "sapplicationengine.h"

SApplicationEngine::SApplicationEngine()
{
}

SApplicationEngine::SApplicationEngine(const QString & fileName)
{
    this->setFileName( fileName );
}

SApplicationEngine::~SApplicationEngine()
{
}

void SApplicationEngine::setFileName(const QString & fileName)
{
    file = fileName;
    library.setFileName( file );

    appFunction = (appProto) library.resolve( "application");
    confFunction = (confProto) library.resolve( "configure");
    aboutFunction = (aboutProto) library.resolve( "about");
}

int SApplicationEngine::application( const QVariantList & args )
{
    return appFunction( args );
}

QWidget *SApplicationEngine::configure()
{
    return confFunction();
}

SAboutData SApplicationEngine::about()
{
    return aboutFunction();
}

bool SApplicationEngine::isApplication()
{
    if( appFunction && aboutFunction )
        if( this->isAccessName(this->about().name()) )
            return true;

    return false;
}

bool SApplicationEngine::hasConfigure()
{
    if( confFunction )
        return true;

    return false;
}

bool SApplicationEngine::isAccessName(QString name)
{
    name = name.toLower();

    if(    name == "processes"
        || name == "appearance"
        || name == "general"
        || name == "about"  )

        return false;

    return true;
}

