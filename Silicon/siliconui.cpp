#include "siliconui.h"

#include <QToolBar>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QListWidgetItem>
#include <QAction>
#include <QTimer>
#include <QMimeData>
#include <QHash>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QDebug>

#include <SCatWidget>
#include <SMenu>
#include <SMenuPanel>
#include <SMasterIcons>
#include <STabBar>
#include <SContextButton>
#include <Silicon>
#include <SInfoWidget>
#include <SPushButton>
#include <libsilicon_prev.h>
#include <SAuthorInfo>
#include <SAboutData>
#include <SStyleSheet>
#include <SVariableConverter>
#include <SArgumentList>
#include <SSql>
#include <SDialog>

#ifdef Q_OS_WIN32
    #include <QtWin/qtwin.h>
#endif

#include "sdefaults.h"
#include "menubarmanager.h"
#include "fileloaderengine.h"
#include "pluginmanager.h"
#include "applicationmanager.h"
#include "sapplicationsconfigure.h"
#include "configure.h"
#include "about.h"
#include "sthemeengine.h"
#include "sprogresslist.h"
#include "discmanager.h"
#include "dialogmanager.h"
#include "toolbarmanager.h"
#include "statusbarmanager.h"
#include "appmainwindow.h"


bool stringToBoolian( const QString & str )
{
    bool a = false;
    if( str.toLower() == "true" )
        a = true ;

    return a;
}


class SiliconUIPrivate
{
public:
    perConf    *prc;
    QStringList favorites;
    SSql       *sql;

    SMasterIcons      *master_icons;
    SiliconSignalsBox *signals_box;
    SiliconDataBox    *data_box;
    SiliconSet         silicon_set;

    QToolBar    *toolbar;
    QFrame      *main;
    QVBoxLayout *base_layout;

    QAction *switch_type_action;
    QAction *favorite_action;
    QAction *find_action;
    QAction *process_action;
    QAction *configure_action;
    QAction *plugins_action;
    QAction *applications_action;
    QAction *about_action;
    QAction *about_qt_action;
    QAction *about_sialan_action;
    QAction *quit_action;

    SCatWidget     *cat_widget;
    SInfoWidget    *info_widget;
    SPushButton    *menu_button;
    SContextButton *context_button;
    SMenu          *menu;
    STabBar        *tab_bar;
    SMenuPanel     *menu_panel;
    SProgressList  *process_list;

    SApplicationsConfigure *sapps_confs;
    About                  *about_ui;
    Configure              *configure_ui;
    MenuBarManager         *menubar_manager;
    FileLoaderEngine       *file_loader;
    PluginManager          *plugin_manager;
    ApplicationManager     *application_manager;
    DiscManager            *disc_manager;
    DialogManager          *dialog_manager;
    ToolBarManager         *toolbar_manager;
    StatusBarManager       *status_bar_manager;

    SDefaults  defaults;
    SAboutData silicon_about;

    QList<SPage *>                       page_lists;
    QList<SPage *>                       windowed_page_list;
    QHash<SPage *,AppMainWindow *>       page_to_window_hash;
    QHash<QListWidgetItem *,SAboutData > apps_hash;

    SApplication *Silicon;
    SPage        *Process_page;
    SPage        *Configure_page;
    SPage        *About_page;

    QPalette palette;

    QString favorite_text;
    QString process_text;

    QString about_style;

    bool gradiant_back;
#ifdef Q_OS_WIN32
    bool aero_disabled;
#endif
};



SiliconUI::SiliconUI( perConf *prcnf , QWidget *parent)
    : QMainWindow(parent)
{
    p = new SiliconUIPrivate;
        p->prc = prcnf;


    resize( 1024 , 600 );
    setWindowTitle( tr("Silicon") );
    setAcceptDrops( true );
    setUnifiedTitleAndToolBarOnMac( true );
    setStyleSheet( "QToolBar{border-style:solid}" );

    init_system();
    init_ui();
    init_about_data();
    init_actions();
    init_presonal_confs();
    init_graphic_effects();
    init_style_sheet();
    init_connections();
    init_last_chance();

    QIcon icon = SMasterIcons::icon( QSize(48,48) , "silicon.png" );
    if( icon.isNull() )
        icon = QIcon(":/files/Pixs/icon.png");

    setWindowIcon( icon );
}

