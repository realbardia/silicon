#ifndef STINYFILEINFO_H
#define STINYFILEINFO_H

#include <QString>
#include "sdatabase_global.h"

class SDATABASESHARED_EXPORT STinyFileInfo
{
public:
    STinyFileInfo();

    void setID( int id );
    void setName( const QString & name );
    void setAddress( const QString & address );
    void setDiscID( const QString & disc );
    void setType( const QString & type );
    void setDirectory( bool stt );

    int ID() const;
    const QString & name() const;
    const QString & address() const;
    const QString & discID() const;
    const QString & type() const;
    bool isDirectory() const;

private:
    int id_int;
    QString name_str;
    QString address_str;
    QString disc_id_str;
    QString type_str;
    bool is_dir;
};

typedef QList<STinyFileInfo> SFileInfoList;

#endif // STINYFILEINFO_H
