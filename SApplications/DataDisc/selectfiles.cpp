#define LABEL_SIZE 213
#define ICON_SIZE  QSize(48,48)
#define RATIO (1024*1024)

#include "selectfiles.h"
#include "optionsselect.h"

#include <QLabel>
#include <QLineEdit>
#include <QToolBar>
#include <QAction>
#include <QPushButton>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QScrollArea>
#include <QStack>
#include <QStatusBar>
#include <QDebug>

#include <SFileListWidget>
#include <SFolderGraph>
#include <SMasterIcons>
#include <SGraphicStack>
#include <SDesktopFunctions>
#include <SDialog>
#include <SDialogTools>
#include <SComboBox>
#include <Silicon>
#include <QtSystemInfo/QtSystemInfo.h>

#include <SDeviceList>
#include <SDeviceItem>

class SelectFilesPrivate
{
public:
    QLineEdit *volume_line;
    QLineEdit *copyright_line;
    QLineEdit *publisher_line;
    QLineEdit *application_line;
    QLineEdit *system_line;
    QLineEdit *abstract_line;
    QLineEdit *biblio_line;

    QScrollArea *scroll_area;
    QWidget     *lines_widget;
    QVBoxLayout *lines_layout;

    SFileListWidget *list_widget;
    SFolderGraph    *folder_graph;

    QStatusBar    *status_bar;
    QToolBar      *toolbar;
    QToolBar      *start_toolbar;
    QProgressBar  *progress_bar;
    SComboBox     *devices_combo;
    SGraphicStack *graphic_stack;
    OptionsSelect *option_select;

    QMenu   *context_menu;
    QAction *add_file_action;
    QAction *add_folder_action;
    QAction *new_folder_action;
    QAction *delete_action;
    QAction *rename_action;
    QAction *properties_action;
    QAction *previous_action;
    QAction *open_dir_action;
    QAction *start_action;
    QAction *more_action;
    QAction *data_image_action;

    QVBoxLayout *v_layout;
    QHBoxLayout *h_layout;
    QVBoxLayout *panel_layout;

    SDeviceList *device_list;

    QStack<QString>    address_stack;
    QList<SDeviceItem> devices;
    QString            image_path;

    QString      application_id_str;
    QtSystemInfo systemInfo;
    QString temp;

    SAbstractDataBurner   *data_burner;
    SAbstractImageCreator *image_creator;
};



