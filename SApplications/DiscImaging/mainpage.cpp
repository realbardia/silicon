#define ICON_SIZE         QSize(48,48)
#define EXPANDED_HEIGHT   413
#define UNEXPANDED_HEIGHT 137

#include "mainpage.h"
#include "ui_options.h"

#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMimeData>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include <QFileDialog>
#include <QUrl>

#include <SMasterIcons>
#include <SDeviceList>
#include <SComboBox>
#include <SDialogTools>
#include <SDiscFeatures>

#include <QDebug>

class MainPagePrivate
{
public:
    QVBoxLayout *layout;
    QHBoxLayout *image_layout;

    QToolButton *open_btn;

    QLineEdit *dst_line;
    SComboBox *src_combo;

    QLabel    *label;

    QToolBar *toolbar;

    QAction *go_action;
    QAction *more_action;

    SDeviceList *device_list;

    Ui::OptionsUi *options_ui;
    QWidget       *options_widget;

    QList<SDeviceItem> devices;
};

MainPage::MainPage( SApplication *parent )
    : SPage( tr("Disc Imaging") , parent , SPage::WindowedPage )
{
    p = new MainPagePrivate;

    p->device_list = new SDeviceList( this );

    p->dst_line = new QLineEdit();
        p->dst_line->setReadOnly( true );
        p->dst_line->setFixedHeight( 28 );
        p->dst_line->setPlaceholderText( tr("Please select a Disc Image") );
        p->dst_line->setFocusPolicy( Qt::NoFocus );

    p->open_btn = new QToolButton();
        p->open_btn->setIcon( SMasterIcons::icon( ICON_SIZE , "document-open.png" ) );
        p->open_btn->setFixedSize( 28 , 28 );

    p->image_layout = new QHBoxLayout();
        p->image_layout->addWidget( p->dst_line );
        p->image_layout->addWidget( p->open_btn );

    p->src_combo = new SComboBox();
        p->src_combo->setIconSize( QSize(22,22) );

    p->label = new QLabel();
        p->label->setText( tr("To") );

    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );

    p->options_widget = new QWidget();
    p->options_ui = new Ui::OptionsUi;
        p->options_ui->setupUi( p->options_widget );

    p->layout = new QVBoxLayout( this );
        p->layout->addWidget( p->src_combo      );
        p->layout->addWidget( p->label          );
        p->layout->addLayout( p->image_layout   );
        p->layout->addWidget( p->options_widget );
        p->layout->addWidget( p->toolbar        );
        p->layout->setContentsMargins( 10 , 10 , 10 , 10 );

    setFixedWidth( 413 );
    setFixedHeight( EXPANDED_HEIGHT );

    p->src_combo->setCurrentIndex( 0 );

    connect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , SLOT(deviceDetected(SDeviceItem)) );
    connect( p->open_btn    , SIGNAL(clicked())                   , SLOT(select_dst_image())          );


    connect( p->options_ui->library_check  , SIGNAL(toggled(bool))     , p->options_ui->library_widget , SLOT(setVisible(bool))       );
    connect( p->options_ui->scan_check     , SIGNAL(toggled(bool))     , p->options_ui->scan_widget    , SLOT(setVisible(bool))       );
    connect( p->options_ui->custom_check   , SIGNAL(toggled(bool))     , p->options_ui->custom_widget  , SLOT(setVisible(bool))       );
    connect( p->options_ui->custom_check   , SIGNAL(toggled(bool))     , this                          , SLOT(setDefaultOptions())  );
    connect( p->options_ui->custom_to_spin , SIGNAL(valueChanged(int)) , this                          , SLOT(to_spin_changed(int)) );

    p->options_ui->library_check->setChecked( false );
    p->options_ui->scan_check->setChecked( false );
    p->options_ui->custom_check->setChecked( false );

    init_actions();
    more_prev();

    p->device_list->refresh();
    setAcceptDrops( true );
}

void MainPage::init_actions()
{
    QWidget *spr1 = new QWidget();
        spr1->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Minimum );

    p->go_action        = new QAction( SMasterIcons::icon(ICON_SIZE,"tools-media-optical-burn.png") , tr("Go")         , this );
    p->more_action      = new QAction( SMasterIcons::icon(ICON_SIZE,"edit-rename.png")              , tr("More")       , this );

    p->toolbar->addAction( p->more_action );
    p->toolbar->addWidget( spr1 );
    p->toolbar->addAction( p->go_action   );

    connect( p->go_action        , SIGNAL(triggered()) , SLOT(go_prev())          );
    connect( p->more_action      , SIGNAL(triggered()) , SLOT(more_prev())        );
}

void MainPage::deviceDetected( const SDeviceItem & device )
{
    if( !p->devices.contains(device) )
    {
        p->devices << device;
        p->src_combo->insertItem( p->devices.count()-1 , SMasterIcons::icon(ICON_SIZE,"drive-optical.png") , device.name() );
    }
    else
    {
        int index = p->devices.indexOf( device );
        p->devices.removeAt( index );
        p->devices.insert( index , device );

        p->src_combo->setItemText( index , device.name() );
    }

    setDefaultOptions();
    check_access();
}

