#include "configure.h"

#include <QFileDialog>

ConfigureWidget::ConfigureWidget( MounterConf *conf , QWidget *parent ) : QWidget( parent )
{
    personal = conf;
    ui = new Ui::Configure;
        ui->setupUi( this );

    ui->mmpoint_line->setText( personal->readOption( MounterConf::MASTER_POINT ) );
    ui->open_fm_check->setChecked( personal->readOption( MounterConf::OPEN_FILE_MANAGER ) == "true" );
    ui->open_location_check->setChecked( personal->readOption( MounterConf::CLICK_ACTION ) == "open" );

    QObject::connect( ui->mmpoint_button       , SIGNAL(clicked())     , this , SLOT(setMasterMountPoint()) );
    QObject::connect( ui->open_fm_check        , SIGNAL(toggled(bool)) , this , SLOT(openFileManager(bool)) );
    QObject::connect( ui->open_location_check  , SIGNAL(toggled(bool)) , this , SLOT(openLocation(bool))    );
}

void ConfigureWidget::setMasterMountPoint()
{
    QString path;
        path = QFileDialog::getExistingDirectory( this , "Select Master MountPoint" , ui->mmpoint_line->text() );

    if( path.isEmpty() )
        return ;

    personal->setOption( MounterConf::MASTER_POINT , path );
    ui->mmpoint_line->setText( path );

    personal->save();
    emit this->configsChanged();
}

void ConfigureWidget::openFileManager( bool stt )
{
    ui->open_fm_check->setChecked( stt );

    QString str = "false";
    if( stt )
        str = "true";

    personal->setOption( MounterConf::OPEN_FILE_MANAGER , str );
    personal->save();

    emit this->configsChanged();
}

void ConfigureWidget::openLocation( bool stt )
{
    ui->open_location_check->setChecked( stt );

    QString str = "open";
    if( ui->unmount_check->isChecked() )
        str = "unmount";

    personal->setOption( MounterConf::CLICK_ACTION , str );
    personal->save();

    emit this->configsChanged();
}
