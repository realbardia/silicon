#ifndef LIBSIDI_PREV_H
#define LIBSIDI_PREV_H

#include <QThread>

class SFileInfo;

class SScanDiscThreadPrivate;
class SScanDiscThread : public QThread
{
    Q_OBJECT
public:
    SScanDiscThread( QObject *parent = 0 );
    ~SScanDiscThread();

    void scan( const QString & disc_id , const QString & file_path , const QString & path_on_disc );

signals:
    void fileScaned( const SFileInfo & file );

protected:
    void run();

private:
    void completeFileInfo(  SFileInfo & info , const QString & path );
    void completeMusicInfo( SFileInfo & info , const QString & path );
    void completeVideoInfo( SFileInfo & info , const QString & path );
    void completeImageInfo( SFileInfo & info , const QString & path );

private:
    SScanDiscThreadPrivate *p;
};

#endif // LIBSIDI_PREV_H
