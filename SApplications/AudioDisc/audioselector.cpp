#define LABEL_SIZE 213
#define ICON_SIZE  QSize(48,48)

#include "audioselector.h"
#include "audioitem.h"
#include "optionsselect.h"

#include <QToolBar>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>
#include <QMimeData>
#include <QProgressBar>
#include <QScrollArea>
#include <QDir>
#include <QUrl>
#include <QDropEvent>
#include <QStatusBar>
#include <QLineEdit>
#include <QToolButton>

#include <SComboBox>
#include <SMasterIcons>
#include <SWidgetList>
#include <SDialogTools>
#include <SDialog>
#include <SDesktopFunctions>
#include <SDeviceList>
#include <QtSystemInfo/QtSystemInfo.h>

class AudioSelectorPrivate
{
public:
    QLineEdit *volume_line;
    QLineEdit *copyright_line;
    QLineEdit *publisher_line;
    QLineEdit *application_line;
    QLineEdit *system_line;
    QLineEdit *abstract_line;
    QLineEdit *biblio_line;

    QVBoxLayout *layout;
    QHBoxLayout *center_layout;
    QHBoxLayout *start_panel_layout;
    QVBoxLayout *panel_layout;

    QStatusBar   *status_bar;
    QToolBar     *toolbar;
    SWidgetList  *view;
    QScrollArea  *scroll_area;
    QWidget      *lines_widget;
    QVBoxLayout  *lines_layout;
    QWidget      *panel_widget;
    QProgressBar *progress_bar;
    SComboBox    *devices_combo;

    QToolButton *go_button;
    QToolButton *more_button;

    QAction *add_action;
    QAction *properties_action;
    QAction *delete_action;
    QAction *rename_action;

    OptionsSelect *option_select;

    QtSystemInfo systemInfo;
    QString application_id_str;

    QList<SDeviceItem> devices;
    SDeviceList *device_list;
};

AudioSelector::AudioSelector( SApplication *parent )
    : SPage( tr("Audio Disc") , parent )
{
    p = new AudioSelectorPrivate;

    p->application_id_str = tr( "Silicon (Using %1)" );

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

    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );

    p->view = new SWidgetList();
        p->view->setSpacing( 3 );

    p->panel_widget = new QWidget();
        p->panel_widget->setFixedWidth( LABEL_SIZE );

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
        p->scroll_area->setFrameShadow( QFrame::Plain );
        p->scroll_area->setFrameShape( QFrame::NoFrame );
        p->scroll_area->setWidget( p->lines_widget );
        p->scroll_area->setMaximumWidth( LABEL_SIZE );
        p->scroll_area->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        p->scroll_area->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
        p->scroll_area->viewport()->setAutoFillBackground( false );
        p->scroll_area->viewport()->setStyleSheet( "QWidget{background-color : transparent}" );

    p->devices_combo = new SComboBox();
        p->devices_combo->setEditable( true );

    p->progress_bar = new QProgressBar();
        p->progress_bar->setValue( 0 );
        p->progress_bar->setFormat( tr("Beta Version, Not work yet ...") );

    p->status_bar = new QStatusBar();
        p->status_bar->addWidget( p->progress_bar , 1 );

    p->go_button = new QToolButton();
        p->go_button->setText( tr("Go") );
        p->go_button->setIcon( SMasterIcons::icon(ICON_SIZE,"tools-media-optical-burn.png") );
        p->go_button->setAutoRaise( true );
        p->go_button->setDisabled( true );
        p->go_button->setIconSize( QSize(22,22) );
        p->go_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

    p->more_button = new QToolButton();
        p->more_button->setText( tr("More") );
        p->more_button->setIcon( SMasterIcons::icon(ICON_SIZE,"edit-rename.png") );
        p->more_button->setAutoRaise( true );
        p->more_button->setIconSize( QSize(22,22) );
        p->more_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

    p->option_select = new OptionsSelect();
        p->option_select->setMaximumWidth( LABEL_SIZE );

    p->start_panel_layout = new QHBoxLayout();
        p->start_panel_layout->addWidget( p->more_button );
        p->start_panel_layout->addStretch();
        p->start_panel_layout->addWidget( p->go_button );
        p->start_panel_layout->setContentsMargins( 0 , 0 , 0 , 0 );

    p->panel_layout = new QVBoxLayout( p->panel_widget );
        p->panel_layout->addWidget( p->scroll_area );
        //p->panel_layout->addStretch();
        p->panel_layout->addWidget( p->devices_combo );
        p->panel_layout->addWidget( p->option_select );
        p->panel_layout->addLayout( p->start_panel_layout );
        p->panel_layout->setContentsMargins( 0 , 0 , 0 , 0 );

    p->center_layout = new QHBoxLayout();
        p->center_layout->addWidget( p->view );
        p->center_layout->addWidget( p->panel_widget );

    p->layout = new QVBoxLayout( this );
        //p->layout->addWidget( p->toolbar );
        p->layout->addLayout( p->center_layout );
        //p->layout->addWidget( p->progress_bar );

    connect( p->more_button , SIGNAL(clicked()) , p->option_select , SLOT(autoExpand()) );
    connect( p->go_button   , SIGNAL(clicked()) , this             , SLOT(go_prev())    );

    connect( p->devices_combo , SIGNAL(currentIndexChanged(int)) , SLOT(device_index_changed(int)) );
    connect( p->volume_line   , SIGNAL(textChanged(QString))     , SLOT(setScan(QString))          );

    init_system();
    init_actions();

    setAcceptDrops( true );
    setToolBar( p->toolbar );
    setStatusBar( p->status_bar );
}

