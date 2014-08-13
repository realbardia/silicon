#include "automount.h"

#include <SiDiTools>
#include <SAbstractImageMounter>

AutoMount::AutoMount( QObject *parent ) : QObject( parent )
{
    mounter = 0;
    setMounter( SiDiTools::createImageMounter(this) );
}

AutoMount::AutoMount( SAbstractImageMounter *m , QObject *parent ) :QObject( parent )
{
    mounter = 0;
    setMounter( m );
}

void AutoMount::setMounter( SAbstractImageMounter *m )
{
    if( mounter != 0 )
        delete mounter;

    mounter = m;

    connect( mounter , SIGNAL(mounted(bool))   , SLOT(mounted(bool))        );
    connect( mounter , SIGNAL(unmounted(bool)) , SLOT(unmounted(bool))      );
    connect( mounter , SIGNAL(error(QString))  , SLOT(error(QString))       );
    connect( mounter , SIGNAL(finished(int))   , SLOT(processFinished(int)) );
}

void AutoMount::setMasterPoint( const QString & path )
{
    masterPointStr = path;
}

void AutoMount::process( const QString & fileName )
{
    process( fileName , QString() );
}

void AutoMount::process( const QString & fileName , const QString & mountPoint )
{
    QString mountPnt = mounter->mountPoint( fileName );
    outStr.clear();

    int num = 0;
    if( mountPnt.isEmpty() )
    {
        mountPnt = mountPoint;
        if( mountPnt.isEmpty() )
        {
            mountPnt = this->masterPointStr + '/' + QDir( fileName ).dirName() ;
            if( mountPnt.right(4).toLower() == ".iso" )
                mountPnt.remove( mountPnt.size()-4 , 4 );

            if( QDir( mountPnt ).exists() )
            {
                while( QDir( mountPnt + '-' + QString::number(num) ).exists() )
                    num++;

                mountPnt = mountPnt + '-' + QString::number(num) ;
            }
        }

        mounter->mount( QFileInfo(fileName).absoluteFilePath() , mountPnt );
    }
    else
    {
        mounter->unmount( mountPnt );
    }

}

QString AutoMount::masterPoint()
{
    return masterPointStr;
}

QString AutoMount::readOutPut()
{
    return outStr;
}

void AutoMount::mounted( bool stt )
{

}

void AutoMount::unmounted( bool stt )
{

}

void AutoMount::error( const QString & error )
{
    outStr = error;
}

void AutoMount::processFinished( int exitCode )
{

}

