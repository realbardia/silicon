#include "managergui.h"

#include <QFileInfo>
#include <QStringList>
#include <QUrl>
#include <QStatusBar>
#include <QMimeData>

#include <SMasterIcons>
#include <Silicon>
#include <SDesktopFunctions>
#include <SDialogTools>
#include <SiDiTools>
#include <SAbstractImageMounter>

managerGUI::managerGUI( const QString & name , SApplication *parent , const QString & confPath , MounterConf *conf )
    : SPage( name , parent )
{
    this->confPath = confPath;
    process_item = 0;
    busy_stt = false;
    open_fm_stt = false;

    setAcceptDrops( true );

    configureUI = 0;
    personal = conf;
    if( personal == 0 )
        personal = new MounterConf( this->confPath + "/config" );

    mounter = SiDiTools::createImageMounter( this );

    SProgressListItem *progress = processItem();
        progress->hide();

    toolbar = new QToolBar();
        toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

    item_preview = new ItemPreview();
        item_preview->hide();
        item_preview->hideWithAnim();

    status_bar = new QStatusBar();
        status_bar->addWidget( item_preview , 1 );
        status_bar->setSizePolicy( QSizePolicy::MinimumExpanding , QSizePolicy::Fixed );

    isoinfo = new SIsoInfo( this );

    progress_indicator = new QProgressIndicator();
        progress_indicator->setDisplayedWhenStopped( false );

    logText = new QTextBrowser();
        logText->setReadOnly( true );

    logLayout = new QHBoxLayout();
        logLayout->addWidget( logText );

    processItem()->extraWidget()->setLayout( logLayout );

    list = new QListWidget();
        list->setIconSize( QSize(32,32) );
        list->setContextMenuPolicy( Qt::CustomContextMenu );

    layout = new QVBoxLayout( this );
        layout->addWidget( list );
        layout->addWidget( status_bar );
        layout->setContentsMargins( 1 , 1 , 1 , 1 );

    menu = new QMenu( this );

    autoMount = new AutoMount( mounter , this );

    connect( mounter , SIGNAL(mounted(bool))   , this , SLOT(mounted(bool))   );
    connect( mounter , SIGNAL(unmounted(bool)) , this , SLOT(unmounted(bool)) );

    connect( progress->cancelButton() , SIGNAL(clicked()) , progress , SLOT(hide()) );

    connect( isoinfo , SIGNAL(copyrightUpdated(QString))    , item_preview , SLOT(setCopyright(QString))     );
    connect( isoinfo , SIGNAL(volumeUpdated(QString))       , item_preview , SLOT(setVolumeID(QString))      );
    connect( isoinfo , SIGNAL(applicationIdReaded(QString)) , item_preview , SLOT(setApplicationID(QString)) );
    connect( isoinfo , SIGNAL(systemUpdated(QString))       , item_preview , SLOT(setSystemID(QString))      );
    connect( isoinfo , SIGNAL(publisherUpdated(QString))    , item_preview , SLOT(setPublisher(QString))     );
    connect( isoinfo , SIGNAL(formatUpdated(QString))       , item_preview , SLOT(setFormat(QString))        );

    connect( item_preview , SIGNAL(eject()) , this , SLOT(processCurrent()) );

    connect( list , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(itemClick(QListWidgetItem*)) );
    connect( list , SIGNAL(currentRowChanged(int))              , this , SLOT(itemChanged(int))            );
    connect( list , SIGNAL(customContextMenuRequested(QPoint))  , this , SLOT(showContextMenu(QPoint))     );

    setupActions();
    reloadConfigs();
    loadList();

    setToolBar( toolbar );
    //setStatusBar( status_bar );
    setMinimumSize( QSize(720,350) );
}