void SiliconUI::init_about_data()
{
    SAuthorInfo bardia;
        bardia.setFirstName( "Bardia" );
        bardia.setLastName( "Daneshvar" );
        bardia.setNickName( "realbardia" );
        bardia.setAuthorIcon( QIcon(":/files/Pixs/authors/bardia-icon.jpg") );
        bardia.setAvatarPixmap( QPixmap(":/files/Pixs/authors/bardia-av.jpg") );
        bardia.setBornDate( QDate( 1990 , 9 , 18 ) );
        bardia.setHomePage( "http://getSilicon.org" );
        bardia.setBlog( "http://realbardia.com" );
        bardia.setMail( "bardia.daneshvar@gmail.com" );
        bardia.setAIM( "bardiax" );
        bardia.setGTalkIM( "bardia.daneshvar" );
        bardia.setMSN( "realbardia@live.com" );
        bardia.setSkypeIM( "bardiax" );
        bardia.setYahooIM( "bard1ax" );
        bardia.setRollList( QStringList()<<"Maintainer"<<"Developer" );
        bardia.setProgramingLanguagesList( QStringList()<<"C"<<"C++"<<"C#"<<"Qt"<<"HTML"<<"Css"<<"JavaScript" );
        bardia.setProjectsLists( QStringList() );


    SAuthorInfo milad_kakoli;
        milad_kakoli.setFirstName( "Milad" );
        milad_kakoli.setLastName( "Kakoli" );
        milad_kakoli.setNickName( "Kitten" );
        milad_kakoli.setBornDate( QDate( 1991 , 3 , 27 ) );
        milad_kakoli.setHomePage( "http://getSilicon.org" );
        milad_kakoli.setBlog( "http://kitten.mihanblog.com" );
        milad_kakoli.setMail( "hellcrueldictator@yahoo.com" );
        milad_kakoli.setAIM( "AIM ID" );
        milad_kakoli.setGTalkIM( "hellcrueldictator" );
        milad_kakoli.setMSN( "Live ID" );
        milad_kakoli.setSkypeIM( "Skype ID" );
        milad_kakoli.setYahooIM( "hellcrueldictator" );
        milad_kakoli.setProgramingLanguagesList( QStringList()<<"C"<<"C++"<<"Java"<<"Qt" );
        milad_kakoli.setRollList( QStringList()<<"Developer: Translation Support" );

    QHash<SAboutData::TranslatorDatas,QVariant> bardia_tr;
        //bardia_tr.insert( SAboutData::Image    , QVariant() );
        bardia_tr.insert( SAboutData::Country  , "Iran" );
        bardia_tr.insert( SAboutData::Language , "Persian" );
        bardia_tr.insert( SAboutData::Name     , "Bardia Daneshvar" );
        bardia_tr.insert( SAboutData::Email    , "bardia.daneshvar@gmail.com" );
        bardia_tr.insert( SAboutData::HomePage , "getSilicon.org" );

    QHash<SAboutData::TranslatorDatas,QVariant> frannoe_tr;
        //frannoe_tr.insert( SAboutData::Image    , QVariant() );
        frannoe_tr.insert( SAboutData::Country  , "Spania" );
        frannoe_tr.insert( SAboutData::Language , "Spanish" );
        frannoe_tr.insert( SAboutData::Name     , "Francisco Luque" );
        frannoe_tr.insert( SAboutData::HomePage , "ubuntu-cosillas.blogspot.com.es" );

    QHash<SAboutData::TranslatorDatas,QVariant> cheng_hong_tr;
        //cheng_hong_tr.insert( SAboutData::Image    , QVariant() );
        cheng_hong_tr.insert( SAboutData::Country  , "China" );
        cheng_hong_tr.insert( SAboutData::Language , "Chinese" );
        cheng_hong_tr.insert( SAboutData::Name     , "Cheng-Hong Wu" );
        cheng_hong_tr.insert( SAboutData::HomePage , "stu2731652@gmail.com" );

    QHash<SAboutData::TranslatorDatas,QVariant> leo_tr;
        //cheng_hong_tr.insert( SAboutData::Image    , QVariant() );
        leo_tr.insert( SAboutData::Country  , "German" );
        leo_tr.insert( SAboutData::Language , "Germany" );
        leo_tr.insert( SAboutData::Name     , "leo" );
        leo_tr.insert( SAboutData::HomePage , "anleo@solnet.ch" );


    //Config Silicon About
        p->silicon_about.setName( "Silicon" );
        p->silicon_about.setTranslatedName( tr("Silicon") );
        p->silicon_about.setIcon( QIcon(":/files/Pixs/icon.png") );
        p->silicon_about.setPixmap( QPixmap(":/files/Pixs/image.png") );
        p->silicon_about.setOrganizer( "Sialan Labs" );
        p->silicon_about.setCopyRight( "Sialan Labs" );
#ifdef Q_OS_LINUX
        p->silicon_about.setVersion( "2.1.0" );
#else
        p->silicon_about.setVersion( "pre-alpha" );
#endif
        p->silicon_about.setHomePage( "http://getSilicon.org" );
        p->silicon_about.setMail( "contact@sialan.org" );
        p->silicon_about.setDescription( "" );
        p->silicon_about.setAbout( tr("Silicon is set of tools by Sialan Labs to manage and organize your optical discs like CDs ,DVDs and Blu-rays. "
                                "Silicon target is made optical discs managing easy.") );
        p->silicon_about.setLicense( Silicon::GPLLicense() );
        p->silicon_about.addTranslator( leo_tr );
        p->silicon_about.addTranslator( bardia_tr );
        p->silicon_about.addTranslator( frannoe_tr );
        p->silicon_about.setAuthors( QList<SAuthorInfo>()<<bardia<<milad_kakoli );
        p->silicon_about.setDescription( QString() + "<center><b>" + tr("Think beyond The Borders") + "</b></center><br />"
                                  + " <b>" + tr("Description :") + "</b><br />"
                                  + tr(" Silicon is set of tools by Sialan Labs to manage and organize your optical discs like CDs ,DVDs and Blu-rays.<br />"
                                    " Silicon target is made optical discs managing easy.<br /><br />")
#ifdef Q_OS_LINUX
                                  + " <b>" + tr("Version :") + "</b> 2.1.0 <br /><br />"
#else
                                  + " <b>" + tr("Version :") + "</b> pre-alpha <br /><br />"
#endif
                                  + " <b>" + tr("License :") + "</b> GPL V3 <br /><br />"
                                  + " <b>" + tr("Authors :") + "</b><br />"
                                  + " -> Sialan Labs");
        p->silicon_about.setCommandMap( "This is Silicon command line options:\n\n"
                                        "file1 file2 ...\t\t\tLoad files via applications that set in the file association.\n\n"
                                        "-L   ApplicationName   arguments\tLoad ApplicationName application with \"arguments\" arguments.");


    p->info_widget->setTitle( p->silicon_about.name() );
    p->info_widget->setPicture( p->silicon_about.pixmap() );
    p->info_widget->setDescription( p->silicon_about.description() );
}

