#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QDialog>
#include <QIcon>
#include <QString>
#include <QHash>
#include <QList>

#include <SPlugin>
#include "spluginengine.h"
#include "perconf.h"

namespace Ui {
    class LoadedPlugins;
}

class PluginManager : public QObject
{
    Q_OBJECT
public:
    PluginManager( PerConf *conf , const QString & pluginsDirectory , QWidget *parent = 0 );
    ~PluginManager();

    void refreshUI();
    void detect();

    void setStyleSheet( const QString & style );

public slots:
    void show();
    void hide();

    void pluginStarted( SPlugin *plugin );
    void pluginStopped( SPlugin *plugin );

private slots:
    void currentCellChanged( int row , int col );
    void stop_start_CurrentItem();

private:
    void save();
    void loadSaved();

    bool defaultActived( const QString & name ) const;

private:
    Ui::LoadedPlugins *ui;
    QDialog *dialog;

    QList<SPlugin *> startedPlugins;
    QHash<QString,SPlugin *> loadedPlugins;
    QHash<QString,SPluginEngine *> plugins_hash;

    QString plugin_dir_str;
    PerConf *prc;

    QString style_sheet;
};

#endif // PLUGINMANAGER_H
