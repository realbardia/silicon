#ifndef SDATABASE_PREV_H
#define SDATABASE_PREV_H

#include <QThread>

#include "sdbtable.h"
#include "sfileinfo.h"
#include "sdiscinfo.h"
#include "stinyfileinfo.h"


class SSql;
class SSqlSignalControlerPrivate;
class SSqlSignalControler : public QObject
{
    Q_OBJECT
public:
    SSqlSignalControler( QObject *parent = 0 );
    ~SSqlSignalControler();

    void enqueueObject( SSql *obj , bool highPriority = false );
    void destroyed( SSql *obj );

public slots:
    void discInserted( const SDiscInfo & disc , bool result );
    void fileInserted( const SFileInfo & file , bool result );

    void discRemoved( const QString & disc_id , bool result );

    void discsIdDetected( const QStringList & list );
    void detailDetected( const SDiscInfo & disc );
    void detailDetected( const SFileInfo & file );

    void detectedFilesOf( const STinyFileInfo & dir , const SFileInfoList & files );
    void founded( const QString & key , const SFileInfoList & list );


private:
    SSqlSignalControlerPrivate *p;
};



class SThreadedSQLConnectPrivate;
class SThreadedSQLConnect : public QThread
{
    Q_OBJECT
public:
    SThreadedSQLConnect( QObject *parent = 0 );
    ~SThreadedSQLConnect();

    void insertDisc( const SDiscInfo & disc );
    void insertFile( const SFileInfo & file );

    void removeDisc( const QString & disc_id );

    void discsID();
    void discDetails( const QString & id , bool md5 );
    void fileDetails( int id );

    void filesOf( const STinyFileInfo & dir );
    void search( const QString & key , SFileInfo::FileCat cat );

signals:
    void discInserted( const SDiscInfo & disc , bool result );
    void fileInserted( const SFileInfo & file , bool result );

    void discRemoved( const QString & disc_id , bool result );

    void discsIdDetected( const QStringList & list );
    void detailDetected( const SDiscInfo & disc );
    void detailDetected( const SFileInfo & file );

    void detectedFilesOf( const STinyFileInfo & dir , const SFileInfoList & files );
    void founded( const QString & key , const SFileInfoList & list );

protected:
    void run();

private:
    SThreadedSQLConnectPrivate *p;
};



class SDataBaseBufferPrivate;
class SDataBaseBuffer
{
public:
    SDataBaseBuffer();
    ~SDataBaseBuffer();

    void buffer( int id , const QString & name , const QString & disc_id , const QString & address );
    void buffer( int id , const QString & artist , const QString & album_name , int year );
    void buffer( const QString & artist );

    int bufferPriority( const QString & name , const QString & disc_id , const QString & address );
    int bufferPriority( const QString & artist , const QString & album , int year );
    int bufferPriority( const QString & artist );

    bool isBuffered( const QString & name , const QString & disc_id , const QString & address );
    bool isBuffered( const QString & artist , const QString & album , int year );
    bool isBuffered( const QString & artist );

    int id( const QString & name , const QString & disc_id , const QString & address );
    int id( const QString & artist , const QString & album , int year );
    int id( const QString & artist_name );

private:
    SDataBaseBufferPrivate *p;
};

#endif // SDATABASE_PREV_H
