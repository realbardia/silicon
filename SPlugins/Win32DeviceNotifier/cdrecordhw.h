#ifndef CDRECORDHW_H
#define CDRECORDHW_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>

#include <SDeviceItem>
#include <STime>

class CdRecordHWPrivate;

class CdRecordHW : public QObject
{

    Q_OBJECT
    Q_DISABLE_COPY( CdRecordHW )

public:
    CdRecordHW( QObject *parent = 0 );
    ~CdRecordHW();

    void prcap( SDeviceItem item );
    const QList<SDeviceItem> & devicesList() const;

public slots:
    void scanbus();

signals:
    void busScaned();
    void devicesScaned();
    void prcapFinished( CdRecordHW *cdrecord , const QString & str );
    void deviceSpeedsDetected( CdRecordHW *cdrecord , QList<int> list );

private slots:
    void scanbus_priv();
    void scanDevices_priv();
    void prcap_prev();

private:
    void init();
    QList<int> detectSpeedFromString( const QStringList & list );

private:
    CdRecordHWPrivate *p;
};

#endif // CDRECORDHW_H
