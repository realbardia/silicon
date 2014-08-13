#include "ssqlconnect.h"
#include "sdatabase_prev.h"
#include "sdbtable.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QFile>
#include <QFileInfo>
#include <QQueue>
#include <QTest>
#include <QHash>
#include <QDebug>

#define BUFFER_MAX 20


// ============================================================================================
// ============================================================================================
// =====================                  EXTRA FUNCTIONS                ======================
// ============================================================================================
// ============================================================================================


SDBTable extractTable( QSqlQuery *query )
{
    QSqlRecord rec = query->record();
    SDBTable result;

    for( int i=0 ; i<rec.count() ; i++ )
    {
        QVariantList list;

        query->seek( -1 );
        while(query->next())
            list << query->value(i);

        result.insert( rec.fieldName(i) , list );
    }

    return result;
}


// ============================================================================================
// ============================================================================================
// ===========================             Private Class Datas           ======================
// ============================================================================================
// ============================================================================================

class SSQLConnectPrivate
{
public:
    QSqlDatabase db;
    SDataBaseBuffer buffer;
};



class SDataBaseBufferPrivate
{
public:
    struct artist{
        QString name;
    };

    struct album{
        QString artist;
        QString name;
        int year;
        int id;
    };

    struct directory{
        QString name;
        QString address;
        QString disc_id;
        int id;
    };

    QQueue<artist>     artist_q;
    QQueue<album>      album_q;
    QQueue<directory>  directory_q;

};



// ============================================================================================
// ============================================================================================
// ============================          SDataBaseBuffer           ============================
// ============================================================================================
// ============================================================================================



SDataBaseBuffer::SDataBaseBuffer()
{
    p = new SDataBaseBufferPrivate;
}

void SDataBaseBuffer::buffer( int id , const QString & name , const QString & disc_id , const QString & address )
{
    if( id < 0 )
        return ;

    if( this->id( name , disc_id , address ) != -1 )
        return ;

    SDataBaseBufferPrivate::directory dir;
        dir.name    = name;
        dir.address = address;
        dir.disc_id = disc_id;
        dir.id      = id;

    p->directory_q.enqueue( dir );

    if( p->directory_q.count() > BUFFER_MAX )
        p->directory_q.dequeue();
}

void SDataBaseBuffer::buffer( int id , const QString & artist , const QString & album_name , int year )
{
    if( id < 0 )
        return ;

    if( this->id( artist , album_name , year ) != -1 )
        return ;

    SDataBaseBufferPrivate::album album;
        album.artist = artist;
        album.name   = album_name;
        album.year   = year;
        album.id     = id;

    p->album_q.enqueue( album );

    if( p->album_q.count() > BUFFER_MAX )
        p->album_q.dequeue();
}

void SDataBaseBuffer::buffer( const QString & artist_name )
{
    if( this->id( artist_name ) != -1 )
        return ;

    SDataBaseBufferPrivate::artist artist;
        artist.name = artist_name;

    p->artist_q.enqueue( artist );

    if( p->artist_q.count() > BUFFER_MAX )
        p->artist_q.dequeue();
}

int SDataBaseBuffer::bufferPriority( const QString & name , const QString & disc_id , const QString & address )
{
    int count = p->directory_q.count();
    for( int i=0 ; i<count ; i++ )
    {
        const SDataBaseBufferPrivate::directory & dir = p->directory_q.at(i);
        if( dir.name == name && dir.disc_id == disc_id && dir.address == address )
            return i;
    }

    return -1;
}

int SDataBaseBuffer::bufferPriority( const QString & artist , const QString & album_name , int year )
{
    int count = p->album_q.count();
    for( int i=0 ; i<count ; i++ )
    {
        const SDataBaseBufferPrivate::album & album = p->album_q.at(i);
        if( album.artist == artist && album.name == album_name && album.year == year )
            return i;
    }

    return -1;
}

int SDataBaseBuffer::bufferPriority( const QString & artist_name )
{
    int count = p->artist_q.count();
    for( int i=0 ; i<count ; i++ )
    {
        const SDataBaseBufferPrivate::artist & artist = p->artist_q.at(i);
        if( artist.name == artist_name )
            return i;
    }

    return -1;
}

bool SDataBaseBuffer::isBuffered( const QString & name , const QString & disc_id , const QString & address )
{
    return bufferPriority( name , disc_id , address ) != -1;
}

