#include "sapplicationsconfigure.h"
#include "ui_sapplicationsconfigure.h"

#include <QFileDialog>
#include <libsilicon_prev.h>

SApplicationsConfigure::SApplicationsConfigure( PerConf *conf , STabBar *tabbar , SiliconDataBox *d_box , QWidget *parent) :
    QWidget(parent),
    ui( new Ui::SApplicationsConfigureUI )
{
    prc = conf;
    tab_bar = tabbar;
    data_box = d_box;

    ui->setupUi( this );

    connect( ui->temp_line , SIGNAL(textChanged(QString)) , SIGNAL(tempChanged(QString)) );
    connect( ui->open_temp , SIGNAL(clicked())            , SLOT(setTemp())              );

    setup();
    setupColors();
}

void SApplicationsConfigure::setup()
{
    data_box->temp = prc->readOption(PerConf::TEMP_ADDRESS);
    ui->temp_line->setText( data_box->temp );
}

void SApplicationsConfigure::setupColors()
{
    default_palette = QApplication::palette();

    colors_hash.insert( PerConf::SAPPS_WINDOW         , new SColorButton( tr("Window Color") )          );
    colors_hash.insert( PerConf::SAPPS_WINDOW_TEXT    , new SColorButton( tr("Window Text Color") )     );
    colors_hash.insert( PerConf::SAPPS_BASE           , new SColorButton( tr("Base Color") )            );
    colors_hash.insert( PerConf::SAPPS_BASE_TEXT      , new SColorButton( tr("Base Text Color") )       );
    colors_hash.insert( PerConf::SAPPS_BUTTON_TEXT    , new SColorButton( tr("Button Text Color") )     );
    colors_hash.insert( PerConf::SAPPS_BUTTON         , new SColorButton( tr("Button Color") )          );
    colors_hash.insert( PerConf::SAPPS_BRIGHT_TEXT    , new SColorButton( tr("Bright Text Color") )     );
    colors_hash.insert( PerConf::SAPPS_DARK           , new SColorButton( tr("Dark Color") )            );
    colors_hash.insert( PerConf::SAPPS_HIGHLIGHT      , new SColorButton( tr("Highlight Color") )       );
    colors_hash.insert( PerConf::SAPPS_HIGHLIGHT_TEXT , new SColorButton( tr("Highlight Text Color") )  );
    colors_hash.insert( PerConf::SAPPS_LIGHT          , new SColorButton( tr("Light Color") )           );
    colors_hash.insert( PerConf::SAPPS_LINK           , new SColorButton( tr("Link Color") )            );
    colors_hash.insert( PerConf::SAPPS_LINK_VISITED   , new SColorButton( tr("Link Visited Color") )    );
    colors_hash.insert( PerConf::SAPPS_MID            , new SColorButton( tr("Mid Color") )             );
    colors_hash.insert( PerConf::SAPPS_MIDLIGHT       , new SColorButton( tr("Midlight Color") )        );
    colors_hash.insert( PerConf::SHADOW               , new SColorButton( tr("Shadow Color") )          );

    colors_role.insert( PerConf::SAPPS_WINDOW         , QPalette::Window          );
    colors_role.insert( PerConf::SAPPS_WINDOW_TEXT    , QPalette::WindowText      );
    colors_role.insert( PerConf::SAPPS_BASE           , QPalette::Base            );
    colors_role.insert( PerConf::SAPPS_BASE_TEXT      , QPalette::Text            );
    colors_role.insert( PerConf::SAPPS_BUTTON_TEXT    , QPalette::ButtonText      );
    colors_role.insert( PerConf::SAPPS_BUTTON         , QPalette::Button          );
    colors_role.insert( PerConf::SAPPS_BRIGHT_TEXT    , QPalette::BrightText      );
    colors_role.insert( PerConf::SAPPS_DARK           , QPalette::Dark            );
    colors_role.insert( PerConf::SAPPS_HIGHLIGHT      , QPalette::Highlight       );
    colors_role.insert( PerConf::SAPPS_HIGHLIGHT_TEXT , QPalette::HighlightedText );
    colors_role.insert( PerConf::SAPPS_LIGHT          , QPalette::Light           );
    colors_role.insert( PerConf::SAPPS_LINK           , QPalette::Link            );
    colors_role.insert( PerConf::SAPPS_LINK_VISITED   , QPalette::LinkVisited     );
    colors_role.insert( PerConf::SAPPS_MID            , QPalette::Mid             );
    colors_role.insert( PerConf::SAPPS_MIDLIGHT       , QPalette::Midlight        );
    colors_role.insert( PerConf::SAPPS_SHADOW         , QPalette::Shadow          );


    SColorButton *tmp;
    QList<int> list = colors_hash.keys();
    for( int i=0 ; i<list.count() ; i++ )
    {
        QString color_str = prc->readOption( list.at(i) );

        tmp = colors_hash.value( list.at(i) );
            tmp->setCheckable( true );
            tmp->setChecked( !color_str.isEmpty() );

        if( color_str.isEmpty() )
            tmp->setColor( default_palette.color( colors_role.value(list.at(i)) ) );
        else
            tmp->setColor( QColor(color_str) );

        ui->colors_layout->addWidget( tmp );
        this->setColors( list.at(i) , color_str );

        QObject::connect( tmp , SIGNAL(colorSelected(SColorButton*)) , SLOT(colorChanged(SColorButton*)) );
    }

    ui->colors_layout->addSpacerItem(  new QSpacerItem( 2 , 4 , QSizePolicy::Minimum , QSizePolicy::Expanding ) );
}

void SApplicationsConfigure::setTemp()
{
    QString temp = QFileDialog::getExistingDirectory( this , tr("Select Temp directory") );
    if( temp.isEmpty() )
        return;

    data_box->temp = temp;
    ui->temp_line->setText( temp );

    prc->setOption( PerConf::TEMP_ADDRESS , temp );
    prc->save();
}

void SApplicationsConfigure::colorChanged( SColorButton *button )
{
    int type = colors_hash.key( button );

    QString res;
    if( button->isChecked() )
        res = button->color().name();

    prc->setOption( type , res );
    prc->save();

    this->setColors( type , res );
}

void SApplicationsConfigure::setColors( int type , const QString & color_str )
{
    QColor color( color_str );
    if( color_str.isEmpty() )
        color = default_palette.color( colors_role.value(type) );

    QPalette palette = tab_bar->stackedPalette();
        palette.setColor( QPalette::Active , colors_role.value(type) , color );

    tab_bar->setStackedPalette( palette );
}
