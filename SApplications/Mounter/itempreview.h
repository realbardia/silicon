#ifndef ITEMPREVIEW_H
#define ITEMPREVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QString>
#include <QScrollArea>
#include <QFrame>

#include <spicturewidget.h>
#include <sanimation.h>

class ItemPreview : public QWidget
{
    Q_OBJECT
public:
    ItemPreview(QWidget *parent = 0);

    QString name();
    QString address();
    QString mountPoint();
    int sizeKB();
    QString volumeLabel();
    QString applicationID();
    QString publisher();
    QString systemID();
    QString format();
    QString copyright();

    void reload();

signals:
    void eject();

public slots:
    void hideWithAnim();
    void showWithAnim();

    void setName( const QString & name );
    void setAddress( const QString & address );
    void setMountPoint( const QString & mountPoint );
    void setSize( int size_kb );
    void setVolumeID( const QString & volume );
    void setApplicationID( const QString & id );
    void setPublisher( const QString & publisher );
    void setSystemID( const QString & system );
    void setFormat( const QString & format );
    void setCopyright( const QString & copyright );

private slots:
    void animation_finished();
    void eject_clicked();

private:
    SPictureWidget *picture_widget;

    QLabel *name_label;
    QLabel *address_label;
    QLabel *mount_point_label;
    QLabel *size_label;
    QLabel *volume_label;
    QLabel *application_id_label;
    QLabel *publisher_label;
    QLabel *system_label;
    QLabel *format_label;
    QLabel *copyright_label;
    QWidget *data_area_widget;
    QScrollArea *data_area;

    QWidget *base_widget;
    QVBoxLayout *base_layout;

    QToolBar *actions_bar;
    QAction *hide_action;
    QAction *umount_action;

    QHBoxLayout *layout;
    QFormLayout *form_layout;

    SAnimation *animation;
};

#endif // ITEMPREVIEW_H
