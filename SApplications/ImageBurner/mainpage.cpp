#define ICON_SIZE         QSize(48,48)
#define EXPANDED_HEIGHT   413
#define UNEXPANDED_HEIGHT 137

#include "mainpage.h"
#include "ui_options.h"

#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QMimeData>
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

class MainPagePrivate
{
public:
    QVBoxLayout *layout;
    QHBoxLayout *image_layout;

    QToolButton *open_btn;

    QLineEdit *src_line;
    SComboBox *dst_combo;

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
    : SPage( tr("Image Burner") , parent , SPage::WindowedPage )
{
    p = new MainPagePrivate;

    p->device_list = new SDeviceList( this );

    p->src_line = new QLineEdit();
        p->src_line->setReadOnly( true );
        p->src_line->setFixedHeight( 28 );
        p->src_line->setPlaceholderText( tr("Please select a Disc Image") );
        p->src_line->setFocusPolicy( Qt::NoFocus );

    p->open_btn = new QToolButton();
        p->open_btn->setIcon( SMasterIcons::icon( ICON_SIZE , "document-open.png" ) );
        p->open_btn->setFixedSize( 28 , 28 );

    p->image_layout = new QHBoxLayout();
        p->image_layout->addWidget( p->src_line );
        p->image_layout->addWidget( p->open_btn );

    p->dst_combo = new SComboBox();
        p->dst_combo->setIconSize( QSize(22,22) );

    p->label = new QLabel();
        p->label->setText( tr("To") );

    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );

    p->options_widget = new QWidget();
    p->options_ui = new Ui::OptionsUi;
        p->options_ui->setupUi( p->options_widget );

    p->layout = new QVBoxLayout( this );
        p->layout->addLayout( p->image_layout   );
        p->layout->addWidget( p->label          );
        p->layout->addWidget( p->dst_combo      );
        p->layout->addWidget( p->options_widget );
        p->layout->addWidget( p->toolbar        );
        p->layout->setContentsMargins( 10 , 10 , 10 , 10 );

    setFixedWidth( 413 );
    setFixedHeight( EXPANDED_HEIGHT );

    p->dst_combo->setCurrentIndex( 0 );

    connect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , SLOT(deviceDetected(SDeviceItem)) );
    connect( p->open_btn    , SIGNAL(clicked())                   , SLOT(select_src_image())          );
    connect( p->dst_combo   , SIGNAL(currentIndexChanged(int))    , SLOT(device_index_changed(int))   );

    connect( p->options_ui->scan_check    , SIGNAL(toggled(bool)) , p->options_ui->scan_widget    , SLOT(setVisible(bool)) );

    p->options_ui->scan_check->setChecked( false );

    p->device_list->refresh();

    init_actions();
    more_prev();

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
        p->dst_combo->insertItem( p->devices.count()-1 , SMasterIcons::icon(ICON_SIZE,"drive-optical.png") , device.name() );
    }
    else
    {
        int index = p->devices.indexOf( device );
        p->devices.removeAt( index );
        p->devices.insert( index , device );

        p->dst_combo->setItemText( index , device.name() );

        device_index_changed( p->dst_combo->currentIndex() );
    }
}

void MainPage::device_index_changed( int index )
{
    if( index < 0 )
        return ;

    const SDeviceItem   & device = p->devices.at( index );
    const SDiscFeatures & disc   = device.currentDiscFeatures();

    QList<int> list;

    if( disc.volume_disc_type_str.contains("blu",Qt::CaseInsensitive) )
        list = device.deviceFeatures().bluray_speed_list;
    else if( disc.volume_disc_type_str.contains("dvd",Qt::CaseInsensitive) )
        list = device.deviceFeatures().dvd_speed_list;
    else
        list = device.deviceFeatures().cd_speed_list;

    if( list.isEmpty() )
        list << 2 << 1;

    p->options_ui->speed_combo->clear();
    for( int i=0 ; i<list.count() ; i++ )
        p->options_ui->speed_combo->addItem( QString::number(list.at(i)) );
}

 QString MainPage::sourceImage() const
{
    return p->src_line->text();
}

const SDeviceItem & MainPage::destinationDevice() const
{
    return p->devices.at( p->dst_combo->currentIndex() );
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

    setDefaultOptions();
}

void MainPage::setDefaultOptions()
{
    int index = p->dst_combo->currentIndex();
    if( index < 0 )
        return ;

    const SDeviceItem   & device = p->devices.at(index);
    const SDiscFeatures & disc   = device.currentDiscFeatures();

    /*! -------------------- Scanner Options -------------------------*/
    p->options_ui->scan_line->setText( disc.volume_label_str );
}

bool MainPage::scan() const
{
    return p->options_ui->scan_check->isChecked();
}

int MainPage::copiesNumber() const
{
    return p->options_ui->copies_spin->value();
}

int MainPage::speed() const
{
    return p->options_ui->speed_combo->currentText().toInt();
}

bool MainPage::eject() const
{
    return p->options_ui->eject_check->isChecked();
}

bool MainPage::dummy() const
{
    return p->options_ui->dummy_check->isChecked();
}

bool MainPage::remove() const
{
    return p->options_ui->remove_check->isChecked();
}

QString MainPage::scanName() const
{
    return p->options_ui->scan_line->text();
}

void MainPage::setSourceImage( const QString & file )
{
    p->src_line->setText( file );
}

void MainPage::select_src_image()
{
    SDialogTools::getOpenFileName( this , this , SLOT(setSourceImage(QString)) );
}

void MainPage::setDestinationDevice( const QString & bus_len_id )
{
    for( int i=0 ; i<p->devices.count() ; i++ )
        if( p->devices.at(i).toQString() == bus_len_id )
        {
            p->dst_combo->setCurrentIndex( i );
            return ;
        }
}

void MainPage::setScan( const QString & str )
{
    p->options_ui->scan_check->setChecked( !str.isEmpty() );
    p->options_ui->scan_line->setText( str );
}

void MainPage::setCopiesNumber( int value )
{
    p->options_ui->copies_spin->setValue( value );
}

void MainPage::setSpeed( int speed )
{
    p->options_ui->speed_combo->setEditText( QString::number(speed) );
}

void MainPage::setEject( bool stt )
{
    p->options_ui->eject_check->setChecked( stt );
}

void MainPage::setDummy( bool stt )
{
    p->options_ui->dummy_check->setChecked( stt );
}

void MainPage::setRemove( bool stt )
{
    p->options_ui->remove_check->setChecked( stt );
}

void MainPage::dropEvent( QDropEvent *event )
{
    QList<QUrl> list = event->mimeData()->urls();
    QString url_path = list.first().path();

#ifdef Q_OS_WIN32
    if( url_path[0] == '/' )
        url_path.remove( 0 , 1 );
#endif

    setSourceImage( url_path );

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