bool SDataBaseBuffer::isBuffered( const QString & artist , const QString & album_name , int year )
{
    return bufferPriority( artist , album_name , year ) != -1;
}

bool SDataBaseBuffer::isBuffered( const QString & artist )
{
    return bufferPriority( artist ) != -1;
}

int SDataBaseBuffer::id( const QString & name , const QString & disc_id , const QString & address )
{
    int index = bufferPriority( name , disc_id , address );
    if( index < 0 )
        return -1;

           p->directory_q.move( index , p->directory_q.count()-1 );
    return p->directory_q.last().id;
}

int SDataBaseBuffer::id( const QString & artist , const QString & album , int year )
{
    int index = bufferPriority( artist , album , year );
    if( index < 0 )
        return -1;

           p->album_q.move( index , p->album_q.count()-1 );
    return p->album_q.last().id;
}

int SDataBaseBuffer::id( const QString & artist )
{
    int index = bufferPriority( artist );
    if( index < 0 )
        return -1;

           p->artist_q.move( index , p->artist_q.count()-1 );
    return 0;
}

SDataBaseBuffer::~SDataBaseBuffer()
{
    delete p;
}


// ============================================================================================
// ============================================================================================
// =========================               SSQLConnect             ============================
// ============================================================================================
// ============================================================================================




SSQLConnect::SSQLConnect( const QString & location )
{
    p = new SSQLConnectPrivate;

    bool db_exists = QFile::exists(location);

    p->db = QSqlDatabase::addDatabase( "QSQLITE" );
        p->db.setDatabaseName( location );
        p->db.open();

    if( !p->db.isOpen() )
        return ;

    if( !db_exists )
        create_db();
}

