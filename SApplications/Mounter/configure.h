#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QWidget>

#include "ui_configure.h"
#include "perconf.h"

class ConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    ConfigureWidget( MounterConf *conf = 0 ,QWidget *parent = 0 );

public slots:
    void setMasterMountPoint();
    void openFileManager( bool stt );
    void openLocation( bool stt );

signals:
    void configsChanged();

private:
    Ui::Configure *ui;

    MounterConf *personal;
};

#endif // CONFIGURE_H