void MainPage::to_spin_changed( int value )
{
    p->options_ui->custom_from_spin->setMaximum( value );
}

void MainPage::setDefaultOptions()
{
    int index = p->src_combo->currentIndex();
    if( index < 0 )
        return ;

    const SDeviceItem   & device = p->devices.at(index);
    const SDiscFeatures & disc   = device.currentDiscFeatures();

    int disc_block_size = 0;
    if( disc.volume_block_size_int != 0 )
        disc_block_size = disc.volume_size_int / disc.volume_block_size_int;

    /*! -------------------- Sector Options -------------------------*/
    p->options_ui->custom_to_spin->setMaximum( disc_block_size );
    p->options_ui->custom_to_spin->setValue( disc_block_size );

    p->options_ui->custom_from_spin->setMaximum( disc_block_size );
    p->options_ui->custom_from_spin->setValue( 0 );


    /*! -------------------- Library Options -------------------------*/
    p->options_ui->library_name_line->setText( disc.volume_label_str );


    /*! -------------------- Scanner Options -------------------------*/
    p->options_ui->scan_line->setText( disc.volume_label_str );
}

 QString MainPage::destinationImage() const
{
    return p->dst_line->text();
}

const SDeviceItem & MainPage::sourceDevice() const
{
    return p->devices.at( p->src_combo->currentIndex() );
}

void MainPage::go_prev()
{
    SDialogTools::getTimer( this , tr("Your Request will be starting after count down.") , 7000 , this , SIGNAL(go()) );
}

void MainPage::more_prev()
{
    if( height() == UNEXPANDED_HEIGHT )
    {
        setFixedHeight( EXPANDED_HEIGHT );
        p->options_widget->show();
        p->more_action->setText( tr("Less") );
    }
    else
    {
        setFixedHeight( UNEXPANDED_HEIGHT );
        p->options_widget->hide();
        p->more_action->setText( tr("More") );
    }
}

void MainPage::check_access()
{
    p->go_action->setEnabled( !p->dst_line->text().isEmpty() );
}

bool MainPage::scan() const
{
    return p->options_ui->scan_check->isChecked();
}

bool MainPage::mount() const
{
    return p->options_ui->mount_check->isChecked();
}

bool MainPage::eject() const
{
    return p->options_ui->eject_check->isChecked();
}

bool MainPage::library() const
{
    return p->options_ui->library_check->isChecked();
}

QString MainPage::libraryName() const
{
    return p->options_ui->library_name_line->text();
}

QString MainPage::libraryTags() const
{
    return p->options_ui->library_tags_line->text();
}

QString MainPage::scanName() const
{
    return p->options_ui->scan_line->text();
}

void MainPage::setDestinationImage( const QString & file )
{
    p->dst_line->setText( file );
    check_access();
}

void MainPage::select_dst_image()
{
    SDialogTools::getSaveFileName( this , this , SLOT(setDestinationImage(QString)) );
}

int MainPage::startSector() const
{
    return p->options_ui->custom_from_spin->value();
}

int MainPage::endSector() const
{
    return p->options_ui->custom_to_spin->value();
}

void MainPage::setSourceDevice( const QString & bus_len_id )
{
    for( int i=0 ; i<p->devices.count() ; i++ )
        if( p->devices.at(i).toQString() == bus_len_id )
        {
            p->src_combo->setCurrentIndex( i );
            return ;
        }
}

void MainPage::setScan( const QString & str )
{
    p->options_ui->scan_check->setChecked( !str.isEmpty() );
    p->options_ui->scan_line->setText( str );
}

void MainPage::setEject( bool stt )
{
    p->options_ui->eject_check->setChecked( stt );
}

void MainPage::setMount( bool stt )
{
    p->options_ui->mount_check->setChecked( stt );
}

void MainPage::setSectors( int start , int end )
{
    p->options_ui->custom_from_spin->setValue( start);
    p->options_ui->custom_to_spin->setValue( end );
}

void MainPage::dropEvent( QDropEvent *event )
{
    QList<QUrl> list = event->mimeData()->urls();
    QString url_path = list.first().path();

#ifdef Q_OS_WIN32
    if( url_path[0] == '/' )
        url_path.remove( 0 , 1 );
#endif

    setDestinationImage( url_path );

    event->acceptProposedAction();
    QWidget::dropEvent( event );
}

void MainPage::dragEnterEvent( QDragEnterEvent *event )
{
    if( !event->mimeData()->hasUrls() )
        return;

    QList<QUrl> list = event->mimeData()->urls();
    if( list.count() != 1 )
        event->ignore();
    else
        event->acceptProposedAction();

    QWidget::dragEnterEvent( event );
}

MainPage::~MainPage()
{
    delete p->options_ui;
    delete p;
}
