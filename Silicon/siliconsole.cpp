#include "siliconsole.h"

siliconsole::siliconsole()
{
    this->detectApps();
}

void siliconsole::detectApps()
{
    QStringList list = QDir(defaults.Silicon_Application_Directory).entryList( QStringList()<<"*.dll"<<"*.so" );

    for( int i=0 ; i<list.count() ; i++ )
    {
        SApplicationEngine app( defaults.Silicon_Application_Directory + '/' + list.at(i) );

        if( app.isApplication() && !this->isAdded( app.about() ) )
        {
            SAboutData about = app.about();
            appsList<< about;
        }
    }
}

bool siliconsole::isAdded( SAboutData about )
{
    for(int i=0;i<appsList.count();i++)
        if( about.name() == appsList.value(i).name() )
            return true;

    return false;
}
