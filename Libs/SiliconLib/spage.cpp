/*
 * File:      spage.cpp
 * Author:    Bardia Daneshvar (bardia@getsilicon.org)
 * Copyright: (C) 2010 Bardia Daneshvar
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define PAGE_HEAD_CONF "PageTypes"
#define TABED_STR      "tabed"
#define WINDOWED_STR   "windowed"

#include "spage.h"

#include <QListWidgetItem>
#include <QApplication>
#include <QDir>
#include <QList>
#include <QStatusBar>
#include <QToolBar>

#include "sdialog.h"
#include "sapplication.h"
#include "sinnermessage.h"
#include "sinnerprogressbar.h"
#include "scolor.h"




/*================================*
 |                                |
 |            Functions           |
 |                                |
 *================================*/

int numberOfChilds( const QList<SPage *> & list )
{
    int result = list.count();
    for( int i=0 ; i<list.count() ; i++ )
        result += numberOfChilds( list.at(i)->childs() );

    return result;
}

int numberOfSubObjects( const QList<QObject *> & list )
{
    int result = list.count();
    for( int i=0 ; i<list.count() ; i++ )
        result += numberOfSubObjects( list.at(i)->children() );

    return result;
}

int numberOfSubDialogs( const QList<SDialog *> & list )
{
    int result = list.count();
    for( int i=0 ; i<list.count() ; i++ )
    {
        SDialog *dialog = list.at(i);

        result += numberOfSubDialogs( dialog->dialogs()  );
        result += numberOfSubObjects( dialog->children() );
    }

    return result;
}

int numberOfSubPages( const QList<SPage *> & list )
{
    int result = list.count();
    for( int i=0 ; i<list.count() ; i++ )
    {
        SPage *page = list.at(i);

        result += numberOfSubPages(   page->childs()   );
        result += numberOfSubObjects( page->children() );
        result += numberOfSubDialogs( page->dialogs()  );
    }

    return result;
}




/*================================*
 |                                |
 |          SPagePrivate          |
 |                                |
 *================================*/

class SPagePrivate
{
public:
    SApplication *sapp;
    SPage *page;

    SPage::PageType page_type;

    SInnerMessage *inner_message;
    SInnerProgressBar *inner_progress;
    SMenuPanel *panel;
    QToolBar   *toolbar;
    QStatusBar *statusbar;

    int app_mode;
    QList<SPage *> childs;
    QList<SDialog *> dialogs;

    QIcon icon;
    QString title;
    QString page_id;
};




/*================================*
 |                                |
 |             SPage              |
 |                                |
 *================================*/

SPage::SPage( const QString & name , SApplication *parent , PageType type )
{
    p = new SPagePrivate;
        p->sapp      = parent;
        p->page      = 0;
        p->page_type = type;
        p->toolbar   = 0;
        p->statusbar = 0;

    setTitle( name );
    init();
}

SPage::SPage( const QString & name , SPage *parent , PageType type )
{
    p = new SPagePrivate;
        p->sapp      = 0;
        p->page      = parent;
        p->page_type = type;
        p->toolbar   = 0;
        p->statusbar = 0;

    setTitle( name );
    init();
}

void SPage::init()
{
    p->panel = 0;
    p->inner_message = 0;
    p->inner_progress = 0;

    const SAboutData & about = parentApp()->parent();
    p->page_id = about.name() + "_" + about.version() + "_" + title();
    p->page_id.remove("=").remove(" ");


    /*! --------- Recover last state of the window ------------- */

    QString type_str = Silicon::readConf( PAGE_HEAD_CONF , p->page_id );
    if( type_str == WINDOWED_STR )
        p->page_type = SPage::WindowedPage;
    else if( type_str == TABED_STR )
        p->page_type = SPage::TabedPage;

    /*! -------------------------------------------------------- */

    switch( static_cast<int>(parentType()) )
    {
    case SPage::Application :
        p->sapp->addChild( this );
        break;
    case SPage::Page :
        p->page->addChild( this );
        break;
    }

    setAttribute( Qt::WA_DeleteOnClose , true );
    registerToSilicon();
}

void SPage::setPageType( SPage::PageType type )
{
    if( p->page_type == type )
        return ;

    p->page_type = type;
    pageTypeChanged();

    QString type_str;
    switch( static_cast<int>(type) )
    {
    case SPage::TabedPage :
        type_str = TABED_STR;
        break;
    case SPage::WindowedPage:
        type_str = WINDOWED_STR;
        break;
    }

    typeChanged( type );
    Silicon::writeConf( PAGE_HEAD_CONF , p->page_id , type_str );
}

SPage::PageType SPage::pageType() const
{
    return p->page_type;
}

SPage::ParentType SPage::parentType() const
{
    if( p->sapp != 0 )
        return SPage::Application;
    else
        return SPage::Page;
}

SApplication *SPage::parentApp() const
{
    if( parentType() == SPage::Application )
        return p->sapp;

    SPage *page = pageParent();
    while( page->parentType() != SPage::Application )
        page = page->pageParent();

    return page->parentApp();
}

SPage *SPage::pageParent() const
{
    if( parentType() == SPage::Page )
        return p->page;
    else
        return 0;
}

SMenuPanel *SPage::menuPanel()
{
    if( p->panel == 0 )
    {
        p->panel = new SMenuPanel( this );

        QObject::connect( p->panel , SIGNAL(updated()) , SIGNAL(panelUpdated()) );
        QObject::connect( p->panel , SIGNAL(updated(SMenuPanel*)) , SIGNAL(panelUpdated(SMenuPanel*)) );
        QObject::connect( p->panel , SIGNAL(updated()) , SLOT(pageSettingsChanged()) );
    }

    return p->panel;
}

