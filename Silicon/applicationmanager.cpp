#include "applicationmanager.h"
#include "ui_applicationmanager.h"
#include "depends.h"

#include <QMessageBox>
#include <QTimer>

#include <SMasterIcons>
#include <SDesktopFunctions>

ApplicationManager::ApplicationManager( const QString & appDir , PerConf *conf , QWidget *parent) :
    QObject(parent)
{
    app_dir    = appDir;
    prc        = conf;
    ui         = 0;
    dialog     = 0;

    last_selected_row = -1;
}

const QList<SApplication *> & ApplicationManager::loadedApplications() const
{
    return this->loaded_apps;
}

const QList<SAboutData> & ApplicationManager::loadedApplicationsAbouts() const
{
    return this->loaded_apps_abouts;
}

const QList<SAboutData> & ApplicationManager::applicationsList() const
{
    return this->apps_list;
}

SApplication *ApplicationManager::loadApplication( const SAboutData & about , const QVariantList & args )
{
    if( !about.multiRun() && loaded_apps_abouts.contains(about) )
    {
        int index = loaded_apps_abouts.indexOf(about);
        SApplication *app = loaded_apps.at(index);
            app->addRuntimeArgs( args );
            //app->focus();
/*
        if( about.privateStructure() )
            return 0;
*/
        return app;
    }

    if( !check_depends(about) )
        return 0;

    int index = apps_list.indexOf( about );
    SApplicationEngine *engine = engines_list.at( index );
        engine->application( args );

/*
    if( about.privateStructure() )
        return 0;
*/
    return loaded_apps.last();
}

bool ApplicationManager::check_depends( const SAboutData & about )
{
    QStringList not_found_bin;
    QStringList not_found_app;

    QStringList depends = about.binaryDepends();
    for( int i=0 ; i<depends.count() ; i++ )
    {
        QString app;
#ifdef Q_OS_WIN32
        QDir dir( SDesktopFunctions::binaryPath() );
            dir.cdUp();

        app = dir.path() + "/cdrtools/" + depends.at(i) + ".exe";
        app.replace( "/" , "\\" );
#else
        app = "/usr/bin/" + depends.at(i);
#endif

        if( !QFile::exists( app ) )
            not_found_bin << app;
    }


    depends = about.sappDepends();
    for( int i=0 ; i<depends.count() ; i++ )
    {
        QString name = depends.at(i);
        bool ok = false;
        for( int j=0 ; j<apps_list.count() ; j++ )
            if( apps_list.at(j).name() == name )
            {
                ok = true;
                break;
            }

        if( !ok )
            not_found_app << name;
    }


    if( !not_found_bin.isEmpty() || !not_found_app.isEmpty() )
    {
        Depends dialog;
            dialog.setBinaryDepends( not_found_bin );
            dialog.setSAppDepends( not_found_app );
            dialog.exec();

        return false;
    }

    return true;
}

void ApplicationManager::applicationAdded( SApplication *app )
{
    const SAboutData & about = app->parent();

    if( !apps_list.contains( about ) )
    {
        delete app;
        return ;
    }

    loaded_apps << app;
    loaded_apps_abouts << app->parent();

    if( dialog != 0 )
    {
        QListWidgetItem *item = new QListWidgetItem();
            item->setText( about.name() );
            item->setIcon( about.icon() );
            item->setToolTip( about.description() );

        ui->listWidget->addItem( item );
        apps_items << item;
    }

    QObject::connect( app , SIGNAL(destroyed(SApplication*)) , SLOT(app_destroyed(SApplication*)) );
}

void ApplicationManager::unregisterApplication( SApplication *app )
{
    if( !loaded_apps.contains(app) )
        return ;

    delete app;
}

void ApplicationManager::lock( SApplication *app )
{
    if( locked_apps.contains( app ) )
        return ;

    locked_apps << app;
}

void ApplicationManager::unlock( SApplication *app )
{
    locked_apps.removeOne( app );
}