void SiliconUI::init_ui()
{
    p->process_text = tr("Processes");
    p->favorite_text = tr("Favorites");

    p->master_icons = new SMasterIcons( p->defaults.Share_Icons_Directory_Path + "/oxygen" );
        p->master_icons->lock();

    p->info_widget = new SInfoWidget();
        p->info_widget->setMaximumWidth( 250 );

    p->menu_button = new SPushButton();
        p->menu_button->setText("");
        p->menu_button->setMinimumSize(QSize(52,40));
        p->menu_button->setMaximumSize(QSize(52,40));
        p->menu_button->setIcon(QIcon(":/files/Pixs/icon.png"));
        p->menu_button->setIconSize(QSize(32,32));
        p->menu_button->setFlat( true );
#ifdef Q_OS_MAC
        p->menu_button->setFocusPolicy(Qt::ClickFocus);
#else
        p->menu_button->setFocusPolicy(Qt::NoFocus);
#endif

    p->context_button = new SContextButton();
        p->context_button->setIcon( SMasterIcons::icon( QSize(48,48) , "configure.png" ) );
        p->context_button->setIconSize( QSize(22,22) );
        p->context_button->setMaximumSize( QSize(47,32) );
        p->context_button->setMinimumSize( QSize(47,32) );
        p->context_button->setFocusPolicy(Qt::NoFocus);
        p->context_button->setFlat( true );

    p->tab_bar = new STabBar();
        p->tab_bar->addWidgetBeforeTab( p->menu_button );
        p->tab_bar->addWidgetAfterTab( p->context_button );

    p->toolbar = new QToolBar();
        p->toolbar->addWidget( p->tab_bar->takeTabBar() );
        p->toolbar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
        p->toolbar->setMovable( false );
        p->toolbar->setContextMenuPolicy( Qt::CustomContextMenu );
        p->toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );
        p->toolbar->setLayoutDirection( Qt::LeftToRight );

    p->main = new QFrame();
    p->toolbar_manager    = new ToolBarManager();
    p->status_bar_manager = new StatusBarManager();

    setCentralWidget( p->main );
    addToolBar( p->toolbar );

    p->base_layout = new QVBoxLayout( p->main );
        p->base_layout->addWidget( p->toolbar_manager );
        p->base_layout->addWidget( p->tab_bar );
        p->base_layout->addWidget( p->status_bar_manager );
        p->base_layout->setContentsMargins( 0 , 0 , 0 , 0 );

    p->cat_widget = new SCatWidget();
        p->cat_widget->addWidgetToRight( p->info_widget );
        p->cat_widget->setCatWidth( 175 );
        p->cat_widget->setCatIconSize( QSize(32,32) );
        p->cat_widget->setListIconSize( QSize(48,48) );
        p->cat_widget->setIconGridSize( QSize(128,64) );
        p->cat_widget->setListGridSize( QSize(256,52) );
        p->cat_widget->setShortGridSize( QSize(256,52) );
        p->cat_widget->addCat( SMasterIcons::icon( QSize(48,48) , "favorites.png" ) , p->favorite_text );

    p->menu = new SMenu(this);
        p->menu->snapTo(this, p->menu_button->minimumHeight()+2 ,0,0,0);
        p->menu->showMenu();
        p->menu->setMainWidget( p->cat_widget );
        p->menu->setLayoutDirection( Qt::LeftToRight );

    p->menu_panel = new SMenuPanel( this );

    p->menubar_manager = new MenuBarManager( this->menuBar() , this );
    p->file_loader     = new FileLoaderEngine( p->application_manager );
    p->sapps_confs     = new SApplicationsConfigure( p->prc , p->tab_bar , p->data_box , this );
    p->disc_manager    = new DiscManager( p->cat_widget , p->info_widget , this );
    p->dialog_manager  = new DialogManager( this , p->main );

    menuBar()->hide();

    p->configure_ui = new Configure( p->prc , this );
        p->configure_ui->setWindowTitle( tr("Configure") );
        //p->configure_ui->setMinimumSize( QSize(850,500) );
        //p->configure_ui->setMaximumSize( QSize(850,500) );
        p->configure_ui->setWindowFlags( Qt::Dialog );
        p->configure_ui->addItem( SMasterIcons::icon( QSize(48,48) , "preferences-desktop-filetype-association.png" ) ,tr("File Association") ,p->file_loader );
        p->configure_ui->addItem( SMasterIcons::icon( QSize(48,48) , "applications-other.png" ) ,tr("Applications") ,p->sapps_confs );
        p->configure_ui->hide();

    p->about_ui     = 0;
    p->process_list = new SProgressList();

    p->info_widget->setFocus();

#ifdef Q_OS_WIN32
    p->aero_disabled = !QtWin::extendFrameIntoClientArea( this );
#endif
}

void SiliconUI::init_system()
{
    p->signals_box = new SiliconSignalsBox;
    p->silicon_set.setSiliconSignalsBox( p->signals_box );

    p->plugin_manager      = new PluginManager( p->prc , p->defaults.Silicon_Plugins_Directory , this );
    p->application_manager = new ApplicationManager( p->defaults.Silicon_Application_Directory , p->prc , this );

    p->data_box = new SiliconDataBox( p->application_manager->applicationsList() , p->application_manager->loadedApplicationsAbouts() );
        p->data_box->config      = p->defaults.Home_Path;
        p->data_box->main_window = this;

    p->silicon_set.setSiliconDataBox( p->data_box );
    p->silicon_set.lock();
}

void SiliconUI::init_actions()
{
    p->find_action = new QAction( tr("find") , this );
        p->find_action->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_F) );

    p->switch_type_action = new QAction( SMasterIcons::icon(QSize(48,48) , "window-duplicate.png") , tr("Switch page type") , this );
        p->switch_type_action->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_W ) );

    p->process_action = new QAction(SMasterIcons::icon( QSize(48,48) , "cpu.png" ), p->process_text, this);
        p->process_action->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_P) );

    p->configure_action = new QAction(SMasterIcons::icon( QSize(48,48) , "configure.png" ), tr("Configure Silicon"), this);
        p->configure_action->setShortcut( QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C) );

    p->applications_action = new QAction(SMasterIcons::icon( QSize(48,48) , "applications-other.png" ), tr("Task Manager"), this);
    p->plugins_action = new QAction(SMasterIcons::icon( QSize(48,48) , "preferences-plugin.png" ), tr("Loaded Plugins"), this);

    p->about_qt_action = new QAction(QIcon(":/files/Pixs/QT.png"), tr("About Qt"), this);
    p->about_sialan_action = new QAction(SMasterIcons::icon( QSize(48,48) , "help-about.png" ), tr("About Sialan labs"), this);
    p->about_action = new QAction(SMasterIcons::icon( QSize(48,48) , "help-about.png" ), tr("About"), this);

    p->quit_action = new QAction(SMasterIcons::icon( QSize(48,48) , "application-exit.png" ), tr("Quit"), this);
        p->quit_action->setShortcut( QKeySequence(Qt::CTRL + Qt::Key_Q) );

    p->favorite_action = new QAction( SMasterIcons::icon( QSize(48,48) , "favorites.png" ) , p->favorite_text , this );


    p->menu->addAction( p->find_action );
    p->cat_widget->addActionTooItems( p->favorite_action );


    p->menu_panel = new SMenuPanel( this );
        p->menu_panel->addAction( p->process_action );
        p->menu_panel->addSeparator();
        p->menu_panel->addAction( p->configure_action );
        p->menu_panel->addAction( p->applications_action );
        p->menu_panel->addAction( p->plugins_action );
        p->menu_panel->addSeparator();
        p->menu_panel->addAction( p->about_qt_action );
        p->menu_panel->addAction( p->about_sialan_action );
        p->menu_panel->addAction( p->about_action );
        p->menu_panel->addSeparator();
        p->menu_panel->addAction( p->quit_action );

    p->menubar_manager->addMenu( "Silicon" , p->menu_panel );
}

void SiliconUI::init_presonal_confs()
{
    bool OK;

    resize( p->prc->readOption( perConf::WIDTH ).toInt(&OK) , p->prc->readOption( perConf::HEIGHT ).toInt(&OK) );
    p->cat_widget->setView( p->prc->readOption(perConf::VIEW_MODE) );

    p->favorites = p->prc->readOption(perConf::FAVORITES).split(",",QString::SkipEmptyParts);   // Moved To DetectApps Function
}