void managerGUI::setupActions()
{
    newMountAct = new QAction( SMasterIcons::icon( QSize(48,48) , "list-add.png" ) , tr("New Mount") , toolbar );
        toolbar->addAction( newMountAct );

    newManualMountAct = new QAction( SMasterIcons::icon( QSize(48,48) , "list-add.png" ) , tr("New Manual Mount") , toolbar );
        toolbar->addAction( newManualMountAct );

    toolbar->addSeparator();

    showLibraryAct = new QAction( QIcon(":/mounter/files/library-icon.png") , "Silicon Library" , toolbar );
        toolbar->addAction( showLibraryAct );

    toolbar->addSeparator();

    unmountAllAct = new QAction( SMasterIcons::icon( QSize(48,48) , "media-eject.png" ) , tr("Unmount All") , toolbar );
        toolbar->addAction( unmountAllAct );

    openFileManagerAct = new QAction( SMasterIcons::icon( QSize(48,48) , "folder.png" ) , tr("Open With FileManager") , menu );
        menu->addAction( openFileManagerAct );

    open_with_menu = new QMenu( tr("Open With") , menu );
        menu->addMenu( open_with_menu );

    burnAct = new QAction( QIcon(":/mounter/files/Image-Burner.png") , tr("Burn Image") , menu );
        open_with_menu->addAction( burnAct );

    menu->addSeparator();

    mountAct = new QAction( SMasterIcons::icon( QSize(48,48) , "emblem-mounted.png" ) , tr("Auto Mount") , menu );
        menu->addAction( mountAct );

    manualMountAct = new QAction( SMasterIcons::icon( QSize(48,48) , "emblem-mounted.png" ) , tr("Manual Mount") , menu );
        menu->addAction( manualMountAct );

    umountAct = new QAction( SMasterIcons::icon( QSize(48,48) , "media-eject.png" ) , tr("Unmount") , menu );
        menu->addAction( umountAct );

    menu->addSeparator();

    reloadAct = new QAction( SMasterIcons::icon( QSize(48,48) , "view-refresh.png" ) , tr("Reload") , toolbar );
        toolbar->addAction( reloadAct );

    toolbar->addWidget( progress_indicator );

    menuPanel()->addAction( newMountAct );
    menuPanel()->addAction( newManualMountAct );
    menuPanel()->addSeparator();
    menuPanel()->addAction( showLibraryAct );
    menuPanel()->addAction( unmountAllAct );
    menuPanel()->addAction( reloadAct );

    connect( newMountAct        , SIGNAL(triggered()) , this , SLOT(newAutoMount())            );
    connect( newManualMountAct  , SIGNAL(triggered()) , this , SLOT(newManualMount())          );
    connect( showLibraryAct     , SIGNAL(triggered()) , this , SLOT(showLibrary())             );
    connect( unmountAllAct      , SIGNAL(triggered()) , this , SLOT(unmountAll())              );
    connect( openFileManagerAct , SIGNAL(triggered()) , this , SLOT(openCurrentItemLocation()) );
    connect( burnAct            , SIGNAL(triggered()) , this , SLOT(burnImage())               );
    connect( mountAct           , SIGNAL(triggered()) , this , SLOT(processCurrent())          );
    connect( manualMountAct     , SIGNAL(triggered()) , this , SLOT(processManualCurrent())    );
    connect( umountAct          , SIGNAL(triggered()) , this , SLOT(processCurrent())          );
    connect( reloadAct          , SIGNAL(triggered()) , this , SLOT(loadList())                );

    this->catChanged();
}

void managerGUI::loadList()
{
    list->clear();

    // Define Mounts Lists =============
    mounter->reload();
    QStringList mounted_list = mounter->mountedList();
    QString name;
    for( int i=0 ; i<mounted_list.count() ; i++ )
    {
        name = mounted_list.at(i);
        if( name.left(5) != "/dev/" )
        {
            QListWidgetItem *item = new QListWidgetItem( SMasterIcons::icon( QSize(48,48) , "dialog-ok-apply.png" ) , name );
                item->setStatusTip( name );
                item->setToolTip( tr("Mounted on %1 ").arg( mounter->mountPoint(name)) );

            list->addItem( item );
        }
    }
    // End Define Mounts List ==========

    if( list->count() != 0 )
        list->setCurrentRow( 0 );
    else
        status_bar->hide();
}

