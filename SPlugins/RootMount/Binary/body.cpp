#include <QCoreApplication>
#include <QHash>
#include <QDebug>
#include <QMetaType>

#include "mounter.h"
#include "unmounter.h"

int main( int argc, char *argv[] )
{
    QCoreApplication app( argc , argv );

    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    QStringList operations;
        operations << "--mount";
        operations << "--unmount";

    QHash<QString,QStringList> hash;
    for( int i=0 ; i<operations.count() ; i++ )
        hash.insert( operations.at(i) , QStringList() );

    const QStringList & arguments = app.arguments();
    QString operation = "--mount";

    for( int i=1 ; i<arguments.count() ; i++ )
    {
        const QString current = arguments.at(i);
        if( operations.contains(current) )
            operation = current;
        else
            hash[ operation ] << current;
    }


    const QStringList & mounts = hash.value("--mount");
    if( mounts.count() % 2 != 0 )    return 0;
    Mounter mounter( &app );
    for( int i=0 ; i<mounts.count() ; i+=2 )
        mounter.mount( mounts.at(i) , mounts.at(i+1) );


    const QStringList & unmounts = hash.value("--unmount");
    Unmounter unmounter( &app );
    for( int i=0 ; i<unmounts.count() ; i++ )
        unmounter.unmount( unmounts.at(i) );



    return app.exec();
}