void SiliconUI::init_graphic_effects()
{
    p->menu->setAnimation(                stringToBoolian( p->prc->readOption(perConf::MENU_ANIMATION      )) );
    p->cat_widget->setSearchBarAnimation( stringToBoolian( p->prc->readOption(perConf::SEARCHBAR_ANIMATION )) );
    p->cat_widget->setAnimation(          stringToBoolian( p->prc->readOption(perConf::SEARCHING_ANIMATION )) );
    p->tab_bar->setAnimation(             stringToBoolian( p->prc->readOption(perConf::TABBAR_ANIMATION    )) );
    p->toolbar_manager->setAnimation(     stringToBoolian( p->prc->readOption(perConf::TOOLBAR_ANIMATION   )) );

    p->context_button->setColorizedMenu( stringToBoolian( p->prc->readOption(perConf::COLORIZED_MENU) ) );

    p->gradiant_back = stringToBoolian( p->prc->readOption(perConf::GRADIANT_BACK) );
    if( p->gradiant_back )
    {
        p->main->setFrameShape(  QFrame::NoFrame );
        p->main->setFrameShadow( QFrame::Plain );
#ifdef Q_OS_UNIX
    }
    else
    {/*
        p->main->setFrameShape(  QFrame::StyledPanel );
        p->main->setFrameShadow( QFrame::Raised );*/
        p->main->setFrameShape(  QFrame::NoFrame );
        p->main->setFrameShadow( QFrame::Plain );
    }
#else
        p->main->setAutoFillBackground( false );
    }
    else
    {
        p->main->setFrameShape(  QFrame::Box );
        p->main->setFrameShadow( QFrame::Sunken );
        p->main->setAutoFillBackground( true );
    }
#endif

    repaint( rect() );
}

void SiliconUI::init_style_sheet()
{

// Reading Theme from file ===================================//
    QString TabStyle,
            lastOfPath = '/' + p->prc->readOption( perConf::CURRENT_THEME ) + '/' + p->defaults.Default_Theme_Name,
            fileName = p->defaults.Personal_Theme_Directory_Path + lastOfPath;

    if(!QFile(fileName).exists())
    {
        fileName = p->defaults.Share_Theme_Directory_Path + lastOfPath;
        if(!QFile(fileName).exists())
        {
            std::cout<<"QString SiliconUI::readTheme() : "
                     <<fileName.toStdString()<<" No such theme"<<std::endl;
            return;
        }
    }

    QFile file(fileName);
    QTextStream stream(&file);

    if(file.open(QIODevice::ReadOnly))
        TabStyle=stream.readAll();
    file.close();
// End of Reading =========//


    SThemeEngine ste( p->prc );
    TabStyle=ste.translate( TabStyle );

    SStyleSheet css;
        css.addStyle( TabStyle );

    SStyleSheet tmpCss;


    setStyleSheet( css.filter("QSilicon") );
    p->tab_bar->setStyleSheet( css.filter("STabBar") );
    p->cat_widget->setStyleSheet( css.filter("CatWidget") );
    p->menu->setStyleSheet( css.filter("SMenu") );

    tmpCss.addStyle( css.filter("SMenuButton") );
        tmpCss.replace( "SMenuButton" , "QPushButton" );
        p->menu_button->setStyleSheet( tmpCss.style() );
        tmpCss.clear();

    tmpCss.addStyle( css.filter("SContextButton") );
        tmpCss.replace( "SContextButton" , "QPushButton" );
        p->context_button->setStyleSheet( tmpCss.style() );
        tmpCss.clear();

    tmpCss.addStyle( css.filter("ConfigureUi") );
        tmpCss.replace( "ConfigureUi" , "QDialog" );
        p->configure_ui->setStyleSheet( tmpCss.style() );
        tmpCss.clear();

    tmpCss.addStyle( css.filter("ApplicationsUi") );
        tmpCss.replace( "ApplicationsUi" , "QDialog" );
        p->application_manager->setStyleSheet( tmpCss.style() );
        tmpCss.clear();

    tmpCss.addStyle( css.filter("PluginsUi") );
        tmpCss.replace( "PluginsUi" , "QDialog" );
        p->plugin_manager->setStyleSheet( tmpCss.style() );
        tmpCss.clear();

    tmpCss.addStyle( css.filter("AboutUi") );
        tmpCss.replace( "AboutUi" , "QDialog" );
        p->about_style = tmpCss.style();
        if( p->about_ui ) p->about_ui->setStyleSheet( p->about_style );
        tmpCss.clear();

    p->info_widget->setStyleSheet( css.filter("SInfoWidget") );
}

