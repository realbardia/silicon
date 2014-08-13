#include "libraryui.h"

#include <QDir>
#include <QFileInfo>
#include <QSize>
#include <QUrl>
#include <QCursor>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDrag>
#include <QMimeData>

#include <SMasterIcons>
#include <SAboutData>
#include <Silicon>
#include <SDialogTools>
#include <SiDiTools>

LibraryUI::LibraryUI( const QString & name , SApplication *parent )
    : SPage( name , parent )
{
    this->setAcceptDrops( true );

    QDir conf_dir( Silicon::config() + "/Library" );
        conf_dir.mkpath( conf_dir.path() );

    QString path = conf_dir.path() + "/library";
    library = new Librarydb( path );

    item_list = new ItemList();
        item_list->setLibrary( library );
        item_list->setIconSize( QSize(32,32) );
        item_list->setContextMenuPolicy( Qt::CustomContextMenu );

    tags_list = new TagsList();
        tags_list->setLibrary( library );
        //tags_list->setFixedWidth( 235 );

    char_list = new CharList();
        char_list->setFixedWidth( 43 );
        char_list->setLibrary( library );
        char_list->setFilterList( tags_list );

    format_list = new FormatList();
        format_list->setFixedWidth( 73 );
        format_list->setFilterList( char_list );


    filters_box = new QGroupBox( tr("Filters") );
        filters_box->setFixedWidth( 313 );

    filters_layout = new QHBoxLayout( filters_box );
        filters_layout->addWidget( format_list );
        filters_layout->addWidget( char_list );
        filters_layout->addWidget( tags_list );

    list_widget_box = new QGroupBox( "Result List" );
    list_widget_layout = new QVBoxLayout( list_widget_box );
        list_widget_layout->addWidget( item_list );

    item_preview = new ItemPreview();
    toolbar = new QToolBar();
        toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        toolbar->setStyleSheet( "QToolBar{border-style:solid}" );

    lists_layout = new QHBoxLayout();
        lists_layout->addWidget( filters_box );
        lists_layout->addWidget( list_widget_box );

    base_layout = new QVBoxLayout( this );
        //base_layout->addWidget( toolbar );
        base_layout->addLayout( lists_layout );
        base_layout->addWidget( item_preview );


    mounter = SiDiTools::createImageMounter( this );
    isoinfo = new SIsoInfo( this );

    QObject::connect( isoinfo , SIGNAL(copyrightUpdated(QString)) , item_preview , SLOT(setCopyright(QString)) );
    QObject::connect( isoinfo , SIGNAL(volumeUpdated(QString)) , item_preview , SLOT(setVolumeID(QString)) );
    QObject::connect( isoinfo , SIGNAL(applicationIdReaded(QString)) , item_preview , SLOT(setApplicationID(QString)) );
    QObject::connect( isoinfo , SIGNAL(systemUpdated(QString)) , item_preview , SLOT(setSystemID(QString)) );
    QObject::connect( isoinfo , SIGNAL(publisherUpdated(QString)) , item_preview , SLOT(setPublisher(QString)) );
    QObject::connect( isoinfo , SIGNAL(formatUpdated(QString)) , item_preview , SLOT(setFormat(QString)) );

    QObject::connect( tags_list , SIGNAL(rowChenged(QStringList)) , item_list , SLOT(setItems(QStringList)) );
    QObject::connect( item_list , SIGNAL(currentRowChanged(int)) , this , SLOT(itemChanged(int)) );
    QObject::connect( item_list , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(mountCurrentItem()) );

    setupActions();
    reload();

    setToolBar( toolbar );
    setMinimumSize( QSize(720,350) );
}

