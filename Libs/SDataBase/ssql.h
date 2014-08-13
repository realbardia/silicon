#ifndef SSQL_H
#define SSQL_H

#include <QObject>

#include "sfileinfo.h"
#include "sdiscinfo.h"
#include "sdbtable.h"
#include "stinyfileinfo.h"

#include "sdatabase_global.h"

class SDATABASESHARED_EXPORT SSql : public QObject
{
    friend class SSqlSignalControler;

    Q_OBJECT
public:
    SSql(QObject *parent = 0);
    ~SSql();

    void lock();
    void unlock();

    bool locked();
    bool unlocked();

    void setDefaultSQLAddress( const QString & address );
    const QString & SQLAddress() const;

public slots:
    void insertDisc( const SDiscInfo & disc );
    void insertFile( const SFileInfo & file );

    void removeDisc( const QString & disc_id );

    void discsID();
    void discDetails( const QString & id , bool md5 = false );
    void fileDetails( int id );

    void filesOf( const STinyFileInfo & dir );
    void search( const QString & key , SFileInfo::FileCat cat = SFileInfo::Normal );

signals:
    void discInserted( const SDiscInfo & disc , bool result );
    void fileInserted( const SFileInfo & file , bool result );

    void discRemoved( const QString & disc_id , bool result );

    void discsIdDetected( const QStringList & list );
    void detailDetected( const SDiscInfo & disc );
    void detailDetected( const SFileInfo & file );

    void detectedFilesOf( const STinyFileInfo & dir , const SFileInfoList & files );
    void founded( const QString & key , const SFileInfoList & list );

private slots:

};

#endif // SSQL_H
