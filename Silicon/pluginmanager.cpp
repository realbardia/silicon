#include "pluginmanager.h"
#include "ui_loadedplugins.h"

#include <QDir>
#include <SColor>
#include <SMasterIcons>

PluginManager::PluginManager( PerConf *conf , const QString & pluginsDirectory , QWidget *parent ) :
    QObject(parent),
    plugin_dir_str( pluginsDirectory )
{
    dialog = 0;
    ui     = 0;
    prc    = conf;
}

void PluginManager::detect()
{
    QList<QString> tmp;
#ifdef Q_OS_LINUX
    tmp = QDir(plugin_dir_str).entryList( QStringList()<<"*.so" );
#else
#ifdef Q_OS_WIN32
    tmp = QDir(plugin_dir_str).entryList( QStringList()<<"*.dll" );
#else
#ifdef Q_OS_MAC
    tmp = QDir(plugin_dir_str).entryList( QStringList()<<"*.dylib" );
#endif
#endif
#endif

    for( int i=0 ; i<tmp.count() ; i++ )
    {
        if( plugins_hash.value(tmp.at(i)) )
            continue ;

        SPluginEngine *plugin_eng = new SPluginEngine( plugin_dir_str + '/' + tmp.at(i) );
        if( !plugin_eng->isPlugin() )
            continue;

        SPlugin *plugin = plugin_eng->plugin();
            plugin->setParent( this );

        plugins_hash.insert(  plugin->name() , plugin_eng );
        loadedPlugins.insert( plugin->name() , plugin );

        if( plugin->isStarted() )
            pluginStarted( plugin );
    }

    loadSaved();
}

void PluginManager::refreshUI()
{
    if( dialog == 0 )
        return ;

    int index = ui->tableWidget->currentRow();
    if( index < 0 )
        return ;

    currentCellChanged( index , 0 );
}

void PluginManager::currentCellChanged( int row , int )
{
    if( ui->tableWidget->rowCount() <= row )
        return ;

    QTableWidgetItem *item = ui->tableWidget->item( row , 0 );
    SPlugin *plugin = loadedPlugins.value( item->text() );

    if( startedPlugins.contains( plugin ) )
    {
        ui->enable_btn->setText( tr("Stop") );
        ui->enable_btn->setIcon( SMasterIcons::icon( QSize(48,48) , "media-playback-stop.png" ) );
    }
    else
    {
        ui->enable_btn->setText( tr("Start") );
        ui->enable_btn->setIcon( SMasterIcons::icon( QSize(48,48) , "media-playback-start.png" ) );
    }


    ui->plugin_name->setText( item->text() );
}

void PluginManager::stop_start_CurrentItem()
{
    int index = ui->tableWidget->currentRow();
    if( index < 0 )
        return ;

    QTableWidgetItem *item = ui->tableWidget->item( index , 0 );
    SPlugin *plugin = loadedPlugins.value( item->text() );

    if( plugin->isStarted() )
        plugin->stop();
    else
        plugin->start();
}

void PluginManager::pluginStarted( SPlugin *plugin )
{
    if( !loadedPlugins.values().contains(plugin) )
        return ;

    if( startedPlugins.contains(plugin) )
        return ;

    bool error = false;

    // Disable Conflicted Plugins ======//
    for( int i=0 ; i<plugin->conflicts().count() ; i++ )
    {
        QString  name     = plugin->conflicts().at(i);
        if( !loadedPlugins.contains(name) )
            continue;

        SPlugin *c_plugin = loadedPlugins.value( name );
        if( c_plugin->isStarted() )
            error = error || c_plugin->stop();
    }

    for( int i=0 ; i<startedPlugins.count() ; i++ )
    {
        if( startedPlugins.at(i)->conflicts().contains(plugin->name()) )
            error = error || startedPlugins.at(i)->stop();
    }
    // END ====//



    // Enable Dependes Plugins =======//
    for( int i=0 ; i<plugin->depends().count() ; i++ )
    {
        QString  name     = plugin->depends().at(i);
        SPlugin *d_plugin = loadedPlugins.value( name );
        if( !d_plugin->isStarted() )
            error = error || d_plugin->start();
    }
    // END ====//

/*
    if( dialog != 0 )
    {
        int index = -1;
        for( int i=0 ; i<ui->tableWidget->rowCount() ; i++ )
            if( ui->tableWidget->item(i,0)->text() == plugin->name() )
                index = i;

        if( index != -1 )
        {
            SColor color = dialog->palette().base().color();
                color = color * 0.92;

            for( int i=0 ; i<ui->tableWidget->columnCount() ; i++ )
                ui->tableWidget->item(index,i)->setBackground( color );
        }
    }*/


    startedPlugins << plugin;
    refreshUI();
}