void SiliconUI::init_connections()
{
    connect( p->application_manager , SIGNAL(foundNewApplication(SAboutData))          , this                   , SLOT(newApplicationFound(SAboutData)) );
    connect( p->signals_box         , SIGNAL(registerApplicationSignal(SApplication*)) , p->application_manager , SLOT(applicationAdded(SApplication*)) );
    connect( p->signals_box         , SIGNAL(deleteApplicationSignal(SApplication*))   , p->application_manager , SLOT(unregisterApplication(SApplication*))
                                    , Qt::QueuedConnection );

    connect( p->signals_box , SIGNAL(pluginStartedSignal(SPlugin*)) , p->plugin_manager , SLOT(pluginStarted(SPlugin*)) );
    connect( p->signals_box , SIGNAL(pluginStoppedSignal(SPlugin*)) , p->plugin_manager , SLOT(pluginStopped(SPlugin*)) );

    connect( p->signals_box , SIGNAL(registerPageSignal(SPage*))                     , this              , SLOT(newPageRequest(SPage*))                 );
    connect( p->signals_box , SIGNAL(closeRequestSignal(SPage*))                     , this              , SLOT(closePageRequest(SPage*))               );
    connect( p->signals_box , SIGNAL(loadAppSignal(QString,QVariantList,void*))      , this              , SLOT(load(QString ,QVariantList,void*))      );
    connect( p->signals_box , SIGNAL(sendArgumentSignal(int,QVariantList))           , this              , SLOT(runtimeArgRequest(int,QVariantList))    );
    connect( p->signals_box , SIGNAL(showProcessessSignal())                         , this              , SLOT(showProcesses())                        );
    connect( p->signals_box , SIGNAL(pageVisiblityChangeSignal(SPage*,bool))         , this              , SLOT(focusOn(SPage*,bool))                   );
    connect( p->signals_box , SIGNAL(registerProgressItemSignal(SProgressListItem*)) , p->process_list   , SLOT(add(SProgressListItem*))                );
    connect( p->signals_box , SIGNAL(menuPanelUpdatedSignal(SMenuPanel*))            , this              , SLOT(set_current_menupanel())                );
    connect( p->signals_box , SIGNAL(pageTypeChangedSignal(SPage*))                  , this              , SLOT(pageTypeChanged(SPage*))                );
    connect( p->signals_box , SIGNAL(pageSettingsChangedSignal(SPage*))              , this              , SLOT(pageSettingsChanged(SPage*))            );
    connect( p->signals_box , SIGNAL(registerDialogSignal(SDialog*))                 , p->dialog_manager , SLOT(insert(SDialog*))                       );
    connect( p->signals_box , SIGNAL(dialogVisiblityChangeSignal(SDialog*,bool))     , p->dialog_manager , SLOT(dialogVisibilityChanged(SDialog*,bool)) );
    connect( p->signals_box , SIGNAL(registerDialogMethodSignal(SDialogMethod*))     , p->dialog_manager , SLOT(setMethod(SDialogMethod*))              );

    connect( p->signals_box , SIGNAL(openFileSignal(QString))                   , p->file_loader , SLOT(open(QString))                       );
    connect( p->file_loader , SIGNAL(runtimeArgumentsRequest(int,QVariantList)) , this           , SLOT(runtimeArgRequest(int,QVariantList)) );
    connect( p->file_loader , SIGNAL(loadAppRequest(SAboutData,QVariantList))   , this           , SLOT(load(SAboutData,QVariantList))       );

    connect( p->signals_box , SIGNAL(writeConfSignal(QString,QString,QString)) , SLOT(writeConf(QString,QString,QString)) );
    connect( p->signals_box , SIGNAL(readConfSignal(QString,QString,QString&)) , SLOT(readConf(QString,QString,QString&)) );
    connect( p->signals_box , SIGNAL(saveAllConfsSignal())                     , SLOT(saveConfs())                        );

    connect( p->menu_button , SIGNAL(clicked())         , this , SLOT(menuShowHide())         );
    connect( p->tab_bar     , SIGNAL(closeRequest(int)) , this , SLOT(closeAt(int))           );
    connect( p->tab_bar     , SIGNAL(tabChanged(int))   , this , SLOT(currentTabChanged(int)) );

    connect( p->configure_ui , SIGNAL(graphicEffectsChanged()) , this , SLOT(init_graphic_effects())  );
    connect( p->configure_ui , SIGNAL(themeChanged())          , this , SLOT(init_style_sheet()) );

    connect( p->favorite_action     , SIGNAL(triggered()) , this , SLOT(addToFavorites())    );
    connect( p->switch_type_action  , SIGNAL(triggered()) , this , SLOT(switchPageType())    );
    connect( p->process_action      , SIGNAL(triggered()) , this , SLOT(showProcesses())     );
    connect( p->configure_action    , SIGNAL(triggered()) , this , SLOT(showConfigure())     );
    connect( p->applications_action , SIGNAL(triggered()) , this , SLOT(showApplications())  );
    connect( p->plugins_action      , SIGNAL(triggered()) , this , SLOT(showLoadedPlugins()) );
    connect( p->about_qt_action     , SIGNAL(triggered()) , this , SLOT(aboutQt())           );
    connect( p->about_action        , SIGNAL(triggered()) , this , SLOT(showAbout())         );
    connect( p->about_sialan_action , SIGNAL(triggered()) , this , SLOT(aboutSialan())       );
    connect( p->quit_action         , SIGNAL(triggered()) , this , SLOT(close())             );

    connect( p->cat_widget , SIGNAL(itemDoubleClicked(QListWidgetItem*))  , this , SLOT(loadApp(QListWidgetItem*))           );
    connect( p->cat_widget , SIGNAL(itemSingleClicked(QListWidgetItem*))  , this , SLOT(previewApp(QListWidgetItem*))        );
    connect( p->cat_widget , SIGNAL(currentItemChanged(QListWidgetItem*)) , this , SLOT(favoriteSettingUp(QListWidgetItem*)) );
    connect( p->cat_widget , SIGNAL(viewChanged(QString))                 , this , SLOT(viewModeChanged(QString))            );

/*
// Connections of Actions
    connect( p->find_action      , SIGNAL(triggered()) , this , SLOT(expandSearchBar())   );*/
}

void SiliconUI::init_last_chance()
{
    p->data_box->main_panel = p->toolbar_manager;

    p->plugin_manager->detect();

    p->application_manager->registerAboutData( p->silicon_about );
    p->application_manager->detect();
    p->file_loader->refresh();

    set_current_menupanel();

    p->Silicon = new SApplication( p->silicon_about );
        p->Silicon->setQuitOnLastPageClosed( false );

    p->application_manager->lock( p->Silicon );

    p->Process_page   = 0;
    p->Configure_page = 0;
    p->About_page     = 0;
}

void SiliconUI::newApplicationFound( const SAboutData & about )
{
    QListWidgetItem *item = new QListWidgetItem();
        item->setText( about.translatedName() );
        item->setIcon( about.icon() );
        item->setWhatsThis( about.description() );

    for(int j=0;j<about.type().count();j++)
    {
        SAboutData::Type type = about.type().at(j);
        QString type_name = p->defaults.catGroup( type );

        p->cat_widget->addCat( p->defaults.catIcons(type) , type_name );
        p->cat_widget->addItem( type_name , item );
    }

    if( p->favorites.contains( about.name() ) )
        p->cat_widget->addItem( p->favorite_text , item );

    QWidget *widget = p->application_manager->configure( about );
    if( widget != 0 )
        p->configure_ui->addItem( about.icon() , about.translatedName() , widget );

    p->apps_hash.insert( item , about );
}

void SiliconUI::newPageRequest( SPage *page )
{
    switch( static_cast<int>(page->pageType()) )
    {
    case SPage::TabedPage :
        this->addPageToTab( page );
        break;

    case SPage::WindowedPage :
        this->addPageToWindow( page );
        break;
    }

    p->application_manager->refreshUI();
}