void AudioSelector::init_actions()
{
    p->add_action        = new QAction( SMasterIcons::icon(ICON_SIZE,"archive-insert.png")      , tr("Add Audio")  , this );
    p->delete_action     = new QAction( SMasterIcons::icon(ICON_SIZE,"edit-delete.png")         , tr("Delete")     , this );
    p->properties_action = new QAction( SMasterIcons::icon(ICON_SIZE,"document-properties.png") , tr("Properties") , this );
    p->rename_action     = new QAction( SMasterIcons::icon(ICON_SIZE,"edit-rename.png")         , tr("Rename")     , this );

    p->toolbar->addAction( p->add_action );
    p->toolbar->addAction( p->delete_action );
    p->toolbar->addSeparator();
    p->toolbar->addAction( p->rename_action );
    p->toolbar->addSeparator();
    p->toolbar->addAction( p->properties_action );

    p->rename_action->setDisabled( true );
    p->properties_action->setDisabled( true );

    connect( p->add_action    , SIGNAL(triggered()) , SLOT(addFiles())       );
    connect( p->delete_action , SIGNAL(triggered()) , SLOT(removeCurrents()) );
}

void AudioSelector::init_system()
{
    p->device_list = new SDeviceList( this );

    connect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , SLOT(deviceDetected(SDeviceItem)) );

    p->device_list->refresh();
}

QStringList AudioSelector::files() const
{
    QStringList result;
    for( int i=0 ; i<p->view->count() ; i++ )
    {
        AudioItem *item = static_cast<AudioItem*>(p->view->widget(i));
        result << item->address();
    }

    return result;
}

SDeviceItem AudioSelector::currentDevice() const
{
    return p->devices.at( p->devices_combo->currentIndex() );
}

int AudioSelector::copiesNumber() const
{
    return p->option_select->copiesNumber();
}

int AudioSelector::speed() const
{
    return p->option_select->speed();
}

bool AudioSelector::eject() const
{
    return p->option_select->eject();
}

bool AudioSelector::mount() const
{
    return p->option_select->mount();
}

bool AudioSelector::multiSession() const
{
    return p->option_select->multiSession();
}

bool AudioSelector::dummy() const
{
    return p->option_select->dummy();
}

bool AudioSelector::library() const
{
    return p->option_select->library();
}

bool AudioSelector::trackAtOnce() const
{
    return p->option_select->trackAtOnce();
}

bool AudioSelector::discAtOnce() const
{
    return p->option_select->discAtOnce();
}

bool AudioSelector::swab() const
{
    return p->option_select->swab();
}

QString AudioSelector::libraryName() const
{
    return p->option_select->libraryName();
}

QString AudioSelector::libraryTags() const
{
    return p->option_select->libraryTags();
}

QString AudioSelector::scanName() const
{
    return p->option_select->scanName();
}

QString AudioSelector::volumeLabel() const
{
    QString str = p->volume_line->text();
    if( str.isEmpty() )
        str = tr( "New Disc" );

    return str;
}

QString AudioSelector::copyRight() const
{
    QString str = p->copyright_line->text();
    if( str.isEmpty() )
        str = tr( "New Disc" );

    return str;
}

QString AudioSelector::publisher() const
{
    QString str = p->publisher_line->text();/*
    if( str.isEmpty() )
        str = SDesktopFunctions::findEnvVariable( "USER" );*/

    return str;
}

QString AudioSelector::applicationId() const
{
    QString str = p->application_line->text();
    if( str.isEmpty() )
        str = p->application_id_str;

    return str;
}

QString AudioSelector::systemId() const
{
    QString str = p->system_line->text();
    if( str.isEmpty() )
        str = p->systemInfo.systemName() + " " + p->systemInfo.systemVersion();

    return str;
}

QString AudioSelector::abstract() const
{
    QString str = p->abstract_line->text();
    return str;
}

QString AudioSelector::biblio() const
{
    QString str = p->biblio_line->text();
    return str;
}

