#include "sidi_prev.h"

#include <QMutex>
#include <QQueue>
#include <QFileInfo>
#include <QProcess>

#include <SFileInfo>
#include <SMusicInfo>
#include <SPictureInfo>
#include <SVideoInfo>

#include <tag.h>
#include <fileref.h>

class SScanDiscThreadPrivate
{
public:
    struct file_pack
    {
        QString disc_id;
        QString path;
        QString path_on_disc;
    };

    QMutex mutex;
    QQueue<file_pack> queue;
};

SScanDiscThread::SScanDiscThread( QObject *parent )
    : QThread( parent )
{
    p = new SScanDiscThreadPrivate;
}

void SScanDiscThread::scan( const QString & disc_id , const QString & file_path , const QString & path_on_disc )
{
    SScanDiscThreadPrivate::file_pack pack;
        pack.disc_id      = disc_id;
        pack.path         = file_path;
        pack.path_on_disc = path_on_disc;

    p->mutex.lock();
    p->queue.enqueue( pack );
    p->mutex.unlock();

    start();
}

void SScanDiscThread::run()
{
    QProcess process;

    while( !p->queue.isEmpty() )
    {
        p->mutex.lock();
        SScanDiscThreadPrivate::file_pack pack = p->queue.dequeue();
        p->mutex.unlock();


        SFileInfo result;
            result.setParent( QFileInfo(pack.path_on_disc).path() );
            result.setDiscId( pack.disc_id );

        completeFileInfo( result , pack.path );
        process.start( "file" , QStringList()<<"-b"<<"--mime-type"<<pack.path );
        if( !process.waitForFinished() )
        {
            emit fileScaned( result );
            continue;
        }

        QString output = process.readAllStandardOutput();
        if(      output.contains( "audio" , Qt::CaseInsensitive ) )
            completeMusicInfo( result , pack.path );
        else if( output.contains( "video" , Qt::CaseInsensitive ) )
            completeVideoInfo( result , pack.path );
        else if( output.contains( "image" , Qt::CaseInsensitive ) )
            completeImageInfo( result , pack.path );

        emit fileScaned( result );
    }
}

void SScanDiscThread::completeFileInfo( SFileInfo & info , const QString & path )
{
    QFileInfo file( path );

    info.setName( file.fileName() );
    info.setType( file.suffix()   );
    //info.setPermission(  );
    info.setOwner( file.owner() );
    info.setModifiedDate( file.lastModified().date() );
    info.setAccessesDate( file.lastRead().date() );
    info.setSize( file.size() );
}

void SScanDiscThread::completeMusicInfo( SFileInfo & info , const QString & path )
{
    TagLib::FileRef file( path.toUtf8().constData() );
    TagLib::Tag    *tag = file.tag();

    SMusicInfo music( path , info.discId() );
        music.setAlbum( QString::fromUtf8(tag->album().to8Bit(true).c_str()) );
        music.setArtist( QString::fromUtf8(tag->artist().to8Bit(true).c_str()) );
        //music.setCover(  );
        music.setGenre( QString::fromUtf8(tag->genre().to8Bit(true).c_str()) );
        music.setTitle( QString::fromUtf8(tag->title().to8Bit(true).c_str()) );
        music.setTrackNumber( tag->track() );

    info.setMusicInfo( music );
}

void SScanDiscThread::completeVideoInfo( SFileInfo & info , const QString & path )
{

}

void SScanDiscThread::completeImageInfo( SFileInfo & info , const QString & path )
{

}

SScanDiscThread::~SScanDiscThread()
{
    delete p;
}