void LibraryUI::setupActions()
{
    burn_act = new QAction( QIcon(":/library/pics/Image-Burner.png") , tr("Burn Image") , this );
    mount_act = new QAction( SMasterIcons::icon( QSize(48,48) , "emblem-mounted.png" ) , tr("Mount") , this );
    delete_act = new QAction( SMasterIcons::icon( QSize(48,48) , "edit-delete.png" ) , tr("Delete From Library") , this );
    edit_tags_act = new QAction( tr("Edit Tags") , this );
    rename_act = new QAction( SMasterIcons::icon( QSize(48,48) , "edit-rename.png" ) , tr("Rename") , this );

    add_to_library_action = new QAction( SMasterIcons::icon( QSize(48,48) , "list-add.png" ) , tr("Add to Library") , this );
    reload_action = new QAction( SMasterIcons::icon( QSize(48,48) , "view-refresh.png" ) , tr("Reload") , this );

    toolbar->addAction( add_to_library_action );
    toolbar->addAction( reload_action );

    this->menuPanel()->addAction( add_to_library_action );
    this->menuPanel()->addAction( reload_action );

    menu = new QMenu( this );
        menu->addAction( burn_act );
        menu->addAction( mount_act );
        menu->addSeparator();
        menu->addAction( delete_act );
        menu->addAction( edit_tags_act );
        menu->addAction( rename_act );


    QObject::connect( item_list , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(showMenu(QPoint)) );
    QObject::connect( burn_act , SIGNAL(triggered()) , this , SLOT(burnCurrentItem()) );
    QObject::connect( mount_act , SIGNAL(triggered()) , this , SLOT(mountCurrentItem()) );
    QObject::connect( delete_act , SIGNAL(triggered()) , this , SLOT(deleteCurrentItem()) );
    QObject::connect( edit_tags_act , SIGNAL(triggered()) , this , SLOT(editCurrentItemTags()) );
    QObject::connect( rename_act , SIGNAL(triggered()) , this , SLOT(renameCurrentItem()) );
    QObject::connect( reload_action , SIGNAL(triggered()) , this , SLOT(reload()) );
    QObject::connect( add_to_library_action , SIGNAL(triggered()) , this , SLOT(addToLibrary()) );
}

void LibraryUI::reload()
{
    QStringList list;
    for( int i=0 ; i<(int)library->headsNumber() ; i++ )
        list << library->head(i);

    format_list->setItems( list );
}

void LibraryUI::addToLibrary()
{
    SDialogTools::getOpenFileNames( this , this , SLOT(addToLibrary(QStringList)) , tr("Select Files to add to Library") );
}

void LibraryUI::addToLibrary( const QStringList & list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->addToLibrary( list.at(i) , QFileInfo(list.at(i)).fileName() );
}

void LibraryUI::addToLibrary( const QString & path , const QString & name , const QStringList & tags )
{
    if( library->checkHeadExist( path ) )
        return ;

    if( name.isEmpty() )
        return;

    library->addHead( path );
    library->setOption( path , Librarydb::NAME , name );
    library->setOption( path , Librarydb::TAGS , tags.join(",") );
}

void LibraryUI::itemChanged( int  )
{
    QListWidgetItem *item = item_list->currentItem();
    if( item == 0 )
        return ;

    mounter->reload();
    QFileInfo file(item->statusTip());

    item_preview->setName( item->text() );
    item_preview->setAddress( file.filePath() );
    item_preview->setMountPoint( mounter->mountPoint(file.filePath()) );
    item_preview->setSize( file.size() / 1024 );

    item_preview->reload();
    item_preview->show();
    isoinfo->setFile( file.filePath() );
}

void LibraryUI::showMenu( const QPoint &  )
{
    int row = item_list->currentRow();
    if( row < 0 )
        return ;

    menu->move( QCursor::pos() );
    menu->show();
}

void LibraryUI::burnCurrentItem()
{
    QListWidgetItem *item = item_list->currentItem();
    if( item == 0 )
        return ;

    QString path = item->statusTip();

    SAboutData about;
    QList<SAboutData> apps_list = Silicon::appsList();
    for( int i=0 ; i<apps_list.count() ; i++ )
        if( apps_list[i].name() == "Copy Disc" )
        {
            about = apps_list[i];
            break;
        }

    if( about.name().isEmpty() )
        return ;

    Silicon::loadApp( about , QVariantList()<< path );
}