SelectFiles::SelectFiles( SApplication *parent, SAbstractDataBurner *data_burner, SAbstractImageCreator *image_creator )
    : SPage( tr("Data Disc") , parent )
{
    p = new SelectFilesPrivate;
        p->application_id_str = tr( "Silicon (Using %1)" );
        p->data_burner = data_burner;
        p->image_creator = image_creator;

    QColor window = palette().window().color();
        window.setAlpha( 73 );

    QPalette pallete = this->palette();
        pallete.setColor( QPalette::Window , window );

    p->volume_line      = new QLineEdit();
    p->copyright_line   = new QLineEdit();
    p->publisher_line   = new QLineEdit();
    p->application_line = new QLineEdit();
    p->system_line      = new QLineEdit();
    p->abstract_line    = new QLineEdit();
    p->biblio_line      = new QLineEdit();

    p->volume_line->setPlaceholderText(      tr("Volume Label")   );
    p->copyright_line->setPlaceholderText(   tr("CopyRight")      );
    p->publisher_line->setPlaceholderText(   tr("Publisher")      );
    p->application_line->setPlaceholderText( tr("Application ID") );
    p->system_line->setPlaceholderText(      tr("System ID")      );
    p->abstract_line->setPlaceholderText(    tr("Abstract")       );
    p->biblio_line->setPlaceholderText(      tr("Biblio")         );

    p->publisher_line->setText( SDesktopFunctions::findEnvVariable("USER") );
    p->application_line->setText( p->application_id_str );
    p->system_line->setText( p->systemInfo.systemName() + " " + p->systemInfo.systemVersion() );

    p->volume_line->setMaximumWidth(      LABEL_SIZE );
    p->copyright_line->setMaximumWidth(   LABEL_SIZE );
    p->publisher_line->setMaximumWidth(   LABEL_SIZE );
    p->application_line->setMaximumWidth( LABEL_SIZE );
    p->system_line->setMaximumWidth(      LABEL_SIZE );
    p->abstract_line->setMaximumWidth(    LABEL_SIZE );
    p->biblio_line->setMaximumWidth(      LABEL_SIZE );

    p->devices_combo = new SComboBox();
        p->devices_combo->setEditable( true );
        p->devices_combo->setIconSize( QSize(22,22) );
        p->devices_combo->setMaximumWidth( LABEL_SIZE );

    p->start_toolbar = new QToolBar();
        p->start_toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->start_toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );
        p->start_toolbar->setMaximumWidth( LABEL_SIZE );

    p->folder_graph = new SFolderGraph();
    p->list_widget  = new SFileListWidget( this );
        p->list_widget->setFolderGraph( p->folder_graph );
        p->list_widget->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Expanding );
        p->list_widget->setViewMode( QListView::IconMode );
        p->list_widget->setGridSize( QSize( 128 , 64 ) );
        p->list_widget->setMovement( QListView::Snap );
        p->list_widget->setSelectionMode( QAbstractItemView::ExtendedSelection );
        p->list_widget->setFoldersIcon( SMasterIcons::icon( QSize(48,48) , "folder.png" ) );
        p->list_widget->setFilesIcon( SMasterIcons::icon( QSize(48,48) , "application-x-zerosize.png" ) );
        p->list_widget->setContextMenuPolicy( Qt::CustomContextMenu );

    p->context_menu = new QMenu( this );

    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );

    p->graphic_stack = new SGraphicStack();
        p->graphic_stack->push( SMasterIcons::icon( QSize(48,48) , "folder.png" ) , tr("Root") );

    p->progress_bar = new QProgressBar();
        p->progress_bar->setValue( 0 );
        p->progress_bar->setMaximum( 700 );
        p->progress_bar->setPalette( pallete );

    p->status_bar = new QStatusBar();
        p->status_bar->addWidget( p->progress_bar , 1 );

    p->option_select = new OptionsSelect();
        p->option_select->setMaximumWidth( LABEL_SIZE );

    p->lines_widget = new QWidget();
    p->lines_layout = new QVBoxLayout( p->lines_widget );
        p->lines_layout->addSpacing( 23 );
        p->lines_layout->addWidget( p->volume_line      );
        p->lines_layout->addSpacing( 37 );
        p->lines_layout->addWidget( p->copyright_line   );
        p->lines_layout->addWidget( p->publisher_line   );
        p->lines_layout->addWidget( p->application_line );
        p->lines_layout->addWidget( p->system_line      );
        p->lines_layout->addWidget( p->abstract_line    );
        p->lines_layout->addWidget( p->biblio_line      );
        p->lines_layout->addStretch();

    p->scroll_area = new QScrollArea();
        p->scroll_area->setWidgetResizable( true );
        p->scroll_area->setWidget( p->lines_widget );
        p->scroll_area->setFrameShadow( QFrame::Plain );
        p->scroll_area->setFrameShape( QFrame::NoFrame );
        p->scroll_area->setMaximumWidth( LABEL_SIZE );
        p->scroll_area->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        p->scroll_area->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
        p->scroll_area->viewport()->setAutoFillBackground( false );
        p->scroll_area->viewport()->setStyleSheet( "QWidget{background-color : transparent}" );

    p->panel_layout = new QVBoxLayout();
        p->panel_layout->addWidget( p->scroll_area );
        p->panel_layout->addWidget( p->devices_combo     );
        p->panel_layout->addWidget( p->option_select     );
        p->panel_layout->addWidget( p->start_toolbar     );

    p->h_layout = new QHBoxLayout();
        p->h_layout->addWidget( p->list_widget  );
        p->h_layout->addLayout( p->panel_layout );

    p->v_layout = new QVBoxLayout( this );
        p->v_layout->addLayout( p->h_layout     );

    connect( p->graphic_stack , SIGNAL(currentChanged(QString))            , SLOT(addressChanged())          );
    connect( p->list_widget   , SIGNAL(addressChanged(QString))            , SLOT(folderChanged())           );
    connect( p->list_widget   , SIGNAL(customContextMenuRequested(QPoint)) , SLOT(contextMenu(QPoint))       );
    connect( p->list_widget   , SIGNAL(reloaded())                         , SLOT(reloaded_slt())            );
    connect( p->devices_combo , SIGNAL(currentIndexChanged(int))           , SLOT(device_index_changed(int)) );
    connect( p->volume_line   , SIGNAL(textChanged(QString))               , SLOT(setScan(QString))          );
    connect( p->image_creator , SIGNAL(imageSizeDetected(int))             , SLOT(image_size_changed(int))   );
    connect( p->data_burner   , SIGNAL(imageSizeDetected(int))             , SLOT(image_size_changed(int))   );

    init_system();
    init_actions();
    reloaded_slt();

    setToolBar( p->toolbar );
    setStatusBar( p->status_bar );
    setMinimumSize( QSize(720,350) );
}