void managerGUI::setConfigure( ConfigureWidget *conf )
{
    configureUI = conf;
    connect( configureUI , SIGNAL(configsChanged()) , this , SLOT(reloadConfigs()) );
}

void managerGUI::newManualMount()
{
    SDialogTools::getOpenFileName( this , this , SLOT(manualMount(QString)) );
}

void managerGUI::newAutoMount()
{
    SDialogTools::getOpenFileName( this , this , SLOT(mount(QString)) );
}

void managerGUI::unmountAll()
{
    if( mounter->isStarted() )
        return ;

    for( int i=0 ; i<list->count() ; i++ )
        queue.enqueue( list->item(i)->statusTip() );

    if( queue.isEmpty() )
        return ;

    mount( queue.dequeue() );
}

void managerGUI::processManualCurrent()
{
    QString mPoint = QFileDialog::getExistingDirectory( this );
    if( mPoint.isEmpty() )
        return ;

    mount( list->currentItem()->statusTip() , mPoint );
}

void managerGUI::processCurrent()
{
    QListWidgetItem *item = list->currentItem();
    if( item == 0 )
        return ;

    mount( item->statusTip() );
}

void managerGUI::itemClick( QListWidgetItem * item )
{
    if( click_to_open_stt )
        this->openCurrentItemLocation();
    else
        mount( item->statusTip() );
}

void managerGUI::manualMount( const QString & fileName )
{
    if( fileName.isEmpty() )
        return;

    QString mPoint = QFileDialog::getExistingDirectory( this );
    if( mPoint.isEmpty() )
        return ;

    mount( fileName , mPoint );
}

void managerGUI::automaticMount( const QString & fileName )
{
    mount( fileName );
}

void managerGUI::mount( const QString & fileName , const QString & mountPoint )
{
    if( fileName.isEmpty() )
        return;
/*
    if( mounter->isStarted() )
        return ;*/

    progress_indicator->startAnimation();
    busy_stt = true;

    last_mounted_adrs = fileName;

    autoMount->process( fileName , mountPoint );
    while( !queue.isEmpty() )
        mount( queue.dequeue() );
}

void managerGUI::itemChanged( int  )
{
    QListWidgetItem *item = list->currentItem();
    if( item == 0 )
        return ;

    mounter->reload();
    bool mountStt = mounter->isMount(item->statusTip());

    openFileManagerAct->setEnabled( mountStt );
    mountAct->setEnabled( !mountStt );
    manualMountAct->setEnabled( !mountStt );
    umountAct->setEnabled( mountStt );

    QFileInfo file(item->text());

    item_preview->setName( file.fileName() );
    item_preview->setAddress( file.filePath() );
    item_preview->setMountPoint( mounter->mountPoint(file.filePath()) );
    item_preview->setSize( file.size() / 1024 );

    item_preview->reload();
    item_preview->show();
    status_bar->show();
    isoinfo->setFile( file.filePath() );
}

void managerGUI::burnImage()
{
    int count = Silicon::appsList().count();
    SAboutData app;

    for( int i=0 ; i<count ; i++ )
        if( Silicon::appsList().value(i).name() == "Copy Disc" )
        {
        app = Silicon::appsList().at(i);
            break;
        }

    emit Silicon::loadApp( app , QVariantList()<< list->currentItem()->statusTip() );
}

void managerGUI::showLibrary()
{
    SAboutData about;
    QList<SAboutData> apps_list = Silicon::appsList();
    for( int i=0 ; i<apps_list.count() ; i++ )
        if( apps_list[i].name() == "Library" )
        {
            about = apps_list[i];
            break;
        }

    if( about.name().isEmpty() )
        return ;

    Silicon::loadApp( about );
}

void managerGUI::catChanged()
{
    openFileManagerAct->setEnabled( false );
    mountAct->setEnabled( false );
    manualMountAct->setEnabled( false );
    umountAct->setEnabled( false );
}

void managerGUI::showContextMenu( const QPoint &  )
{
    QListWidgetItem *item = list->currentItem();
    if( item == 0 )
        return ;

    this->itemChanged( list->currentRow() );

    menu->move( QCursor::pos() );
    menu->show();
}

