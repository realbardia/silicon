#define ICON_SIZE         QSize(48,48)
#define EXPANDED_HEIGHT   213
#define UNEXPANDED_HEIGHT 97

#include "mainpage.h"
#include "ui_options.h"

#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include <QFileDialog>

#include <SMasterIcons>
#include <SDeviceList>
#include <SComboBox>
#include <SDialogTools>

class MainPagePrivate
{
public:
    QVBoxLayout *layout;
    SComboBox *dst_combo;

    QToolBar *toolbar;

    QAction *go_action;
    QAction *more_action;

    SDeviceList *device_list;

    Ui::OptionsUi *options_ui;
    QWidget       *options_widget;

    QList<SDeviceItem> devices;
};

MainPage::MainPage( SApplication *parent )
    : SPage( tr("Disc Eraser") , parent , SPage::WindowedPage )
{
    p = new MainPagePrivate;

    p->device_list = new SDeviceList( this );

    p->dst_combo = new SComboBox();
        p->dst_combo->setIconSize( QSize(22,22) );

    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setStyleSheet( "QToolBar{ border-style:solid ; margin:0px }" );

    p->options_widget = new QWidget();
    p->options_ui = new Ui::OptionsUi;
        p->options_ui->setupUi( p->options_widget );

    p->layout = new QVBoxLayout( this );
        p->layout->addWidget( p->dst_combo      );
        p->layout->addWidget( p->options_widget );
        p->layout->addWidget( p->toolbar        );
        p->layout->setContentsMargins( 10 , 10 , 10 , 10 );

    setFixedWidth( 373 );
    setFixedHeight( EXPANDED_HEIGHT );

    p->dst_combo->setCurrentIndex( 0 );

    connect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , SLOT(deviceDetected(SDeviceItem)) );

    p->device_list->refresh();

    init_actions();
    more_prev();
}

void MainPage::init_actions()
{
    QWidget *spr1 = new QWidget();
        spr1->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Minimum );

    p->go_action   = new QAction( QIcon(":/DiscEraser/pics/icon.png")             , tr("Go")   , this );
    p->more_action = new QAction( SMasterIcons::icon(ICON_SIZE,"edit-rename.png") , tr("More") , this );

    p->toolbar->addAction( p->more_action );
    p->toolbar->addWidget( spr1 );
    p->toolbar->addAction( p->go_action   );

    connect( p->go_action        , SIGNAL(triggered()) , SLOT(go_prev())   );
    connect( p->more_action      , SIGNAL(triggered()) , SLOT(more_prev()) );
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
    }
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
}

bool MainPage::eject() const
{
    return p->options_ui->eject_check->isChecked();
}

bool MainPage::force() const
{
    return p->options_ui->force_check->isChecked();
}

bool MainPage::leadOut() const
{
    return p->options_ui->leadout_check->isChecked();
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

void MainPage::setEject( bool stt )
{
    p->options_ui->eject_check->setChecked( stt );
}

void MainPage::setForce( bool stt )
{
    p->options_ui->force_check->setChecked( stt );
}

void MainPage::setLeadOut( bool stt )
{
    p->options_ui->leadout_check->setChecked( stt );
}

MainPage::~MainPage()
{
    delete p->options_ui;
    delete p;
}
