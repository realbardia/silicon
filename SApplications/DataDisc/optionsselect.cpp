#include "optionsselect.h"
#include "ui_options.h"

#include <SAnimation>

#include <QCheckBox>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>

class OptionsSelectPrivate
{
public:
    SAnimation *animation;

    QScrollArea *scroll_area;
    QWidget     *base;
    Ui::OptionsUi *options_ui;

    QVBoxLayout *this_layout;
};

OptionsSelect::OptionsSelect( QWidget *parent ) :
    QWidget(parent)
{
    p = new OptionsSelectPrivate;

    setFixedHeight( 0 );

    p->animation = new SAnimation( this );
        p->animation->setFixOptions( SAnimation::WIDTH_FIX );
        p->animation->setFrameRate( 12 );

    p->base = new QWidget();
    p->options_ui = new Ui::OptionsUi;
        p->options_ui->setupUi( p->base );

    p->scroll_area = new QScrollArea();
        p->scroll_area->setWidget( p->base );
        p->scroll_area->setWidgetResizable( true );
        p->scroll_area->setFrameShadow( QFrame::Plain );
        p->scroll_area->setFrameShape( QFrame::NoFrame );
        p->scroll_area->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        p->scroll_area->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
        p->scroll_area->viewport()->setAutoFillBackground( false );
        //p->scroll_area->viewport()->setStyleSheet( "QWidget{background-color : transparent}" );
        p->scroll_area->hide();

    p->this_layout = new QVBoxLayout( this );
        p->this_layout->addWidget( p->scroll_area );
        p->this_layout->setContentsMargins( 0 , 0 , 0 , 0 );

    connect( p->animation                 , SIGNAL(finished())    , this                          , SLOT(animation_finished()) );
    connect( p->options_ui->scan_check    , SIGNAL(toggled(bool)) , p->options_ui->scan_widget    , SLOT(setVisible(bool))       );
    connect( p->options_ui->library_check , SIGNAL(toggled(bool)) , p->options_ui->library_widget , SLOT(setVisible(bool))       );

    init_defaults();

    p->options_ui->scan_check->setChecked( false );
}

void OptionsSelect::init_defaults()
{
    p->options_ui->scan_check->setChecked( true );
    p->options_ui->library_check->setChecked( false );
}

void OptionsSelect::setCurrentDevice( const SDeviceItem & device )
{
    const SDiscFeatures & disc = device.currentDiscFeatures();

    /*! -------------------- Scanner Options -------------------------*/
    p->options_ui->scan_line->setText( disc.volume_label_str );


    /*! --------------------- Speed Options --------------------------*/
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

void OptionsSelect::autoExpand()
{
    if( height() == 0 )
        expand();
    else
        unexpand();
}

void OptionsSelect::expand()
{
    if( height() != 0 )
        return ;

    p->scroll_area->hide();
    p->animation->start( this , QSize(0,213) , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
}

void OptionsSelect::unexpand()
{
    if( height() == 0 )
        return ;

    p->scroll_area->hide();
    p->animation->start( this , QSize(0,0) , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
}

void OptionsSelect::setImageBurn( bool stt )
{
    setDiscBurn( !stt );
}

void OptionsSelect::setDiscBurn( bool stt )
{
    p->options_ui->image_widget->setHidden( stt );
    p->options_ui->disc_widget->setHidden( !stt );
}

void OptionsSelect::animation_finished()
{
    p->scroll_area->show();
}

int OptionsSelect::copiesNumber() const
{
    return p->options_ui->copies_spin->value();
}

int OptionsSelect::speed() const
{
    return p->options_ui->speed_combo->currentText().toInt();
}

bool OptionsSelect::eject() const
{
    return p->options_ui->eject_check->isChecked();
}

bool OptionsSelect::onFly() const
{
    return p->options_ui->on_fly_check->isChecked();
}

bool OptionsSelect::mount() const
{
    return p->options_ui->mount_check->isChecked();
}

bool OptionsSelect::multiSession() const
{
    return p->options_ui->multi_session_check->isChecked();
}

bool OptionsSelect::dummy() const
{
    return p->options_ui->dummy_check->isChecked();
}

bool OptionsSelect::library() const
{
    return p->options_ui->library_check->isChecked();
}

QString OptionsSelect::libraryName() const
{
    return p->options_ui->library_name_line->text();
}

QString OptionsSelect::libraryTags() const
{
    return p->options_ui->library_tags_line->text();
}

QString OptionsSelect::scanName() const
{
    return p->options_ui->scan_line->text();
}

void OptionsSelect::setScan( const QString & str )
{
    p->options_ui->scan_check->setChecked( !str.isEmpty() );
    p->options_ui->scan_line->setText( str );
}

void OptionsSelect::setCopiesNumber( int value )
{
    p->options_ui->copies_spin->setValue( value );
}

void OptionsSelect::setSpeed( int speed )
{
    p->options_ui->speed_combo->setEditText( QString::number(speed) );
}

void OptionsSelect::setEject( bool stt )
{
    p->options_ui->eject_check->setChecked( stt );
}

void OptionsSelect::setDummy( bool stt )
{
    p->options_ui->dummy_check->setChecked( stt );
}

void OptionsSelect::setMultiSession( bool stt )
{
    return p->options_ui->multi_session_check->setChecked( stt );
}

void OptionsSelect::setOnFly( bool stt )
{
    p->options_ui->on_fly_check->setChecked( stt && p->options_ui->on_fly_check->isEnabled() );
}

void OptionsSelect::setMount( bool stt )
{
    p->options_ui->mount_check->setChecked( stt );
}

OptionsSelect::~OptionsSelect()
{
    delete p->options_ui;
    delete p;
}