void ApplicationManager::show()
{
    if( dialog == 0 )
    {
        picture = new SPictureWidget();
            picture->setFixedSize( 25 , 25 );

        dialog = new QDialog( static_cast<QWidget *>(this->parent()) );
            dialog->setStyleSheet( style_sheet );

        ui     = new Ui::AppManagerUi;
            ui->setupUi( dialog );
            ui->kill_btn->setIcon( SMasterIcons::icon( QSize(48,48) , "process-stop.png" ) );
            ui->horizontalLayout->insertWidget( 0 , picture );

        dialog->setWindowIcon( SMasterIcons::icon( QSize(48,48) , "applications-other.png" ) );
        dialog->setWindowFlags( Qt::Dialog );

        for( int i=0 ; i<loaded_apps.count() ; i++ )
        {
            const SAboutData & about = loaded_apps.at(i)->parent();

            QListWidgetItem *item = new QListWidgetItem();
                item->setText( about.name() );
                item->setIcon( about.icon() );
                item->setToolTip( about.description() );

            ui->listWidget->addItem( item );
            apps_items << item;
        }

        QObject::connect( ui->buttonBox  , SIGNAL(rejected())             , dialog , SLOT(close())                );
        QObject::connect( ui->listWidget , SIGNAL(currentRowChanged(int)) , this   , SLOT(currentRowChanged(int)) );
        QObject::connect( ui->kill_btn   , SIGNAL(clicked())              , this   , SLOT(killCurrentItem())      );
    }

    dialog->show();
}

void ApplicationManager::currentRowChanged( int index )
{
    if( index < 0 )
        return ;

    SApplication *app = loaded_apps.at( index );
    int opened_pages = app->numberOfAllChilds();
    int runner_objs  = app->numberOfAllSubObjects();

    ui->PagesNumberLabel->setText( tr("Created %1 sub objects and %2 Opened pages").arg( QString::number(runner_objs) , QString::number(opened_pages) ) );

    if( index != last_selected_row )
        picture->drawIcon( app->parent().icon() , QSize(25,25) );

    if( !dialog->isHidden() )
        QTimer::singleShot( 1000 , this , SLOT(refreshUI()) );

    last_selected_row = index;
}

void ApplicationManager::refreshUI()
{
    if( dialog == 0 )
        return ;

    int index = ui->listWidget->currentRow();
    if( index < 0 )
        return ;

    currentRowChanged( index );
}

void ApplicationManager::killCurrentItem()
{
    if( dialog == 0 )
        return ;

    int index = ui->listWidget->currentRow();
    if( index < 0 )
        return ;

    SApplication *app = loaded_apps.at( index );

    if( locked_apps.contains(app) )
    {
        QMessageBox::warning( dialog , tr("Warning") , tr("Warning, It's System Application,\nYou can't kill this application at this time.") );
        return ;
    }

    app->setQuitOnLastPageClosed( false );
    delete app;
}

void ApplicationManager::hide()
{
    if( dialog == 0 )
        return ;

    dialog->hide();
}

void ApplicationManager::app_destroyed( SApplication *app )
{
    int index = loaded_apps.indexOf( app );
    if( dialog != 0 )
    {
        delete ui->listWidget->takeItem( index );
        apps_items.removeAt( index );
    }

    loaded_apps.removeAt( index );
    loaded_apps_abouts.removeAt( index );
}

void ApplicationManager::detect()
{
    QList<QString> tmp;
#ifdef Q_OS_LINUX
    tmp = QDir(app_dir).entryList( QStringList()<<"*.so" );
#else
#ifdef Q_OS_WIN32
    tmp = QDir(app_dir).entryList( QStringList()<<"*.dll" );
#else
#ifdef Q_OS_MAC
    tmp = QDir(app_dir).entryList( QStringList()<<"*.dylib" );
#endif
#endif
#endif

    for( int i=0 ; i<tmp.count() ; i++ )
    {
        SApplicationEngine *app = new SApplicationEngine( app_dir + '/' + tmp.at(i) );

        if( app->isApplication() && !apps_list.contains(app->about()) )
        {
            engines_list << app;
            apps_list    << app->about();

            emit this->foundNewApplication( app->about() );
        }
    }
}

QWidget *ApplicationManager::configure( const SAboutData & about )
{
    int index = apps_list.indexOf( about );

    SApplicationEngine *engine = engines_list.at(index);

    if( !engine->hasConfigure() )
        return 0;

    return engine->configure();
}

void ApplicationManager::registerAboutData( const SAboutData & about )
{
    engines_list << 0;
    apps_list << about;
}

void ApplicationManager::setStyleSheet( const QString & style )
{
    style_sheet = style;
    if( dialog == 0 )
        return ;

    dialog->setStyleSheet( style_sheet );
}

ApplicationManager::~ApplicationManager()
{
    if( ui != 0 )
        delete ui;
}
