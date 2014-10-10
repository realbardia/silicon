#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <QWidget>
#include <QObject>
#include <QList>
#include <QHash>
#include <QString>
#include <QIcon>

#include <SApplication>
#include <SAboutData>
#include <SPictureWidget>

#include "sapplicationengine.h"
#include "perconf.h"

class QListWidgetItem;
namespace Ui{
    class AppManagerUi;
}

class ApplicationManager : public QObject
{
    Q_OBJECT
public:
    ApplicationManager( const QString & appDir , PerConf *conf , QWidget *parent = 0);
    ~ApplicationManager();

    const QList<SApplication *> & loadedApplications() const;
    const QList<SAboutData> & loadedApplicationsAbouts() const;
    const QList<SAboutData> & applicationsList() const;

    void detect();

    void registerAboutData( const SAboutData & about );
    QWidget *configure( const SAboutData & about );

    void setStyleSheet( const QString & style );

signals:
    void foundNewApplication( const SAboutData & about );

public slots:
    SApplication *loadApplication( const SAboutData & about , const QVariantList &args = QVariantList() );
    void applicationAdded( SApplication *app );
    void unregisterApplication( SApplication *app );

    void lock( SApplication *app );
    void unlock( SApplication *app );

    void show();
    void hide();

    void refreshUI();

private slots:
    void app_destroyed( SApplication *app );
    void currentRowChanged( int index );
    void killCurrentItem();

    bool check_depends( const SAboutData & about );

private:
    QList<SApplicationEngine *> engines_list;
    QList<SAboutData>           apps_list;
    QList<SAboutData>           loaded_apps_abouts;
    QList<SApplication *>       loaded_apps;
    QList<QListWidgetItem *>    apps_items;
    QList<SApplication *>       locked_apps;

    QString  app_dir;
    PerConf *prc;

    Ui::AppManagerUi *ui;
    QDialog          *dialog;
    SPictureWidget   *picture;

    QString style_sheet;

    int last_selected_row;
};

#endif // APPLICATIONMANAGER_H