void AudioSelector::setDevice( const QString & bus_len_id )
{
    for( int i=0 ; i<p->devices.count() ; i++ )
        if( p->devices.at(i).toQString() == bus_len_id )
        {
            p->devices_combo->setCurrentIndex( i );
            return ;
        }
}

void AudioSelector::setScan( const QString & str )
{
    p->option_select->setScan( str );
}

void AudioSelector::setCopiesNumber( int value )
{
    p->option_select->setCopiesNumber( value );
}

void AudioSelector::setSpeed( int speed )
{
    p->option_select->setSpeed( speed );
}

void AudioSelector::setEject( bool stt )
{
    p->option_select->setEject( stt );
}

void AudioSelector::setDummy( bool stt )
{
    p->option_select->setDummy( stt );
}

void AudioSelector::setMultiSession( bool stt )
{
    p->option_select->setMultiSession( stt );
}

void AudioSelector::setMount( bool stt )
{
    p->option_select->setMount( stt );
}

void AudioSelector::setTrackAtOnce( bool stt )
{
    p->option_select->setTrackAtOnce( stt );
}

void AudioSelector::setDiscAtOnce( bool stt )
{
    p->option_select->setDiscAtOnce( stt );
}

void AudioSelector::setSwab( bool stt )
{
    p->option_select->setSwab( stt );
}

void AudioSelector::setVolumeLabel( const QString & name )
{
    p->volume_line->setText( name );
}

void AudioSelector::setCopyRight( const QString & name )
{
    p->copyright_line->setText( name );
}

void AudioSelector::setPublisher( const QString & name )
{
    p->publisher_line->setText( name );
}

void AudioSelector::setApplicationId( const QString & name )
{
    p->application_line->setText( name );
}

void AudioSelector::setSystemId( const QString & name )
{
    p->system_line->setText( name );
}

void AudioSelector::setAbstract( const QString & name )
{
    p->abstract_line->setText( name );
}

void AudioSelector::setBiblio( const QString & name )
{
    p->biblio_line->setText( name );
}

void AudioSelector::addFiles( const QStringList & files )
{
    bool error = false;

    for( int i=0 ; i<files.count() ; i++ )
    {
        AudioItem *item = new AudioItem();
        if( !item->setAddress(files.at(i)) )
        {
            error = true;
            delete item;
            continue;
        }

        p->view->addWidget( item );
    }

    if( error )
        SDialogTools::message( this , tr("Error") ,
                                      tr("Can't add or detect some selected files.\t\t\nformat not supported.") ,
                                      QMessageBox::Critical );

    p->go_button->setEnabled( !p->view->isEmpty() );
}

void AudioSelector::addFiles()
{
    SDialogTools::getOpenFileNames( this , this , SLOT(addFiles(QStringList)) , tr("Add Audios") );
}

void AudioSelector::removeCurrents()
{
    const QList<QWidget*> & list = p->view->currentSelected();
    for( int i=0 ; i<list.count() ; i++ )
    {
        QWidget *widget = list.at(i);

        p->view->remove( widget );
        delete widget;
    }
}

void AudioSelector::deviceDetected( const SDeviceItem & device )
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

void AudioSelector::device_index_changed( int index )
{
    p->option_select->setImageBurn( index == p->devices.count() );

    if( index == p->devices.count() )
    {
        if( dialogs().isEmpty() )
        {
            SDialogTools::getSaveFileName( this , this , SLOT(image_selected(QString)) );
            connect( dialogs().first() , SIGNAL(accepted(bool)) , SLOT(image_select_accp(bool)) );
        }
    }
    else if( index >= 0 )
    {
        p->option_select->setCurrentDevice( p->devices.at(index) );
    }
}

void AudioSelector::go_prev()
{
    setDisabled( true );
    SDialog *dialog = SDialogTools::getTimer( this , tr("Your Request will be starting after count down.") , 7000 );
    connect( dialog , SIGNAL(accepted(bool)) , SLOT(timer_finished(bool)) , Qt::QueuedConnection );

}

void AudioSelector::timer_finished( bool stt )
{
    if( stt )
        emit go();
    else
        setEnabled( true );
}

void AudioSelector::dropEvent( QDropEvent *event )
{
    QList<QUrl> list = event->mimeData()->urls();

    QStringList files;
    for( int i=0 ; i< list.count() ; i++ )
    {
        QString url_path = list.at(i).path();

#ifdef Q_OS_WIN32
        if( url_path[0] == '/' )
            url_path.remove( 0 , 1 );
#endif

        QFileInfo file( url_path );
        if( file.isDir() )
            continue;

        files << url_path;
    }

    event->acceptProposedAction();
    addFiles( files );

    QWidget::dropEvent( event );
}

void AudioSelector::dragEnterEvent( QDragEnterEvent *event )
{
    if( event->mimeData()->hasUrls() )
        event->acceptProposedAction();

    QWidget::dragEnterEvent( event );
}

AudioSelector::~AudioSelector()
{
    delete p;
}