void PluginManager::pluginStopped( SPlugin *plugin )
{
    if( !loadedPlugins.values().contains(plugin) )
        return ;

    if( !startedPlugins.contains(plugin) )
        return ;

    bool error = false;

    // Enable Depended Plugins =======//
    for( int i=0 ; i<startedPlugins.count() ; i++ )
    {
        if( startedPlugins.at(i)->depends().contains(plugin->name()) )
            error = error || startedPlugins.at(i)->stop();
    }
    // END ====//

    startedPlugins.removeOne( plugin );
    refreshUI();
}

void PluginManager::save()
{
    const QList<SPlugin*> & list = loadedPlugins.values();

    QStringList actives;
    QStringList deactives;

    for( int i=0 ; i<list.count() ; i++ )
    {
        SPlugin *plugin = list.at(i);
        if( plugin->isStarted() )
            actives << plugin->name();
        else
            deactives << plugin->name();
    }

    prc->setOption( PerConf::ACTIVE_PLUGINS   , actives.join(",")   );
    prc->setOption( PerConf::DEACTIVE_PLUGINS , deactives.join(",") );
}

void PluginManager::loadSaved()
{
    QStringList actives   = prc->readOption( PerConf::ACTIVE_PLUGINS   ).split(",",QString::SkipEmptyParts);
    QStringList deactives = prc->readOption( PerConf::DEACTIVE_PLUGINS ).split(",",QString::SkipEmptyParts);

    const QList<SPlugin*> & list = loadedPlugins.values();

    for( int i=0 ; i<list.count() ; i++ )
    {
        SPlugin *plugin = list.at(i);
        QString  name   = plugin->name();

        if( actives.contains(name) && !deactives.contains(name) )
        {
            pluginStarted( plugin );
            plugin->start();
        }
        else if( !actives.contains(name) && deactives.contains(name) )
        {
            pluginStopped( plugin );
            plugin->stop();
        }
        else if( defaultActived(name) )
        {
            pluginStarted( plugin );
            plugin->start();
        }
    }
}

bool PluginManager::defaultActived( const QString & name ) const
{
    return (
                name == "CdRecord"            ||
                name == "AudioCdRecord"       ||
                name == "MkDiscFs"            ||
                name == "MkIsoFs"             ||
                name == "ReadCd"              ||
                name == "Eraser"              ||
                name == "Single-Inner-Dialog" ||
                name == "MPG123"              ||
                name == "LyricBrowser"        ||
                name == "NowPlaying"          ||
                name == "TagargAudioDisc"     ||
#ifdef Q_OS_LINUX
                name == "FUseIso"             ||
                name == "UDisks"
#else
#ifdef Q_OS_WIN32
                name == "Win32-Device-Notifier"
#endif
#endif
           );
}

void PluginManager::show()
{
    if( dialog == 0 )
    {
        dialog = new QDialog( static_cast<QWidget *>(this->parent()) );
            dialog->setStyleSheet( style_sheet );

        ui     = new Ui::LoadedPlugins;
            ui->setupUi( dialog );

        dialog->setWindowIcon( SMasterIcons::icon( QSize(48,48) , "preferences-plugin.png" ) );
        dialog->setWindowFlags( Qt::Dialog );

        QList<SPlugin *> list = loadedPlugins.values();
        for( int i=0 ; i<list.count() ; i++ )
        {
            SPlugin *plugin = list.at(i);

            QTableWidgetItem *t_item_name = new QTableWidgetItem();
                t_item_name->setText(      plugin->name()        );
                t_item_name->setIcon(      plugin->icon()        );
                t_item_name->setToolTip(   plugin->description() );

            QTableWidgetItem *t_item_desc = new QTableWidgetItem();
                t_item_desc->setText(      plugin->description() );
                t_item_desc->setToolTip(   plugin->description() );

            ui->tableWidget->setRowCount( ui->tableWidget->rowCount()+1 );
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1 , 0 , t_item_name );
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1 , 1 , t_item_desc );
        }

        QObject::connect( ui->buttonBox , SIGNAL(rejected()) , dialog , SLOT(close()) );

        QObject::connect( ui->tableWidget , SIGNAL(currentCellChanged(int,int,int,int)) , SLOT(currentCellChanged(int,int)) );
        QObject::connect( ui->enable_btn  , SIGNAL(clicked())                           , SLOT(stop_start_CurrentItem())    );
    }

    dialog->show();
}

void PluginManager::hide()
{
    if( dialog == 0 )
        return ;

    dialog->hide();
}

void PluginManager::setStyleSheet( const QString & style )
{
    style_sheet = style;
    if( dialog == 0 )
        return ;

    dialog->setStyleSheet( style_sheet );
}

PluginManager::~PluginManager()
{
    if( ui != 0 )
        delete ui;

    save();
}
