#ifndef SDISCINFO_H
#define SDISCINFO_H

#include <QString>

#include "sdatabase_global.h"

class SDATABASESHARED_EXPORT SDiscInfo
{
public:
    enum DiscType{
        Unknown,
        Data,
        Audio,
        VCD,
        SVCD,
        DVD_VIDEO,
        BluRay_VIDEO
    };

    SDiscInfo( const QString & id = QString() );

    void setDiscId( const QString & disc );
    void setMd5Sum( const QString & md5 );
    void setLabel( const QString & label );
    void setAddress( const QString & address );
    void setPublisher( const QString & publisher );
    void setApplicationId( const QString & app_id );
    void setFormat(  const QString & format );
    void setCopyRight(  const QString & copyright );
    void setSysId( const QString & sys_id );
    void setSize( int size );
    void setFreeCapacity( int free_capacity );
    void setAppendable( bool appendable );
    void setType( DiscType type );
    void setTypeString( const QString & type );

    const QString & discId() const;
    const QString & md5Sum() const;
    const QString & label() const;
    const QString & address() const;
    const QString & publisher() const;
    const QString & applicationId() const;
    const QString & format() const;
    const QString & copyRight() const;
    const QString & sysId() const;
    int size() const;
    int freeCapacity() const;
    bool appendable() const;
    DiscType type() const;
    QString typeString() const;

private:
    QString  disc_id_str;
    QString  md5_sum_str;
    QString  label_str;
    QString  address_str;        // If is Iso Image
    QString  publisher_str;
    QString  app_id_str;
    QString  format_str;         // ISO , UDF , ....
    QString  copy_right_str;
    QString  sys_id_str;
    int      size_int;
    int      free_capacity_int;
    bool     appendable_bool;
    DiscType disc_type;
};

#endif // SDISCINFO_H