void SSQLConnect::create_db()
{
    qDebug( "Creating DataBase..." );
    QString disc_table_str = "CREATE  TABLE IF NOT EXISTS discs ("
                                "disc_id     VARCHAR(128)    NOT NULL ,"
                                "md5sum      VARCHAR(32)     NOT NULL ,"
                                "label       VARCHAR(128)    NULL ,"
                                "type        VARCHAR(6)      NULL ,"
                                "iso_address VARCHAR(512)    NULL ,"
                                "app_id      VARCHAR(128)    NULL ,"
                                "publisher   VARCHAR(128)    NULL ,"
                                "format      VARCHAR(4)      NULL ,"
                                "copy_right  VARCHAR(37)     NULL ,"
                                "sys_id      VARCHAR(32)     NULL ,"
                                "size        BIGINT          UNSIGNED NOT NULL ,"
                                "capacity    BIGINT          UNSIGNED NOT NULL ,"
                                "appendable  TINYINT(1)      NULL ,"
                                "PRIMARY KEY (md5sum) "
                             ")";

    QString dir_table_str = "CREATE  TABLE IF NOT EXISTS directories ("
                                "disc_id VARCHAR(128) NOT NULL ,"
                                "address VARCHAR(512)     NULL ,"
                                "name    VARCHAR(64)      NULL ,"
                                "PRIMARY KEY (disc_id, address, name),"

                                "FOREIGN KEY (disc_id ) "
                                "REFERENCES discs (disc_id ) "
                                "ON DELETE NO ACTION "
                                "ON UPDATE NO ACTION "
                            ")";

    QString file_table_str = "CREATE  TABLE IF NOT EXISTS files ( "
                                "dir_id     BIGINT       UNSIGNED NOT NULL ,"
                                "file_name  VARCHAR(64)  NOT NULL ,"
                                "type       VARCHAR(6)   NULL ,"
                                "permission VARCHAR(16)  NULL ,"
                                "owner      VARCHAR(32)  NULL ,"
                                "hash       VARCHAR(256) NULL ,"
                                "modified   DATE         NULL ,"
                                "accessed   DATE         NULL ,"
                                "rate       TINYINT(3)   NULL ,"
                                "size       BIGINT       UNSIGNED NOT NULL ,"
                                "UNIQUE (dir_id, file_name),"

                                "FOREIGN KEY (dir_id ) "
                                "REFERENCES directories (rowid ) "
                                "ON DELETE NO ACTION "
                                "ON UPDATE NO ACTION "
                            ")";

    QString artist_table_str = "CREATE  TABLE IF NOT EXISTS artist ("
                                "artist VARCHAR(128) NOT NULL ,"
                                "PRIMARY KEY (artist) "
                            ")";

    QString album_table_str = "CREATE  TABLE IF NOT EXISTS album ("
                                "album    VARCHAR(128)    NOT NULL ,"
                                "artist   VARCHAR(128)    NOT NULL ,"
                                "year     SMALLINT        UNSIGNED NULL ,"
                                "UNIQUE (album, artist, year),"

                                "FOREIGN KEY (artist ) "
                                "REFERENCES artist (artist ) "
                                "ON DELETE NO ACTION "
                                "ON UPDATE NO ACTION "
                            ")";

    QString track_table_str = "CREATE  TABLE IF NOT EXISTS tracks ("
                                "id           BIGINT       UNSIGNED NOT NULL ,"
                                "album_id     BIGINT       UNSIGNED NOT NULL ,"
                                "title        VARCHAR(128) NULL ,"
                                "genre        VARCHAR(16)  NULL ,"
                                "mood         VARCHAR(16)  NULL ,"
                                "play_count   SMALLINT     UNSIGNED NULL ,"
                                "track_number INT          UNSIGNED NULL ,"
                                "duration     TIME         NULL ,"
                                "PRIMARY KEY (id) ,"

                                "FOREIGN KEY (album_id ) "
                                "REFERENCES album (rowid ) "
                                "ON DELETE NO ACTION "
                                "ON UPDATE NO ACTION, "

                                "FOREIGN KEY (id ) "
                                "REFERENCES files (rowid ) "
                                "ON DELETE NO ACTION "
                                "ON UPDATE NO ACTION "
                            ")";

    QString pics_table_str = "CREATE  TABLE IF NOT EXISTS pictures ("
                                "id       BIGINT   UNSIGNED NOT NULL ,"
                                "date     DATE     NULL ,"
                                "location GEOMETRY NULL ,"
                                "PRIMARY KEY (id) ,"

                                "FOREIGN KEY (id ) "
                                "REFERENCES files (rowid ) "
                                "ON DELETE NO ACTION "
                                "ON UPDATE NO ACTION"
                            ")";

    QString video_table_str = "CREATE  TABLE IF NOT EXISTS videos ("
                                "id         BIGINT       UNSIGNED NOT NULL ,"
                                "title      VARCHAR(128) NULL ,"
                                "publisher  VARCHAR(128) NULL ,"
                                "copy_right VARCHAR(37)  NULL ,"
                                "PRIMARY KEY (id) ,"

                                "FOREIGN KEY (id ) "
                                "REFERENCES files (rowid ) "
                                "ON DELETE NO ACTION "
                                "ON UPDATE NO ACTION"
                            ")";

    QSqlQuery query( p->db );
        query.exec( disc_table_str   );
        query.exec( dir_table_str    );
        query.exec( file_table_str   );
        query.exec( artist_table_str );
        query.exec( album_table_str  );
        query.exec( track_table_str  );
        query.exec( pics_table_str   );
        query.exec( video_table_str  );
}

bool SSQLConnect::isOpen()
{
    return p->db.isOpen();
}

bool SSQLConnect::insertDisc( const SDiscInfo & disc )
{
    QSqlQuery query( p->db );

    bool result;
    result = query.exec( "INSERT INTO discs VALUES("
                           + QString("\"") + disc.discId()                        + "\","
                           + QString("\"") + disc.md5Sum()                        + "\","
                           + QString("\"") + disc.label()                         + "\","
                           + QString("\"") + disc.typeString()                    + "\","
                           + QString("\"") + disc.address()                       + "\","
                           + QString("\"") + disc.applicationId()                 + "\","
                           + QString("\"") + disc.publisher()                     + "\","
                           + QString("\"") + disc.format()                        + "\","
                           + QString("\"") + disc.copyRight()                     + "\","
                           + QString("\"") + disc.sysId()                         + "\","
                           + QString(    ) + QString::number(disc.size())         + ","
                           + QString(    ) + QString::number(disc.freeCapacity()) + ","
                           + QString(    ) + QString::number(disc.appendable())   + ")"
                 );

    if( !result )
        return false;


    result = query.exec( "INSERT INTO directories VALUES("
                           + QString("\"") + disc.discId()  + "\" ,NULL,NULL)"
                       );

    return result;
}