void SiliconUI::addPageToTab( SPage *page )
{
    SApplication *application = page->parentApp();
    const SAboutData & about = application->parent();

    QString name = page->title();
    if( name.isEmpty() )
        name = about.translatedName();
    if( name.size() > 14 )
        name.replace( name.size()-5 , 5 , "..." );

    QIcon icon = page->icon();
    if( icon.isNull() )
        icon = about.icon();

    p->page_lists << page;
    p->tab_bar->addTab( icon , name );
    p->menubar_manager->addMenu( name , page->menuPanel() );

    QWidget *new_page = p->tab_bar->page( p->tab_bar->currentIndex() );
        new_page->setLayout( new QHBoxLayout() );
        new_page->layout()->setContentsMargins(0,0,0,0);
        new_page->layout()->addWidget( page );

    pageSettingsChanged( page );
    show();
}

void SiliconUI::addPageToWindow( SPage *page )
{
    AppMainWindow *window = new AppMainWindow( page , this );

    p->windowed_page_list << page;
    p->page_to_window_hash.insert( page , window );

    pageSettingsChanged( page );

    window->show();
}

void SiliconUI::closeAt( int index )
{
    p->page_lists[index]->close();
}

void SiliconUI::closePageRequest( SPage *page )
{
    int index;

    switch( static_cast<int>(page->pageType()) )
    {
    case SPage::TabedPage :
        index = p->page_lists.indexOf( page );
        p->page_lists.removeAt( index );
        p->tab_bar->removeTab( index );
        p->menubar_manager->remove( index+1 );
        p->dialog_manager->remove( page );
        break;

    case SPage::WindowedPage :
        p->windowed_page_list.removeOne( page );
        p->dialog_manager->remove( page );
        delete p->page_to_window_hash.take( page );
        //delete page;
        break;

    }

    p->application_manager->refreshUI();

    if( p->windowed_page_list.isEmpty() && p->page_lists.isEmpty() && isHidden() )
        close();
}

void SiliconUI::pageTypeChanged( SPage *page )
{
    int index;
    switch( static_cast<int>(page->pageType()) )
    {
    case SPage::TabedPage :
        page->setParent( 0 );
        p->windowed_page_list.removeOne( page );
        p->dialog_manager->pageTypeChanged( SPage::TabedPage );
        delete p->page_to_window_hash.take( page );

        addPageToTab( page );
        break;

    case SPage::WindowedPage :
        index = p->page_lists.indexOf( page );
            p->page_lists.removeAt( index );
            p->dialog_manager->pageTypeChanged( SPage::WindowedPage );
            p->tab_bar->removeTab( index );
            p->menubar_manager->remove( index+1 );

        addPageToWindow( page );
        //page->setStyle( page->style() );
        break;
    }
}

void SiliconUI::pageSettingsChanged( SPage *page )
{
    int index = p->page_lists.indexOf( page );
    if( index < 0 )
    {
        if( p->windowed_page_list.contains(page) && page->pageType() == SPage::WindowedPage )
            p->page_to_window_hash.value( page )->refresh();

        return ;
    }

    STabItem *item = p->tab_bar->tab( index );
        item->setText( page->title() );
        item->setIcon( page->icon() );

    if( index == p->tab_bar->currentIndex() )
    {
        set_current_menupanel();
        p->toolbar_manager->setCurrent( page , page->toolBar() );
        p->status_bar_manager->setCurrent( page->statusBar() );
    }
}

void SiliconUI::focusOn( SPage *page , bool stt )
{
    int index = p->page_lists.indexOf( page );


    if( index != -1 && stt && p->tab_bar->currentIndex() != index )
    {
        p->tab_bar->setCurrentIndex( index );
    }

    else if( p->windowed_page_list.contains(page) )
        p->page_to_window_hash.value(page)->setVisible( stt );

    /*else
    {
        if( p->tab_bar->currentIndex() == p->page_lists.indexOf(page) && p->menu->menuIsHidden() )
            this->menuShowHide();
    }*/
}

void SiliconUI::load( const QString & name , const QVariantList & args , void *pack )
{
    const QList<SAboutData> & list = p->application_manager->applicationsList();

    for( int i=0 ; i<list.count() ; i++ )
        if( list.at(i).name() == name )
            load( list.at(i) , args , pack );
}

void SiliconUI::load( const SAboutData & about , const QVariantList & args , void *pack )
{
    struct Packet{
        SApplication *application;
    };

    Packet *c_pack = static_cast<Packet*>(pack);

    SApplication *app = loadApp( p->apps_hash.key(about) , args );
    if( c_pack != 0 )
        c_pack->application = app;
}

SApplication *SiliconUI::loadApp( QListWidgetItem *item , const QVariantList & args )
{
    if( !p->apps_hash.contains( item ) )
        return 0;

    SAboutData about = p->apps_hash.value( item );
    SApplication *app = p->application_manager->loadApplication( about , args );

    p->signals_box->applicationLoaded( about.name() );

    return app;
}

void SiliconUI::runtimeArgRequest( int index , const QVariantList & args )
{
    SApplication *app = p->application_manager->loadedApplications().at( index );
        app->addRuntimeArgs( args );
        //app->focus();
}

void SiliconUI::currentTabChanged( int index )
{
    SPage *page = 0;
    if( index != -1 )
        page = p->page_lists.at(index);

    if( index != -1 && p->menu->isHidden() )
    {
        p->dialog_manager->currentTabChanged( page );
        p->toolbar_manager->setCurrent( page , page->toolBar() );
        p->status_bar_manager->setCurrent( page->statusBar() );

        set_current_menupanel();
        return ;
    }

    menuShowHide();

    if( page != 0 )
    {
        p->toolbar_manager->setCurrent( page , page->toolBar() );
        p->status_bar_manager->setCurrent( page->statusBar() );
    }
    else
    {
        p->toolbar_manager->setCurrent( 0 , 0 );
        p->status_bar_manager->setCurrent( 0 );
    }
}

void SiliconUI::menuShowHide()
{
    if( p->menu->isHidden() )
    {
        p->menu->showMenu();
        p->tab_bar->unselectAll( true );
        p->dialog_manager->currentTabChanged( 0 );
    }
    else if( !p->menu->isHidden() && !p->page_lists.isEmpty() )
    {
        p->menu->hideMenu();
        p->tab_bar->unselectAll( false );
        p->dialog_manager->currentTabChanged( p->page_lists.at(p->tab_bar->currentIndex()) );
    }

    set_current_menupanel();
}

void SiliconUI::switchPageType()
{
    if( !p->menu->menuIsHidden() )
        return ;

    int index = p->tab_bar->currentIndex();
    SPage *page = p->page_lists.at( index );
        page->switchPageType();
}