void managerGUI::openCurrentItemLocation()
{
    SDesktopFunctions::openDirectory( mounter->mountPoint(list->currentItem()->statusTip()) );
}

void managerGUI::mounted( bool stt )
{
    SProgressListItem *progress = this->processItem();
        logText->setText( autoMount->readOutPut() );
        progress->showExtra( false );
        progress->show();

    QString message;
    if( stt )
    {
        progress->setValue( 100 );
        message = tr("Mount operation complete") ;
        this->loadList();

        if( open_fm_stt )
            SDesktopFunctions::openDirectory( mounter->mountPoint(last_mounted_adrs) );
    }
    else
    {
        queue.clear();
        progress->setValue( 40 );
        message = tr("Can't complete the mount operation") ;
        Silicon::showProcessess();
    }

    if( queue.count() != 0 )
    {
        mount( queue.dequeue() );
        return ;
    }

    progress->setInfoText( message );
    progress_indicator->stopAnimation();
    busy_stt = false;
}

void managerGUI::unmounted( bool stt )
{
    SProgressListItem *progress = this->processItem();
        logText->setText( autoMount->readOutPut() );
        progress->showExtra( false );
        progress->show();

    QString message;
    if( stt )
    {
        progress->setValue( 100 );
        message = tr("Unmount operation complete") ;
        this->loadList();
    }
    else
    {
        queue.clear();
        progress->setValue( 40 );
        message = tr("Can't complete the unmount operation") ;
        Silicon::showProcessess();
    }

    if( queue.count() != 0 )
    {
        mount( queue.dequeue() );
        return ;
    }

    progress->setInfoText( message );
    progress_indicator->stopAnimation();
    busy_stt = false;
}

void managerGUI::error( const QString & error )
{

}

void managerGUI::processFinished( int exitCode )
{

}

void managerGUI::reloadConfigs()
{
    autoMount->setMasterPoint( personal->readOption( MounterConf::MASTER_POINT ) );

    open_fm_stt       = personal->readOption( MounterConf::OPEN_FILE_MANAGER ) == "true";
    click_to_open_stt = personal->readOption( MounterConf::CLICK_ACTION )      == "open";
}

SProgressListItem *managerGUI::processItem()
{
    if( process_item == 0 )
    {
        this->process_item = new SProgressListItem( this );
        this->process_item->setTitle( "Silicon Mounter" );
        this->process_item->setIcon( QIcon(":/mounter/files/mounted.png") );
    }

    return this->process_item;
}

void managerGUI::dropEvent( QDropEvent *event )
{
    QList<QUrl> list = event->mimeData()->urls();


    for( int i=0 ; i<list.count() ; i++ )
    {
        QString url_path = list.at(i).path();

#ifdef Q_OS_WIN32
        if( url_path[0] == '/' )
            url_path.remove( 0 , 1 );
#endif
        queue.enqueue( url_path );
    }


    if( !mounter->isStarted() )
        mount( queue.dequeue() );

    event->acceptProposedAction();
}

void managerGUI::dragEnterEvent( QDragEnterEvent *event )
{
    if( mounter->isStarted() )
    {
        event->ignore();
        return ;
    }

    if( event->mimeData()->hasUrls() )
        event->acceptProposedAction();
}

void managerGUI::closeEvent( QCloseEvent *event )
{/*
    if( busy_stt )
        event->ignore();
    else
        event->accept();*/
}

void managerGUI::addRuntimeArgs( const QVariantList & args )
{
    if( args.isEmpty() )
        return ;

    for( int i=0 ; i<args.count() ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        queue.enqueue( var.toString() );
    }

    if( !mounter->isStarted() )
        mount( queue.dequeue() );
}

managerGUI::~managerGUI()
{
    if( configureUI != 0 )
        disconnect( configureUI , SIGNAL(configsChanged()) , this , SLOT(reloadConfigs()) );

    personal->save();

    delete list;
    delete autoMount;
}