bool SSQLConnect::insertFile( const SFileInfo & file )
{
    QSqlQuery query( p->db );

    int dir_id = addNewDirectory( file.discId() , file.parent() );

    QString query_str = "INSERT INTO files VALUES("
                            + QString(    ) + QString::number(dir_id)                    + ","
                            + QString("\"") + file.name()                                + "\","
                            + QString("\"") + file.type()                                + "\","
                            + QString("\"") + file.permission()                          + "\","
                            + QString("\"") + file.owner()                               + "\","
                            + QString("\"") + file.hash()                                + "\","
                            + QString("\"") + file.modifiedDate().toString("yyyy-MM-dd") + "\","
                            + QString("\"") + file.accessedDate().toString("yyyy-MM-dd") + "\","
                            + QString(    ) + QString::number(file.rate())               + ","
                            + QString(    ) + QString::number(file.size())               + ")";

    query_str.replace( "\"\"" , "NULL" );

    int file_id = -1;
    query.exec( query_str );
    query.exec( "SELECT rowid FROM files WHERE "
                         "dir_id=\""     + QString::number(dir_id) + "\" AND "
                         "file_name=\""  + file.name()             + "\""      );

    const SDBTable & table = extractTable( &query );
    if( table.contains( "rowid" ) && table.value("rowid").count() == 1 )
        file_id = table.value("rowid").first().toInt();


    if( file.category() == SFileInfo::Music )
        insertMusic( file_id , file.musicInfo() );

    return file_id != -1;
}

int SSQLConnect::addNewDirectory( const QString & disc_id , const QString & dir_str )
{
    QSqlQuery query( p->db );
    QFileInfo dir( dir_str );

    QString dir_path = dir.path();

    if( !dir_path.isEmpty() && dir_path[0] == '.' )
        dir_path.remove(0,1);

    if( !dir_path.isEmpty() && dir_path!="/" )
        addNewDirectory( disc_id , dir_path );

    STinyFileInfo tfile;
        tfile.setAddress( dir_path );
        tfile.setName( dir.fileName() );
        tfile.setDiscID( disc_id );
        tfile.setDirectory( true );

    int dir_id = dirID( tfile );
    if( dir_id == -1 )
    {
        bool done;

        QString query_str = "INSERT INTO directories VALUES("
                                + QString("\"") + disc_id        + "\","
                                + QString("\"") + dir_path       + "\","
                                + QString("\"") + dir.fileName() + "\")";

        query_str.replace( "\"\"" , "NULL" );
        done = query.exec( query_str );

        if( !done )
            return false;

        QString query_str2 = "SELECT rowid FROM directories WHERE "
                                    "disc_id=\""  + disc_id        + "\" AND "
                                    "address=\""  + dir_path       + "\" AND "
                                    "name=\""     + dir.fileName() + "\"";

        query_str2.replace( "\"\"" , "NULL" );
        query.exec( query_str2 );

        const SDBTable & table = extractTable( &query );
        if( table.contains( "rowid" ) && table.value("rowid").count() == 1 )
            dir_id = table.value("rowid").first().toInt();

        p->buffer.buffer( dir_id , disc_id , dir.fileName() , dir_path );
    }

    return dir_id;
}

bool SSQLConnect::removeDisc( const QString & disc_id )
{
    /*! --- Check Disc Exist --- */
    SDiscInfo disc( disc_id );
    if( !exists(disc) )
        return false;

    QSqlQuery query( p->db );
    bool done;


    /*! --- Find All Directories in disc --- */
    QString query_str = "SELECT rowid FROM directories WHERE "
                                "disc_id=\""  + disc_id + "\"";

    done = query.exec( query_str );
    if( !done )
        return false;

    QStringList dir_id_list;
    const SDBTable & table = extractTable( &query );
    if( table.contains( "rowid" ) )
    {
        for( int i=0 ; i<table.value("rowid").count() ; i++ )
            dir_id_list << QString::number(table.value("rowid").at(i).toInt());
    }

    if( dir_id_list.isEmpty() )
        return false;


    /*! --- Delete all files in directories that We found before --- */
    query_str = "DELETE FROM files WHERE " + QString()
                    + " dir_id=" + dir_id_list.join(" OR dir_id=");

    done = query.exec( query_str );
    if( !done )
        return false;



    /*! --- Delete all directories that We found before --- */
    query_str = "DELETE FROM directories WHERE disc_id=\"" + disc_id + "\"";
    done = query.exec( query_str );
    if( !done )
        return false;



    /*! --- Delete Disc --- */
    query_str = "DELETE FROM discs WHERE disc_id=\"" + disc_id + "\"";
    done = query.exec( query_str );
    if( !done )
        return false;


    return true;
}