void LibraryUI::mountCurrentItem()
{
    QListWidgetItem *item = item_list->currentItem();
    if( item == 0 )
        return ;

    QString path = item->statusTip();
    Silicon::openFile( path );
/*
    SAboutData about;
    QList<SAboutData> apps_list = Silicon::appsList();
    for( int i=0 ; i<apps_list.count() ; i++ )
        if( apps_list[i].name() == "Mounter" )
        {
            about = apps_list[i];
            break;
        }

    if( about.name().isEmpty() )
        return ;

    int item_index = -1;
    QList<SAboutData> loaded_list = Silicon::loadedInnerApps();
    for( int i=0 ; i<loaded_list.count() ; i++ )
        if( loaded_list[i].name() == "Mounter" )
        {
            item_index = i;
            break;
        }

    if( item_index == -1 )
        Silicon::loadApp( about , QStringList()<< path );
    else
        Silicon::sendArgument( item_index , QStringList()<< path );*/
}

void LibraryUI::deleteCurrentItem()
{
    QListWidgetItem *item = item_list->currentItem();
    if( item == 0 )
        return ;

    QMessageBox msgBox;
        msgBox.setText( tr("Are you sure to delete item from Library ?") );
        msgBox.setWindowTitle( tr("Silicon Warning") );
        msgBox.setStandardButtons( QMessageBox::No | QMessageBox::Yes );
        msgBox.setDefaultButton( QMessageBox::No );
        msgBox.setIcon( QMessageBox::Warning );

    int ret = msgBox.exec();
    if( ret == QMessageBox::No )
        return ;

    library->remove( item->statusTip() );
    this->reload();
}

void LibraryUI::renameCurrentItem()
{
    QListWidgetItem *item = item_list->currentItem();
    if( item == 0 )
        return ;

    bool ok;
    QString current_name = library->readOption( item->statusTip() , Librarydb::NAME );
    QString name = QInputDialog::getText(this, tr("Rename"), tr("Rename Item :"), QLineEdit::Normal, current_name ,&ok );

    if ( !ok || name == current_name )
        return ;

    library->setOption( item->statusTip() , Librarydb::NAME , name );
    this->reload();
}

void LibraryUI::editCurrentItemTags()
{
    QListWidgetItem *item = item_list->currentItem();
    if( item == 0 )
        return ;

    bool ok;
    QString current_tags = library->readOption( item->statusTip() , Librarydb::TAGS );
    QString tags = QInputDialog::getText(this, tr("Tags"), tr("Edit Tags :"), QLineEdit::Normal, current_tags ,&ok );

    if ( !ok || tags == current_tags )
        return ;

    library->setOption( item->statusTip() , Librarydb::TAGS , tags );
    this->reload();
}

void LibraryUI::dropEvent( QDropEvent *event )
{
    QList<QUrl> list = event->mimeData()->urls();

    for( int i=0 ; i<list.count() ; i++ )
    {
        QString url_path = list.at(i).path();

#ifdef Q_OS_WIN32
        if( url_path[0] == '/' )
            url_path.remove( 0 , 1 );
#endif
        this->addToLibrary( url_path , QFileInfo(url_path).fileName() );
    }

    this->reload();
    event->acceptProposedAction();
}

void LibraryUI::dragEnterEvent( QDragEnterEvent *event )
{
    if( event->mimeData()->hasUrls() )
        event->acceptProposedAction();
}

void LibraryUI::addRuntimeArgs( const QVariantList & args )
{
    QString path_str = "path=";
    QString name_str = "name=";
    QString tags_str = "tags=";

    QString path;
    QString name;
    QString tags;

    int args_count = args.count();
    for( int i=0 ; i<args_count ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        QString arg_str = var.toString();

        if( arg_str.left(path_str.size()) != path_str )
            continue;

        path = arg_str.remove( 0 , path_str.size() );

        while( i+1 < args_count && args.at(i+1).type() == QVariant::String && args.at(i+1).toString().left(path_str.size()) != path_str )
        {
            i++;
            if( args.at(i).toString().left(name_str.size()) == name_str )
                name = QString(args.at(i).toString()).remove( 0 , name_str.size() );

            else if( args.at(i).toString().left(tags_str.size()) == tags_str )
                tags = QString(args.at(i).toString()).remove( 0 , tags_str.size() );
        }

        this->addToLibrary( path , name , tags.split(",") );
    }

    this->reload();
}

LibraryUI::~LibraryUI()
{
    delete library;
}