void SelectFiles::init_actions()
{
    QWidget *spr1 = new QWidget();
        spr1->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Minimum );

    p->add_file_action   = new QAction( SMasterIcons::icon(ICON_SIZE,"archive-insert.png")           , tr("Add File")    , this );
    p->add_folder_action = new QAction( SMasterIcons::icon(ICON_SIZE,"archive-insert-directory.png") , tr("Add Folder")  , this );
    p->new_folder_action = new QAction( SMasterIcons::icon(ICON_SIZE,"folder-new.png")               , tr("New Folder")  , this );
    p->delete_action     = new QAction( SMasterIcons::icon(ICON_SIZE,"edit-delete.png")              , tr("Delete")      , this );
    p->rename_action     = new QAction( SMasterIcons::icon(ICON_SIZE,"edit-rename.png")              , tr("Rename")      , this );
    p->properties_action = new QAction( SMasterIcons::icon(ICON_SIZE,"document-properties.png")      , tr("Properties")  , this );
    p->previous_action   = new QAction( SMasterIcons::icon(ICON_SIZE,"go-previous.png")              , tr("Previous")    , this );
    p->open_dir_action   = new QAction( SMasterIcons::icon(ICON_SIZE, "go-jump-locationbar.png" )    , tr("Open Folder") , this );
    p->start_action      = new QAction( SMasterIcons::icon(ICON_SIZE,"tools-media-optical-burn.png") , tr("Go")          , this );
    p->more_action       = new QAction( SMasterIcons::icon(ICON_SIZE,"edit-rename.png")              , tr("More")        , this );
    p->data_image_action = new QAction( SMasterIcons::icon(ICON_SIZE,"application-x-cd-image.png")   , tr("Image")       , this );

    p->toolbar->addAction( p->previous_action   );
    p->toolbar->addSeparator();
    p->toolbar->addAction( p->add_file_action   );
    p->toolbar->addAction( p->add_folder_action );
    p->toolbar->addAction( p->new_folder_action );
    p->toolbar->addAction( p->delete_action     );
    p->toolbar->addAction( p->rename_action     );
    p->toolbar->addAction( p->properties_action );
    p->toolbar->addSeparator();
    p->toolbar->addWidget( p->graphic_stack );

    p->start_toolbar->addAction( p->more_action  );
    p->start_toolbar->addWidget( spr1 );
    p->start_toolbar->addAction( p->start_action );

    menuPanel()->addAction( p->add_file_action   );
    menuPanel()->addAction( p->add_folder_action );
    menuPanel()->addAction( p->new_folder_action );

    p->context_menu->addAction( p->open_dir_action   );
    p->context_menu->addSeparator();
    p->context_menu->addAction( p->delete_action     );
    p->context_menu->addAction( p->rename_action     );
    p->context_menu->addSeparator();
    p->context_menu->addAction( p->properties_action );

    p->devices_combo->addAction( p->data_image_action );

    p->previous_action->setShortcut( QKeySequence( Qt::Key_Backspace ) );
    p->delete_action->setShortcut(   QKeySequence( Qt::Key_Delete    ) );
    p->rename_action->setShortcut(   QKeySequence( Qt::Key_F2        ) );

    connect( p->add_file_action   , SIGNAL(triggered()) , SLOT(addFiles())       );
    connect( p->add_folder_action , SIGNAL(triggered()) , SLOT(addFolder())      );
    connect( p->new_folder_action , SIGNAL(triggered()) , SLOT(newFolder())      );
    connect( p->rename_action     , SIGNAL(triggered()) , SLOT(renameFiles())    );
    connect( p->delete_action     , SIGNAL(triggered()) , SLOT(removeCurrent())  );
    connect( p->previous_action   , SIGNAL(triggered()) , SLOT(cdPrevious())     );
    connect( p->open_dir_action   , SIGNAL(triggered()) , SLOT(openFileDir())    );
    connect( p->properties_action , SIGNAL(triggered()) , SLOT(showProperties()) );
    connect( p->start_action      , SIGNAL(triggered()) , SLOT(go_prev())        );

    connect( p->more_action  , SIGNAL(triggered()) , p->option_select , SLOT(autoExpand()) );
}