int SSQLConnect::discRow( const QString & disc_label )
{
    int result = -1;
    QString query_str = "SELECT rowid FROM discs WHERE "
                            "disc_id=\""  + disc_label + "\"";

    QSqlQuery query( p->db );
        query.exec( query_str );

    const SDBTable & table = extractTable( &query );
    if( table.contains( "rowid" ) && table.value("rowid").count() == 1 )
        result = table.value("rowid").first().toInt();

    return result;
}

int SSQLConnect::fileID( const SFileInfo & file )
{
    QSqlQuery query( p->db );
    QFileInfo dir( file.parent() );

    int dir_id = p->buffer.id( file.discId() , dir.fileName() , dir.path() );
    if( dir_id == -1 )
    {
        query.exec( "SELECT rowid FROM directories WHERE "
                             "disc_id=\""  + file.discId()  + "\" AND "
                             "address=\""  + dir.path()     + "\" AND "
                             "name=\""     + dir.fileName() + "\""      );

        const SDBTable & table = extractTable( &query );
        if( table.contains( "rowid" ) && table.value("rowid").count() == 1 )
            dir_id = table.value("rowid").first().toInt();

        p->buffer.buffer( dir_id , file.discId() , dir.fileName() , dir.path() );
    }

    int file_id = -1;
    query.exec( "SELECT rowid FROM files WHERE "
                         "dir_id=\""     + QString::number(dir_id) + "\" AND "
                         "file_name=\""  + file.name()             + "\""      );

    const SDBTable & table = extractTable( &query );
    if( table.contains( "rowid" ) && table.value("rowid").count() == 1 )
        file_id = table.value("rowid").first().toInt();

    return file_id;
}

int SSQLConnect::dirID( const STinyFileInfo & dir )
{
    QSqlQuery query( p->db );

    int dir_id = p->buffer.id( dir.discID() , dir.name() , dir.address() );
    if( dir_id == -1 )
    {
        QString dir_path = dir.address();
        if( !dir_path.isEmpty() && dir_path[0] == '.' )
            dir_path.remove(0,1);
        if( dir_path == "/" )
            dir_path = QString();

        QString query_str = "SELECT rowid FROM directories WHERE "
                                "disc_id=\""  + dir.discID() + "\" AND "
                                "name=\""     + dir.name()   + "\" AND "
                                "address=\""  + dir_path     + "\"";

        query_str.replace( "=\"\"" , " is NULL" );
        query.exec( query_str );

        const SDBTable & table = extractTable( &query );
        if( table.contains( "rowid" ) && table.value("rowid").count() == 1 )
            dir_id = table.value("rowid").first().toInt();

        p->buffer.buffer( dir_id , dir.discID() , dir.name() , dir.address() );
    }

    return dir_id;
}

SFileInfoList SSQLConnect::filesOf( const STinyFileInfo & dir )
{
    SFileInfoList result;
    QSqlQuery query( p->db );

    if( dir.discID().isEmpty() )
        return result;

    QString dir_address;
    if( dir.address() != "/" )
        dir_address = dir.address();

    int dir_id = dir.ID();
    if( dir_id == -1 )
        dir_id = dirID( dir );
    if( dir_id == -1 )
        return result;

    dir_address = dir_address + "/" + dir.name();
    if( !dir_address.isEmpty() && dir_address[0] == '.' )
        dir_address.remove(0,1);

    QString query_str = "SELECT rowid, name FROM directories WHERE "
                            "disc_id=\""  + dir.discID() + "\" AND "
                            "address=\""  + dir_address  + "\"";

    query_str.replace( "\"\"" , "NULL" );
    query.exec( query_str );

    const SDBTable & d_table = extractTable( &query );
    if( d_table.contains( "name" ) && d_table.value("name").count() > 0 )
    {
        QVariantList id_list = d_table.value("rowid");
        QVariantList name_list = d_table.value("name");

        for( int i=0 ; i<name_list.count() ; i++ )
        {
            STinyFileInfo file;
                file.setDirectory( true );
                file.setID( id_list.at(i).toString().toInt() );
                file.setDiscID( dir.discID() );
                file.setAddress( dir_address );
                file.setName( name_list.at(i).toString() );

            result << file;
        }
    }



    query.exec( "SELECT rowid, file_name, type FROM files WHERE "
                         "dir_id=\""     + QString::number(dir_id) + "\"" );

    const SDBTable & f_table = extractTable( &query );
    if( f_table.contains( "file_name" ) && f_table.value("file_name").count() > 0 )
    {
        QVariantList id_list = f_table.value("rowid");
        QVariantList name_list = f_table.value("file_name");
        QVariantList type_list = f_table.value("type");

        for( int i=0 ; i<name_list.count() ; i++ )
        {
            STinyFileInfo file;
                file.setDirectory( false );
                file.setID( id_list.at(i).toString().toInt() );
                file.setDiscID( dir.discID() );
                file.setAddress( dir_address );
                file.setName( name_list.at(i).toString() );
                file.setType( type_list.at(i).toString() );

            result << file;
        }
    }


    return result;
}

