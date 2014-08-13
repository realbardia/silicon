#include "about.h"

#include <QSize>
#include <QIcon>
#include <QSizePolicy>

#include <smastericons.h>

About::About(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle( tr("About") );
    this->setWindowIcon( SMasterIcons::icon(QSize(48,48),"help-about.png") );

    list_widget = new QListWidget();
        list_widget->setIconSize( QSize(48,48) );
        list_widget->setGridSize( QSize( 128 , 92 ) );
        list_widget->setViewMode( QListView::IconMode );

    about_widget = new QWidget();
    app_info_widget = new AppInfoWidget();

    back_button = new QPushButton( SMasterIcons::icon(QSize(48,48),"go-previous.png") , tr("Back") );

    about_panel_layout = new QHBoxLayout();
        about_panel_layout->addSpacerItem( new QSpacerItem( 2 , 4 , QSizePolicy::Expanding , QSizePolicy::Minimum ) );
        about_panel_layout->addWidget( back_button );

    about_layout = new QVBoxLayout( about_widget );
        about_layout->addWidget( app_info_widget );
        about_layout->addLayout( about_panel_layout );
        about_layout->setContentsMargins( 0 , 0 , 0 , 0 );

    stacked_widget = new QStackedWidget();
        stacked_widget->addWidget( list_widget );
        stacked_widget->addWidget(about_widget );

    base_layout = new QHBoxLayout( this );
        base_layout->addWidget( stacked_widget );

    connect( list_widget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , this , SLOT(showItem(QListWidgetItem*)) );
    connect( back_button , SIGNAL(clicked()) , this , SLOT(showFirstPage()) );
}

void About::addListAbout( const QList<SAboutData> & l )
{
    list << l;

    for( int i=0 ; i<l.count() ; i++ )
    {
        QListWidgetItem *item = new QListWidgetItem( l.at(i).icon() , l.at(i).translatedName() );
        list_widget->addItem( item );
    }
}

void About::setAboutList( const QList<SAboutData> & l )
{
    list.clear();
    hash.clear();
    list_widget->clear();

    list << l;

    for( int i=0 ; i<list.count() ; i++ )
    {
        QListWidgetItem *item = new QListWidgetItem( list.at(i).icon() , list.at(i).translatedName() );
        list_widget->addItem( item );

        hash.insert( item , list.at(i) );
    }
}

void About::showItem( QListWidgetItem *item )
{
    app_info_widget->setAboutData( hash.value(item) );
    stacked_widget->setCurrentIndex( 1 );
}

void About::showFirstPage()
{
    stacked_widget->setCurrentIndex( 0 );
}

void About::resizeEvent( QResizeEvent *event )
{
    QWidget::resizeEvent( event );

    list_widget->sortItems();
    list_widget->insertItem( 0 , list_widget->takeItem(list_widget->row(hash.key(list.first()))) );
}