void SelectFiles::init_system()
{
    p->device_list = new SDeviceList( this );

    connect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , SLOT(deviceDetected(SDeviceItem)) );

    p->device_list->refresh();
}

void SelectFiles::deviceDetected( const SDeviceItem & device )
{
    if( !p->devices.contains(device) )
    {
        p->devices << device;
        p->devices_combo->insertItem( p->devices.count(), SMasterIcons::icon(ICON_SIZE,"drive-optical.png") , device.name() );
    }
    else
    {
        int index = p->devices.indexOf( device );
        p->devices.removeAt( index );
        p->devices.insert( index , device );
        p->devices_combo->setItemText( index , device.name() );
    }

    device_index_changed( p->devices_combo->currentIndex() );
}

SelectFiles::Type SelectFiles::type() const
{
    int index = p->devices_combo->currentIndex();


    if( index == p->devices.count() )
        return SelectFiles::DataIso;

    else if( index >= 0  && p->option_select->onFly() )
        return SelectFiles::OnFlyDataDisc;

    else if( index >= 0  && !p->option_select->onFly() )
        return SelectFiles::NoOnFlyDataDisc;

    else
        return SelectFiles::Unknown;
}

QStringList SelectFiles::pathSpec() const
{
    return p->folder_graph->gaugingRoot();
}

SDeviceItem SelectFiles::currentDevice() const
{
    if( p->devices.isEmpty() || p->devices_combo->currentIndex() >= p->devices.count() )
        return SDeviceItem();

    return p->devices.at( p->devices_combo->currentIndex() );
}

const QString & SelectFiles::output() const
{
    if( type() == SelectFiles::NoOnFlyDataDisc )
    {
        if( p->temp.isEmpty() )
            p->temp = Silicon::requestTempFile();

        return p->temp;
    }

    return p->image_path;
}

void SelectFiles::setProgressBar( int value , int maximum )
{
    if( maximum == 1 )
        maximum = 4700;

    if( value < maximum )
    {
        p->progress_bar->setFormat( "%vMB / %mMB" );
    }
    else
    {
        maximum = value;
        p->progress_bar->setFormat( tr("Over Flow - %vMB") );
    }

    p->progress_bar->setMaximum( maximum );
    p->progress_bar->setValue( value );
}