QStringList SSQLConnect::discsID()
{
    QSqlQuery query( p->db );
        query.exec( "SELECT disc_id FROM discs" );

    QStringList result;

    const SDBTable & table = extractTable( &query );
    if( table.contains( "disc_id" ) && table.value("disc_id").count() > 0 )
    {
        QVariantList list = table.value("disc_id");
        for( int i=0 ; i<list.count() ; i++ )
            result << list.at(i).toString();
    }

    return result;
}

SDiscInfo SSQLConnect::discDetailsFromMd5( const QString & md5sum )
{
    QSqlQuery query( p->db );
        query.exec( "SELECT * FROM discs WHERE "
                         "md5sum=\""     + md5sum + "\""      );

    SDiscInfo result;

    const SDBTable & table = extractTable( &query );
    if( table.value("disc_id").count() != 1 )
        return result;

    result.setDiscId(        table.value("disc_id"     ).first().toString() );
    result.setMd5Sum(        md5sum                                         );
    result.setLabel(         table.value("label"       ).first().toString() );
    result.setTypeString(    table.value("type"        ).first().toString() );
    result.setAddress(       table.value("iso_address" ).first().toString() );
    result.setApplicationId( table.value("app_id"      ).first().toString() );
    result.setPublisher(     table.value("publisher"   ).first().toString() );
    result.setFormat(        table.value("format"      ).first().toString() );
    result.setCopyRight(     table.value("copy_right"  ).first().toString() );
    result.setSysId(         table.value("sys_id"      ).first().toString() );
    result.setSize(          table.value("size"        ).first().toInt()    );
    result.setFreeCapacity(  table.value("capacity"    ).first().toInt()    );
    result.setAppendable(    table.value("appendable"  ).first().toBool()   );

    return result;
}

SDiscInfo SSQLConnect::discDetails( const QString & id )
{
    QSqlQuery query( p->db );
        query.exec( "SELECT * FROM discs WHERE "
                         "disc_id=\""     + id + "\""      );

    SDiscInfo result;

    const SDBTable & table = extractTable( &query );
    if( table.value("disc_id").count() != 1 )
        return result;

    result.setDiscId(        id                                             );
    result.setMd5Sum(        table.value("md5sum"      ).first().toString() );
    result.setLabel(         table.value("label"       ).first().toString() );
    result.setTypeString(    table.value("type"        ).first().toString() );
    result.setAddress(       table.value("iso_address" ).first().toString() );
    result.setApplicationId( table.value("app_id"      ).first().toString() );
    result.setPublisher(     table.value("publisher"   ).first().toString() );
    result.setFormat(        table.value("format"      ).first().toString() );
    result.setCopyRight(     table.value("copy_right"  ).first().toString() );
    result.setSysId(         table.value("sys_id"      ).first().toString() );
    result.setSize(          table.value("size"        ).first().toInt()    );
    result.setFreeCapacity(  table.value("capacity"    ).first().toInt()    );
    result.setAppendable(    table.value("appendable"  ).first().toBool()   );

    return result;
}