void SiliconUI::set_current_menupanel()
{
    QIcon icon = SMasterIcons::icon( QSize(48,48) , "configure.png" );
    p->context_button->clear();

    if( p->menu->menuIsHidden() && !p->page_lists.isEmpty() )
    {
        int index = p->tab_bar->currentIndex();

        SPage *page = p->page_lists.at( index );

        p->context_button->addMenuPanel( page->menuPanel() );
        p->context_button->addSeparator();
        p->context_button->addItem( p->switch_type_action );

        p->switch_type_action->setEnabled( page->isEnabled() );

        icon = p->tab_bar->tab( index )->icon();
    }

    p->context_button->addMenuPanel( p->menu_panel );
    p->context_button->setIcon( icon );
}

void SiliconUI::viewModeChanged( QString name )
{
    p->prc->setOption( perConf::VIEW_MODE , name );
}

void SiliconUI::aboutQt()
{
    QApplication::aboutQt();
}

void SiliconUI::aboutSialan()
{
    QDesktopServices::openUrl(QUrl("http://labs.sialan.org"));
}

void SiliconUI::showApplications()
{
    p->application_manager->show();
}

void SiliconUI::showLoadedPlugins()
{
    p->plugin_manager->show();
}

void SiliconUI::showConfigure()
{
    saveConfs();

    if( p->Configure_page != 0 )
    {
        p->Configure_page->show();
        return ;
    }

    p->Configure_page = new SPage( tr("Configure") , p->Silicon , SPage::TabedPage );
        p->Configure_page->setLayout( new QVBoxLayout() );
        p->Configure_page->setIcon( SMasterIcons::icon( QSize(48,48) , "configure.png" ) );
        p->Configure_page->layout()->addWidget( p->configure_ui );
        p->Configure_page->layout()->setContentsMargins( 0 , 0 , 0 , 0 );
        p->Configure_page->setMinimumSize( QSize(720,350) );
        p->configure_ui->show();

    connect( p->configure_ui   , SIGNAL(closed())          , p->Configure_page , SLOT(close())                     );
    connect( p->Configure_page , SIGNAL(destroyed(SPage*)) , this              , SLOT(configurePageClosed(SPage*)) );
}

void SiliconUI::configurePageClosed( SPage * )
{
    p->Configure_page = 0;

    p->configure_ui->hide();
    p->configure_ui->setParent( 0 );
}

void SiliconUI::showAbout()
{
    if( p->about_ui == 0 )
    {
        p->about_ui = new About(this);
            p->about_ui->setMinimumWidth( 937 );
            p->about_ui->setWindowFlags( Qt::Dialog );
            p->about_ui->setWindowModality( Qt::WindowModal );
            p->about_ui->setStyleSheet( p->about_style );
    }

    if( p->About_page != 0 )
    {
        p->About_page->show();
        return ;
    }

    p->about_ui->setAboutList( p->application_manager->applicationsList() );
    p->about_ui->showFirstPage();

    p->About_page = new SPage( tr("About") , p->Silicon , SPage::TabedPage );
        p->About_page->setLayout( new QVBoxLayout() );
        p->About_page->setIcon( SMasterIcons::icon( QSize(48,48) , "help-about.png" ) );
        p->About_page->layout()->addWidget( p->about_ui );
        p->About_page->layout()->setContentsMargins( 0 , 0 , 0 , 0 );
        p->About_page->setMinimumSize( QSize(720,350) );
        p->about_ui->show();

    //connect( p->about_ui   , SIGNAL(closed())          , p->About_page , SLOT(close())                 );
    connect( p->About_page , SIGNAL(destroyed(SPage*)) , this          , SLOT(aboutPageClosed(SPage*)) );

    p->about_ui->show();
}

void SiliconUI::aboutPageClosed( SPage * )
{
    p->About_page = 0;

    p->about_ui->hide();
    p->about_ui->setParent( 0 );
}

void SiliconUI::showProcesses()
{
    if( p->Process_page != 0 )
    {
        p->Process_page->show();
        return ;
    }

    p->Process_page = new SPage( "Processes" , p->Silicon );
        p->Process_page->setLayout( new QVBoxLayout() );
        p->Process_page->setIcon( SMasterIcons::icon( QSize(48,48) , "cpu.png" ) );
        p->Process_page->layout()->addWidget( p->process_list );
        p->Process_page->setMinimumSize( QSize(720,350) );
        p->process_list->show();

    p->data_box->process_page = p->Process_page;
    connect( p->Process_page , SIGNAL(destroyed(SPage*)) , SLOT(processPageClosed(SPage*)) );
}

void SiliconUI::processPageClosed( SPage * )
{
    p->Process_page = 0;
    p->data_box->process_page = p->Process_page;

    p->process_list->hide();
    p->process_list->setParent( 0 );
}

void SiliconUI::addToFavorites()
{
    QListWidgetItem *item = p->cat_widget->currentItem();
    if( item <= 0 )
        return ;

    const SAboutData & about = p->apps_hash.value( item );
    if( p->favorites.contains(about.name()) )
    {
        p->favorites.removeOne( about.name() );
        p->cat_widget->removeItem( p->favorite_text , item );
    }
    else
    {
        p->favorites << about.name();
        p->cat_widget->addItem( p->favorite_text , item );
        p->cat_widget->setCategoryRow( 0 ); // 0 is Favorite Item row in catwidget
    }

    favoriteSettingUp( item );
}

void SiliconUI::favoriteSettingUp( QListWidgetItem *item )
{
    if( item <= 0 )
        return ;

    const SAboutData & about = p->apps_hash.value( item );

    QPixmap fav_pixmap;
    if( p->favorites.contains(about.name()) )
        fav_pixmap = SMasterIcons::icon( QSize(48,48) , "favorites.png" ).pixmap( QSize(22,22) , QIcon::Normal   );
    else
        fav_pixmap = SMasterIcons::icon( QSize(48,48) , "favorites.png" ).pixmap( QSize(22,22) , QIcon::Disabled );

    p->favorite_action->setIcon( fav_pixmap );
}

void SiliconUI::previewApp(QListWidgetItem *item)
{
    if( !item )
        return ;

    if( !p->apps_hash.contains( item ) )
    {
        p->disc_manager->showDisc( item->text() );
        return ;
    }

    const SAboutData & about = p->apps_hash.value( item );

    p->info_widget->setTitle( about.translatedName() );
    p->info_widget->setPicture( about.pixmap() );
    p->info_widget->setDescription( about.description() );
}