void SelectFiles::device_index_changed( int index )
{
    p->option_select->setImageBurn( index == p->devices.count() );

    if( index == p->devices.count() )
    {
        if( dialogs().isEmpty() )
        {
            SDialogTools::getSaveFileName( this , this , SLOT(image_selected(QString)) );
            connect( dialogs().first() , SIGNAL(accepted(bool)) , SLOT(image_select_accp(bool)) );

            p->progress_bar->setFormat(tr("N/A"));
            p->progress_bar->setMaximum( 1 );
            p->progress_bar->setValue( 1 );
        }
    }
    else if( index >= 0 )
    {
        p->option_select->setCurrentDevice( p->devices.at(index) );
        image_size_changed( p->data_burner->imageSize() );
    }
}

void SelectFiles::image_select_accp( bool stt )
{
    if( stt )
        return ;

    p->devices_combo->setCurrentIndex( 0 );
    p->option_select->setDiscBurn( true );
}

void SelectFiles::image_selected( const QString & address )
{
    p->image_path = address;
    p->devices_combo->setEditText( tr("Image : %1").arg(address) );
    p->option_select->setImageBurn( true );
}

void SelectFiles::go_prev()
{
    setDisabled( true );
    SDialog *dialog = SDialogTools::getTimer( this , tr("Your Request will be starting after count down.") , 7000 );
    connect( dialog , SIGNAL(accepted(bool)) , SLOT(timer_finished(bool)) , Qt::QueuedConnection );

}

void SelectFiles::timer_finished( bool stt )
{
    if( stt )
        emit go();
    else
        setEnabled( true );
}

void SelectFiles::image_size_changed( int size )
{
    setProgressBar( size*2048/RATIO , 1+currentDevice().currentDiscFeatures().volume_capacity_int/RATIO );
}

void SelectFiles::folderChanged()
{
    QString path = p->folder_graph->currentPath();

    QStringList list = path.split( "/" , QString::SkipEmptyParts );
        list.prepend( tr("Root") );

    if( !list.isEmpty() && list == p->graphic_stack->allToStringList().mid(0,list.count()) )
    {
        int index = list.count()-1;
        p->graphic_stack->setCurrentIndex( index , false );
        return ;
    }

    p->graphic_stack->clear();
    for( int i=0 ; i<list.count() ; i++ )
        p->graphic_stack->push( SMasterIcons::icon( QSize(48,48) , "folder.png" ) , list.at(i) );
}

void SelectFiles::addressChanged()
{
    QString path = "/" + p->graphic_stack->toStringList().join("/");
        path.remove( 0 , tr("Root").size()+1 );

    if( path.isEmpty() )
        path = "/";

    p->list_widget->cd( path );
}

void SelectFiles::reload()
{
    p->list_widget->reload();
}

void SelectFiles::reloaded_slt()
{
    refresh();
    p->start_action->setEnabled( !p->folder_graph->isEmpty() );

    emit this->reloaded();
}

void SelectFiles::openFileDir()
{
    QList<QListWidgetItem *> list;
        list << p->list_widget->selectedItems();

    for( int i=0 ; i<list.count() ; i++ )
    {
        QFileInfo file( list.at(i)->statusTip() );
        QString path = file.dir().path();
#ifdef Q_OS_WIN32
            path.replace( "/" , "\\" );
#endif

        SDesktopFunctions::openDirectory( path );
    }
}

void SelectFiles::showProperties()
{
    if( p->list_widget->currentRow() < 0 )
        return ;

    SDialogTools::getFileProperties( this , QFileInfo( p->list_widget->currentItem()->statusTip() ) );
}

void SelectFiles::cdUp()
{
    p->list_widget->cdUp();
}

void SelectFiles::cdPrevious()
{
    if( p->address_stack.isEmpty() )
        return ;

    p->list_widget->cd( p->address_stack.pop() );
}