SFileInfo SSQLConnect::fileDetails( int id )
{
    QSqlQuery query( p->db );
        query.exec( "SELECT f.file_name, f.type, f.permission, f.owner, f.hash, "
                           "f.modified, f.accessed, f.size, f.rate, d.disc_id, d.address, d.name "
                           "FROM files AS f, directories AS d "
                           "WHERE f.rowid=\"" + QString::number(id) + "\"" );

    SFileInfo result;

    const SDBTable & table = extractTable( &query );
    if( table.value("rowid").count() != 1 )
        return result;

    result.setName(         table.value("file_name" ).first().toString() );
    result.setType(         table.value("type"      ).first().toString() );
    result.setPermission(   table.value("permission").first().toString() );
    result.setOwner(        table.value("owner"     ).first().toString() );
    result.setHash(         table.value("hash"      ).first().toString() );
    result.setDiscId(       table.value("disc_id"   ).first().toString() );/*
    result.setParent(       table.value("parent"    ).first().toString() );*/
    result.setModifiedDate( table.value("modified"  ).first().toDate()   );
    result.setAccessesDate( table.value("accessed"  ).first().toDate()   );
    result.setSize(         table.value("size"      ).first().toInt()    );
    result.setRate(         table.value("rate"      ).first().toInt()    );

    return result;
}

bool SSQLConnect::exists( const SDiscInfo & disc )
{
    int disc_id = -1;
    QSqlQuery query( p->db );
        query.exec( "SELECT md5sum FROM discs WHERE "
                         "md5sum=\""     + disc.md5Sum() + "\""      );

    const SDBTable & table = extractTable( &query );
    if( table.contains( "md5sum" ) && table.value("md5sum").count() == 1 )
        disc_id = table.value("md5sum").first().toInt();

    return disc_id != -1;
}

SFileInfoList SSQLConnect::search( const QString & key , SFileInfo::FileCat cat )
{
    SFileInfoList result;

    if( key.count() < 3 )
        return result;

    QStringList table_names;
    switch( static_cast<int>(cat) )
    {
    case SFileInfo::Music :
        table_names << "tracks" << "album" << "artist" << "files";
        break;

    case SFileInfo::Photo :
        table_names << "pictures" << "files";
        break;

    case SFileInfo::Video :
        table_names << "videos" << "files";
        break;

    case SFileInfo::Normal :
        table_names << "tracks" << "album" << "artist" << "pictures" << "videos" <<"files";
        break;
    }


    return result;
}

bool SSQLConnect::exists( const SFileInfo & file )
{
    return fileID(file) != -1;
}

bool SSQLConnect::insertMusic( int file_id , const SMusicInfo & music )
{
    QSqlQuery query( p->db );

    if( p->buffer.id( music.artist() ) == -1 )
        query.exec( "INSERT INTO artist VALUES( \"" + music.artist() + "\" )" );


    int album_id = p->buffer.id( music.artist() , music.album() , music.year() );
    if( album_id == -1 )
    {
        query.exec( "INSERT INTO album VALUES("
                           + QString("\"") + music.album()                 + "\","
                           + QString("\"") + music.artist()                + "\","
                           + QString("\"") + QString::number(music.year()) + "\")" );

        query.exec( "SELECT rowid FROM album WHERE "
                             "album=\""  + music.album()                 + "\" AND "
                             "artist=\"" + music.artist()                + "\" AND "
                             "year="   + QString::number(music.year()) );

        const SDBTable & table = extractTable( &query );
        if( table.contains( "rowid" ) && table.value("rowid").count() == 1 )
            album_id = table.value("rowid").first().toInt();
    }


    bool result = false;
    if( album_id != -1 )
    {
        result = query.exec( "INSERT INTO tracks VALUES("
                           + QString(    ) + QString::number(file_id)             + ","
                           + QString(    ) + QString::number(album_id)            + ","
                           + QString("\"") + music.title()                        + "\","
                           + QString("\"") + music.genre()                        + "\","
                           + QString("\"") + music.mood()                         + "\","
                           + QString(    ) + QString::number(0)                   + ","
                           + QString(    ) + QString::number(music.trackNumber()) + ","
                           + QString("\"") + music.duration().toString("h:m:s")   + "\")" );
    }

    if( !result )
        return false;

    p->buffer.buffer( music.artist() );

    if( album_id != -1 )
        p->buffer.buffer( album_id , music.artist() , music.album() , music.year() );

    return true;
}

SSQLConnect::~SSQLConnect()
{
    p->db.close();
    delete p;
}