void SPage::setMenuPanel( SMenuPanel *menuPanel )
{
    p->panel = menuPanel;

    if( p->panel != 0 )
    {
        QObject::connect( p->panel , SIGNAL(updated()) , SIGNAL(panelUpdated()) );
        QObject::connect( p->panel , SIGNAL(updated(SMenuPanel*)) , SIGNAL(panelUpdated(SMenuPanel*)) );
        QObject::connect( p->panel , SIGNAL(updated()) , SLOT(pageSettingsChanged()) );
    }

    emit panelUpdated();
    emit panelUpdated( p->panel );
}

void SPage::setToolBar( QToolBar *toolbar )
{
    p->toolbar = toolbar;
    pageSettingsChanged();
}

QToolBar *SPage::toolBar() const
{
    return p->toolbar;
}

void SPage::setStatusBar( QStatusBar *statusbar )
{
    p->statusbar = statusbar;
    pageSettingsChanged();
}

QStatusBar *SPage::statusBar() const
{
    return p->statusbar;
}

void SPage::showMessage( QMessageBox *message , const QSize & size )
{
    if( p->inner_message != 0 )
        delete p->inner_message;

    SColor color( palette().color( QPalette::Window ) );
        color.invert();
        color.setAlpha( 128 );

    p->inner_message = new SInnerMessage( message , true , this );
        p->inner_message->showMessage( size );
        p->inner_message->setColor( color );
        p->inner_message->setGeometry( 0 , 0 , this->size().width() , this->size().height() );
}

void SPage::hideMessage()
{
    if( p->inner_message == 0 )
        return ;

    delete p->inner_message;
    p->inner_message = 0;
}

void SPage::switchPageType()
{
    switch( static_cast<int>(pageType()) )
    {
    case SPage::TabedPage :
        setPageType( SPage::WindowedPage );
        break;
    case SPage::WindowedPage:
        setPageType( SPage::TabedPage );
        break;
    }
}

void SPage::setBusy( bool stt )
{
    if( stt && p->inner_progress == 0 )
    {
        //setDisabled( true );
        SColor color( palette().color( QPalette::Window ) );
            color.invert();
            color.setAlpha( 128 );

        p->inner_progress = new SInnerProgressBar( this );
            p->inner_progress->setColor( color );
            p->inner_progress->setGeometry( 0 , 0 , size().width() , size().height() );
            p->inner_progress->start();
            p->inner_progress->show();
    }
    else if( !stt && p->inner_progress != 0 )
    {
        //setEnabled( true );
        delete p->inner_progress;
        p->inner_progress = 0;
    }
}

void SPage::addChild( SPage *page )
{
    if( p->childs.contains(page) )
        return ;

    p->childs << page;
    QObject::connect( page , SIGNAL(destroyed(SPage*)) , SLOT(pageDestroyed(SPage*)) );
}

const QList<SPage *> & SPage::childs() const
{
    return p->childs;
}

void SPage::addDialog( SDialog *dialog )
{
    if( p->dialogs.contains(dialog) )
        return ;

    p->dialogs << dialog;
    QObject::connect( dialog , SIGNAL(destroyed(SDialog*)) , SLOT(dialogDestroyed(SDialog*)) );
}

const QList<SDialog *> & SPage::dialogs() const
{
    return p->dialogs;
}

int SPage::numberOfAllChilds() const
{
    return numberOfChilds( p->childs );
}

int SPage::numberOfAllSubObjects() const
{
    return numberOfSubPages( p->childs ) + numberOfSubObjects( children() ) + numberOfSubDialogs( dialogs() );
}

void SPage::setTitle( const QString & title )
{
    p->title = title;
    pageSettingsChanged();
}

const QString & SPage::title() const
{
    return p->title;
}

void SPage::setIcon( const QIcon & icon )
{
    p->icon = icon;
    pageSettingsChanged();
}

const QIcon & SPage::icon() const
{
    if( p->icon.isNull() )
        return parentApp()->parent().icon();

    return p->icon;
}

void SPage::pageDestroyed( SPage *page )
{
    p->childs.removeOne( page );
}

void SPage::dialogDestroyed( SDialog *dialog )
{
    p->dialogs.removeOne( dialog );
}

void SPage::resizeEvent( QResizeEvent *event )
{
    QWidget::resizeEvent( event );

    if( p->inner_progress != 0 )
        p->inner_progress->setGeometry( 0 , 0 , event->size().width() , event->size().height() );

    if( p->inner_message != 0 )
        p->inner_message->setGeometry( 0 , 0 , event->size().width() , event->size().height() );

    emit resized( event->size() );
}

void SPage::typeChanged( PageType )
{

}

bool SPage::event( QEvent *event )
{
    switch( static_cast<int>(event->type()) )
    {
    case QEvent::EnabledChange :
        {
            if( p->toolbar   )  p->toolbar->setEnabled(   isEnabled() );
            if( p->statusbar )  p->statusbar->setEnabled( isEnabled() );
            if( p->panel     )  p->panel->setEnabled(     isEnabled() );
            break;
        }

    }

    return QWidget::event( event );
}

SPage::~SPage()
{
    emit destroyed( this );

    while( !p->childs.isEmpty() )
        delete p->childs.at(0);

    while( !p->dialogs.isEmpty() )
        delete p->dialogs.at(0);

    if( p->toolbar != 0 )
        delete p->toolbar;

    if( p->statusbar != 0 )
        delete p->statusbar;

    delete p;
}