void SelectFiles::contextMenu( const QPoint & )
{
    if( p->list_widget->currentItem() == NULL )
        return ;

    p->context_menu->move( QCursor::pos() );
    p->context_menu->show();
}

void SelectFiles::add( const QString & file_or_folder )
{
    QFileInfo file( file_or_folder );
    if( !file.exists() )
        return;

    if( file.isDir() )
        addFolder( file_or_folder );
    else
        addFiles( QStringList() << file_or_folder );
}

void SelectFiles::addFiles( const QStringList & files )
{
    if( files.isEmpty() )
        return ;

    p->list_widget->addFiles( files );
    refresh();
}

void SelectFiles::addFolder( const QString & folder )
{
    if( folder.isEmpty() )
        return ;

    p->list_widget->addFolder( folder );
    refresh();
}

void SelectFiles::newFolder( const QString & name )
{
    if( name.isEmpty() )
        return ;

    p->list_widget->newFolder( name );
}

void SelectFiles::renameFiles( QString newName )
{
    if ( newName.isEmpty() )
        return ;

    QList<QListWidgetItem *> list;
        list << p->list_widget->selectedItems();

    int cnt = list.count();
    if( cnt == 0 )
        return;

    QStringList strList;
    for( int i=0 ; i<cnt ; i++ )
        strList << list.at(i)->text();

    if( cnt == 1 && !newName.contains("#") )
        p->list_widget->rename( strList.first() , newName );
    else
    {
        if( !newName.contains("#") )
            newName.append( " #" );

        for( int i=0 ; i<cnt ; i++ )
        {
            QString tmp( newName );
                tmp.replace( "#" , QString::number(i) );

            p->list_widget->rename( strList.at(i) , tmp );
        }
    }

    this->reloaded_slt();
}

void SelectFiles::refresh()
{
    if( p->list_widget->isEmpty() )
    {
        image_size_changed( 0 );
        return;
    }

    if( type() == DataIso )
        p->image_creator->setPathSpec( p->list_widget->gaugingRoot() );
    else
        p->data_burner->setPathSpec( p->list_widget->gaugingRoot() );
}

void SelectFiles::addFiles()
{
    SDialogTools::getOpenFileNames( this , this , SLOT(addFiles(QStringList)) , tr("Add Files") );
}

void SelectFiles::addFolder()
{
    SDialogTools::getExistingDirectory( this , this , SLOT(addFolder(QString)) , tr("Add Folder") );
}

void SelectFiles::newFolder()
{
    SDialogTools::getLineEdit( this , this , SLOT(newFolder(QString)) , tr("New Folder") , tr("Please Enter your new folder name:") );
}

void SelectFiles::renameFiles()
{
    QList<QListWidgetItem *> list;
        list << p->list_widget->selectedItems();

    QString default_text;
    int cnt = list.count();
    if( cnt == 0 )
        return ;
    else if( cnt == 1 )
        default_text = list.first()->text();
    else
        default_text = tr("New Name") + " #";

    SDialogTools::getLineEdit( this , this , SLOT(renameFiles(QString)) , tr("Rename") , tr("New Name") , QLineEdit::Normal , default_text );
}

int SelectFiles::copiesNumber() const
{
    return p->option_select->copiesNumber();
}

int SelectFiles::speed() const
{
    return p->option_select->speed();
}

bool SelectFiles::eject() const
{
    return p->option_select->eject();
}

bool SelectFiles::onFly() const
{
    return p->option_select->onFly();
}

bool SelectFiles::mount() const
{
    return p->option_select->mount();
}

bool SelectFiles::multiSession() const
{
    return p->option_select->multiSession();
}

bool SelectFiles::dummy() const
{
    return p->option_select->dummy();
}

bool SelectFiles::library() const
{
    return p->option_select->library();
}

QString SelectFiles::libraryName() const
{
    return p->option_select->libraryName();
}

QString SelectFiles::libraryTags() const
{
    return p->option_select->libraryTags();
}

