#ifndef SSQLCONNECT_H
#define SSQLCONNECT_H

#include <QString>
#include <QVariant>
#include <QObject>

#include "sfileinfo.h"
#include "sdiscinfo.h"
#include "sdbtable.h"
#include "stinyfileinfo.h"
#include "sdatabase_global.h"

class SSQLConnectPrivate;

class SDATABASESHARED_EXPORT SSQLConnect
{
public:
    SSQLConnect( const QString & location );
    ~SSQLConnect();

    bool isOpen();

    bool insertDisc( const SDiscInfo & disc );
    bool insertFile( const SFileInfo & file );

    bool removeDisc( const QString & disc_id );

    int discRow( const QString & disc_label );
    int fileID( const SFileInfo & file );
    int dirID( const STinyFileInfo & dir );

    SFileInfoList filesOf( const STinyFileInfo & dir );
    QStringList discsID();

    SDiscInfo discDetailsFromMd5( const QString & md5sum );
    SDiscInfo discDetails( const QString & id );
    SFileInfo fileDetails( int id );

    bool exists( const SDiscInfo & disc );
    bool exists( const SFileInfo & file );

    SFileInfoList search( const QString & key , SFileInfo::FileCat cat = SFileInfo::Normal );

private:
    int addNewDirectory( const QString & disc_id , const QString & dir );
    bool insertMusic( int file_id , const SMusicInfo & music );
    void create_db();

private:
    SSQLConnectPrivate *p;
};

#endif // SSQLCONNECT_H
