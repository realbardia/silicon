#ifndef SISOINFO_H
#define SISOINFO_H

#include <QString>
#include <QWidget>
#include <QObject>

#include "sdeviceitem.h"

#include "SiDi_global.h"

class SIsoInfoPrivate;

class SIDI_EXPORT SIsoInfo : public QObject
{
    Q_OBJECT
public:
    SIsoInfo( QObject *parent = 0 );
    ~SIsoInfo();

    void setFile( const QString & file );
    void setDevice( const SDeviceItem & disc );

    const QString & applicationID() const;
    const QString & format() const;
    const QString & system() const;
    const QString & volume() const;
    const QString & publisher() const;
    const QString & dataPreparer() const;
    const QString & copyright() const;
    const QString & abstract() const;
    int logicalBlockSize() const;
    int volumeSize() const;

    const QString & allInformations() const;

private slots:
    void readInfos_prev();

signals:
    void infosReaded( const QString & infos );
    void applicationIdReaded( const QString & infos );
    void formatUpdated( const QString & str );
    void systemUpdated( const QString & str );
    void volumeUpdated( const QString & str );
    void publisherUpdated( const QString & str );
    void dataPreparerUpdated( const QString & str );
    void copyrightUpdated( const QString & str );
    void abstractUpdated( const QString & str );
    void logicalBlockSizeUpdated( int value );
    void volumeSizeUpdated( int value );

protected:
    void readInfos();
    QString findItem( QString str );

private:
    SIsoInfoPrivate *p;
};

#endif // SISOINFO_H