QString SelectFiles::scanName() const
{
    return p->option_select->scanName();
}

QString SelectFiles::volumeLabel() const
{
    QString str = p->volume_line->text();
    if( str.isEmpty() )
        str = tr( "New Disc" );

    return str;
}

QString SelectFiles::copyRight() const
{
    QString str = p->copyright_line->text();
    if( str.isEmpty() )
        str = tr( "New Disc" );

    return str;
}

QString SelectFiles::publisher() const
{
    QString str = p->publisher_line->text();/*
    if( str.isEmpty() )
        str = SDesktopFunctions::findEnvVariable( "USER" );*/

    return str;
}

QString SelectFiles::applicationId() const
{
    QString str = p->application_line->text();
    if( str.isEmpty() )
        str = p->application_id_str;

    return str;
}

QString SelectFiles::systemId() const
{
    QString str = p->system_line->text();
    if( str.isEmpty() )
        str = p->systemInfo.systemName() + " " + p->systemInfo.systemVersion();

    return str;
}

QString SelectFiles::abstract() const
{
    QString str = p->abstract_line->text();
    return str;
}

QString SelectFiles::biblio() const
{
    QString str = p->biblio_line->text();
    return str;
}

void SelectFiles::setDevice( const QString & bus_len_id )
{
    for( int i=0 ; i<p->devices.count() ; i++ )
        if( p->devices.at(i).toQString() == bus_len_id )
        {
            p->devices_combo->setCurrentIndex( i );
            return ;
        }
}

void SelectFiles::setScan( const QString & str )
{
    p->option_select->setScan( str );
}

void SelectFiles::setCopiesNumber( int value )
{
    p->option_select->setCopiesNumber( value );
}

void SelectFiles::setSpeed( int speed )
{
    p->option_select->setSpeed( speed );
}

void SelectFiles::setEject( bool stt )
{
    p->option_select->setEject( stt );
}

void SelectFiles::setDummy( bool stt )
{
    p->option_select->setDummy( stt );
}

void SelectFiles::setMultiSession( bool stt )
{
    p->option_select->setMultiSession( stt );
}

void SelectFiles::setOnFly( bool stt )
{
    p->option_select->setOnFly( stt );
}

void SelectFiles::setMount( bool stt )
{
    p->option_select->setMount( stt );
}

void SelectFiles::setVolumeLabel( const QString & name )
{
    p->volume_line->setText( name );
}

void SelectFiles::setCopyRight( const QString & name )
{
    p->copyright_line->setText( name );
}

void SelectFiles::setPublisher( const QString & name )
{
    p->publisher_line->setText( name );
}

void SelectFiles::setApplicationId( const QString & name )
{
    p->application_line->setText( name );
}

void SelectFiles::setSystemId( const QString & name )
{
    p->system_line->setText( name );
}

void SelectFiles::setAbstract( const QString & name )
{
    p->abstract_line->setText( name );
}

void SelectFiles::setBiblio( const QString & name )
{
    p->biblio_line->setText( name );
}

void SelectFiles::removeCurrent()
{
    p->list_widget->removeCurrent();
    this->reloaded_slt();
}

void SelectFiles::closeEvent( QCloseEvent *event )
{
    if( p->folder_graph->isEmpty() )
    {
        event->accept();
        return ;
    }

    QMessageBox msgBox;
        msgBox.setText( tr("Are you sure you want to quit Data Disc ?") );
        msgBox.setWindowTitle( tr("Confirm Quit") );
        msgBox.setStandardButtons( QMessageBox::No | QMessageBox::Yes );
        msgBox.setDefaultButton( QMessageBox::No );
        msgBox.setIcon( QMessageBox::Warning );

    int ret = msgBox.exec();

    if( ret == QMessageBox::Yes )
        event->accept();
    else
        event->ignore();
}

SelectFiles::~SelectFiles()
{
    delete p;
}