void SiliconUI::writeConf( const QString & head , const QString & child , const QString & value )
{
    if( !p->prc->checkHeadExist( head ) )
        p->prc->addHead( head );
    if( !p->prc->checkChildExist( head , child ) )
        p->prc->addChild( head , child );

    p->prc->set( head , child , value );
}

void SiliconUI::readConf( const QString & head , const QString & child , QString & result )
{
    result = p->prc->read( head , child );
}

void SiliconUI::saveConfs()
{
    p->prc->setOption( perConf::WIDTH  , QString::number(this->width())  );
    p->prc->setOption( perConf::HEIGHT , QString::number(this->height()) );

    p->prc->setOption( perConf::FAVORITES , p->favorites.join(",") );

    p->prc->save();
}

void SiliconUI::readMessage( const QString & message )
{
    QStringList args = message.split(";",QString::SkipEmptyParts);
    SArgumentList list;

    if( args.count() > 1 )
    {
        for( int i=1 ; i<args.count() ; i++ )
        {
            SArgument atProcArg;
                atProcArg.arg = args.at(i);

            while( i+1 < args.count() && (args[i+1][0] != '-' || (atProcArg.arg=="-L" && (args[i+1]!="-L") ) ) )
            {
                i++;
                atProcArg.list << args.at(i);
            }

            list << atProcArg;
        }
    }
    else
        show();

    loadArguments( list , args.at(0) );
}

void SiliconUI::loadArguments( const SArgumentList & list , const QString & current_path )
{
    for( int i=0 ; i<list.count() ; i++ )
    {
        SArgument argmnt = list.at(i);
        if( argmnt.arg == "-L" && argmnt.list.count() != 0 )
        {
            QStringList app_args = list.at(i).list;
                app_args.removeAt( 0 );

            QVariantList args;
            for( int j=0 ; j<app_args.count() ; j++ )
                args << app_args.at(j);

            load( list.at(i).list.at(0) , args );
        }
        else if( argmnt.arg == "-f" || argmnt.arg[0] != '-' )
        {
            if( argmnt.arg[0] != '-' )
                argmnt.list.prepend( argmnt.arg );

            for( int i=0 ; i<argmnt.list.count() ; i++ )
            {
                QString file_str = argmnt.list.at(i);

#ifdef Q_OS_WIN32
                if( file_str[1] != ':' )
#else
                if( file_str[0] != '/' )
#endif
                    file_str.prepend( current_path + "/" );

                p->file_loader->open( file_str );
            }
        }
    }
}

void SiliconUI::initializeFinished()
{
    p->data_box->on_initialize = false;
    p->signals_box->finishInitializing();
}

void SiliconUI::resizeEvent( QResizeEvent *event )
{
#ifdef Q_OS_MAC
    p->menu->snapTo( this, 0 ,0,0,0 );
#else
    p->menu->snapTo( this, p->menu_button->minimumHeight()+2 ,0,0,0 );
#endif

    emit resized( event->size() );
}

void SiliconUI::paintEvent( QPaintEvent *event )
{
#ifdef Q_OS_MAC
    p->menu->snapTo( this, 0 ,0,0,0 );
#else
    p->menu->snapTo( this, p->menu_button->minimumHeight()+2 ,0,0,0 );
#endif

    if( p->palette != palette() )
    {
        init_style_sheet();
        p->palette = palette();
    }

    QWidget::paintEvent( event );

    if( !p->gradiant_back )
        return;

    QRect rct = event->rect();
        rct.setHeight( rct.height() + rct.y() );
        rct.setY( 0 );

#ifdef Q_OS_LINUX
    SColor color1( palette().window().color() );
    SColor color2( palette().window().color() );

    color1.setAlpha(0);
    color2.setAlpha(255);

    color2 = color2 * 2;

#else
#ifdef Q_OS_WIN32
    QString color_name;
    if( isActiveWindow() )
        color_name = "#B8D0E9";
    else
        color_name = "#D6E3F1";

    SColor color1( color_name );
    SColor color2( palette().base().color() );

    color1.setAlpha( 255 * p->aero_disabled );
    color2.setAlpha( 255 );

    color2 = color2 * 1.2;
#endif
#endif

    QLinearGradient gradiant(QPointF(0,7), QPointF(0,63));
        gradiant.setColorAt(0, color1);
        gradiant.setColorAt(1, color2);

    QPainter painter(this);
        painter.fillRect( rct , gradiant );
}

void SiliconUI::closeEvent( QCloseEvent *event )
{/*
    if( QSystemTrayIcon::isSystemTrayAvailable() )
    {
        hide();
        event->ignore();
        return;
    }*/

    p->tab_bar->setAnimation( false );
    int ok = true;

    for( int i=0 ; ok && i<p->page_lists.count() ; i++ )
        ok = ok & p->page_lists.at(i)->close();

    p->tab_bar->setAnimation( stringToBoolian(p->prc->readOption(perConf::TABBAR_ANIMATION)) );

    if( !p->windowed_page_list.isEmpty() && ok )
    {
        hide();
        event->ignore();
        return;
    }

/*
    for( int i=0 ; ok && i<p->windowed_page_list.count() ; i++ )
        ok = ok & p->windowed_page_list.at(i)->close();


    const QList<SApplication *> & apps_list = p->application_manager->loadedApplications();
    for( int i=0 ; ok && i<apps_list.count() ; i++ )
        ok = ok && apps_list.at(i)->quit();

    QApplication::processEvents();
*/

    if( ok )
    {
        event->accept();
        QApplication::quit();
    }
    else
        event->ignore();

}

void SiliconUI::dropEvent( QDropEvent *event )
{
    QList<QUrl> list = event->mimeData()->urls();

    for( int i=0 ; i<list.count() ; i++ )
    {
        QString url_path = list.at(i).path();

#ifdef Q_OS_WIN32
        if( url_path[0] == '/' )
            url_path.remove( 0 , 1 );
#endif

        p->file_loader->open( url_path );
    }

    event->acceptProposedAction();
}

void SiliconUI::dragEnterEvent( QDragEnterEvent *event )
{
    if( event->mimeData()->hasUrls() )
        event->acceptProposedAction();
}

bool SiliconUI::event( QEvent *event )
{
    if( event->type() == QEvent::WindowActivate || event->type() == QEvent::WindowDeactivate )
        repaint();

    return QMainWindow::event( event );
}

#ifdef Q_OS_WIN32
bool SiliconUI::winEvent( MSG * msg, long * result )
{
    emit winEventSignal( msg , result );
    return false;
}
#endif

SiliconUI::~SiliconUI()
{
    saveConfs();

    delete p->dialog_manager;
}
