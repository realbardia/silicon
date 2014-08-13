/*
 * File:      silicon.cpp
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

#include <QTimer>
#include <QQueue>
#include <QDebug>
#include <QFile>

#include "splugin.h"
#include "spage.h"
#include "sapplication.h"
#include "sprogresslistitem.h"
#include "smenupanel.h"
#include "sdialogmethod.h"
#include "sdialog.h"

#include "silicon.h"
#include "libsilicon_prev.h"

SiliconDataBox *data_box = 0;
SiliconSignalsBox *signals_box = 0;
SiliconSet *locked_on = 0;





/*================================*
 |                                |
 |         Other Functions        |
 |                                |
 *================================*/

void SPage::registerToSilicon()
{
    QObject::connect( this , SIGNAL(destroyed(SPage*)) , signals_box , SLOT(closeRequest(SPage*)) );
    signals_box->registerPage( this );
}

void SPage::setVisible( bool visible )
{
    QWidget::setVisible( visible );

    switch( static_cast<int>(this->pageType()) )
    {
    case SPage::TabedPage :
        signals_box->pageVisiblityChange( this , visible );
        break;
    case SPage::WindowedPage :
        signals_box->pageVisiblityChange( this , visible );
        setWindowState( Qt::WindowActive );
        break;
    }
}

void SPage::pageTypeChanged()
{
    signals_box->pageTypeChanged( this );
}

void SPage::pageSettingsChanged()
{
    signals_box->pageSettingsChanged( this );
}

void SApplication::registerToSilicon()
{
    QObject::connect( this , SIGNAL(destroyed(SApplication*)) , signals_box , SLOT(closeRequest(SApplication*)) );
    signals_box->registerApplication( this );
}

bool SApplication::quit()
{
    QCloseEvent event;
    closeEvent( &event );

    if( event.isAccepted() )
    {
        signals_box->deleteApplication( this );
        return true;
    }
    else
    {
        return false;
    }
}

void SProgressListItem::registerItem()
{
    signals_box->registerProgressItem( this );
}

void SMenuPanel::update()
{
    signals_box->menuPanelUpdated( this );

    emit updated();
    emit updated( this );
}

bool SPlugin::start()
{
    if( isStarted() )
        return true;

    signals_box->pluginStarted( this );

    bool result = this->startEvent();
    setStarted( result );

    if( !result )
        signals_box->pluginStopped( this );

    return result;
}

bool SPlugin::stop()
{
    if( !isStarted() )
        return true;

    signals_box->pluginStopped( this );

    bool result = this->stopEvent();
    setStarted( !result );

    if( !result )
        signals_box->pluginStarted( this );

    return result;
}

void SDialogMethod::init()
{
    signals_box->registerDialogMethod( this );
}

void SDialogMethod::kill()
{
    signals_box->registerDialogMethod( 0 );
}

void SDialog::init()
{
    setWindowFlags( Qt::Widget );
    setAttribute( Qt::WA_DeleteOnClose , true );
    signals_box->registerDialog( this );
}

void SDialog::setVisible( bool visible )
{
    QWidget::setVisible( visible );
    signals_box->dialogVisiblityChange( this , visible );
}




/*================================*
 |                                |
 |        SiliconSignalsBox       |
 |                                |
 *================================*/

void SiliconSignalsBox::loadApp( const QString & name , const QVariantList & args , void *pack )
{
    emit this->loadAppSignal( name , args , pack );
}

void SiliconSignalsBox::sendArgument( int app_tab_index , const QVariantList & args )
{
    emit this->sendArgumentSignal( app_tab_index , args );
}

void SiliconSignalsBox::openFile( const QString & fileName )
{
    emit this->openFileSignal( fileName );
}

void SiliconSignalsBox::message( Silicon::MessageType type , const QString & title , const QString & message , const QIcon & icon )
{
    emit this->message( type , title , message , icon );
}

void SiliconSignalsBox::showProcessess()
{
    emit this->showProcessessSignal();
}

void SiliconSignalsBox::applicationLoaded( const QString & name )
{
    emit this->applicationLoadedSignal( name );
}

void SiliconSignalsBox::applicationGoingToLoad( const QString & name )
{
    emit this->applicationGoingToLoadSignal( name );
}

void SiliconSignalsBox::pluginStarted( SPlugin *plugin )
{
    emit this->pluginStartedSignal( plugin );
}

void SiliconSignalsBox::pluginStopped( SPlugin *plugin )
{
    emit this->pluginStoppedSignal( plugin );
}

void SiliconSignalsBox::pageVisiblityChange( SPage *page , bool stt )
{
    emit this->pageVisiblityChangeSignal( page , stt );
}

void SiliconSignalsBox::pageTypeChanged( SPage *page )
{
    emit this->pageTypeChangedSignal( page );
}

void SiliconSignalsBox::registerPage( SPage *page )
{
    emit this->registerPageSignal( page );
}

void SiliconSignalsBox::pageSettingsChanged( SPage *page )
{
    emit this->pageSettingsChangedSignal( page );
}

void SiliconSignalsBox::closeRequest( SPage *page )
{
    emit this->closeRequestSignal( page );
}

void SiliconSignalsBox::closeRequest( SApplication *app )
{
    emit this->closeRequestSignal( app );
}

void SiliconSignalsBox::registerApplication( SApplication *app )
{
    emit this->registerApplicationSignal( app );
}

void SiliconSignalsBox::deleteApplication( SApplication *app )
{
    emit this->deleteApplicationSignal( app );
}

void SiliconSignalsBox::registerProgressItem( SProgressListItem *item )
{
    emit this->registerProgressItemSignal( item );
}

void SiliconSignalsBox::registerDialog( SDialog *dialog )
{
    emit this->registerDialogSignal( dialog );
}

void SiliconSignalsBox::dialogVisiblityChange( SDialog *dialog , bool stt )
{
    emit this->dialogVisiblityChangeSignal( dialog , stt );
}

void SiliconSignalsBox::registerDialogMethod( SDialogMethod *method )
{
    emit this->registerDialogMethodSignal( method );
}

void SiliconSignalsBox::menuPanelUpdated( SMenuPanel *panel )
{
    emit this->menuPanelUpdatedSignal( panel );
}

void SiliconSignalsBox::writeConf( const QString & head , const QString & child , const QString & value )
{
    emit writeConfSignal( head , child , value );
}

QString SiliconSignalsBox::readConf(  const QString & head , const QString & child )
{
    QString result;
    emit readConfSignal( head , child , result );

    return result;
}

void SiliconSignalsBox::saveAllConfs()
{
    emit saveAllConfsSignal();;
}

void SiliconSignalsBox::finishInitializing()
{
    emit initializingFinished();
}




/*================================*
 |                                |
 |           SiliconSet           |
 |                                |
 *================================*/

void SiliconSet::setSiliconDataBox( SiliconDataBox *d_box )
{
    if( !this->accessToChange() )
    {
        qCritical( "SiliconSet::setSiliconDataBox() : is Locked" );
        return ;
    }

    data_box = d_box;
}

void SiliconSet::setSiliconSignalsBox( SiliconSignalsBox *s_box )
{
    if( !this->accessToChange() )
    {
        qCritical( "SiliconSet::setSiliconSignalsBox() : is Locked" );
        return ;
    }

    signals_box = s_box;
}

void SiliconSet::lock()
{
    if( !this->accessToChange() )
    {
        qCritical( "SiliconSet::lock() : is Locked" );
        return ;
    }

    locked_on = this;
}

void SiliconSet::unlock()
{
    if( !this->accessToChange() )
    {
        qCritical( "SiliconSet::unlock() : is Locked" );
        return ;
    }

    locked_on = 0;
}

bool SiliconSet::accessToChange()
{
    return ( locked_on == this ||  locked_on == 0 );
}

bool SiliconSet::isLocked()
{
    return ( locked_on != 0 );
}

bool SiliconSet::isUnLocked()
{
    return !this->isLocked();
}


SiliconDataBox::SiliconDataBox( const QList<SAboutData> & appsList , const QList<SAboutData> & loaded_apps )
    : apps_list( appsList ),
      loadedApps( loaded_apps )
{
    on_initialize = true;
}




/*================================*
 |                                |
 |            Silicon             |
 |                                |
 *================================*/

QString lesser_gpl_str;
QString gpl_str;

SApplication *Silicon::loadApp( SAboutData about , const QVariantList & args )
{
    return Silicon::loadApp( about.name() , args );
}

SApplication *Silicon::loadApp( const QString & name , const QVariantList & args )
{
    struct Packet{
        SApplication *application;
    };

    Packet pack;
        pack.application = 0;

    signals_box->loadApp( name , args , &pack );
    return pack.application;
}

bool Silicon::onInitialize()
{
    return data_box->on_initialize;
}

void Silicon::initializeFinished( QObject *object , const char *method, Qt::ConnectionType type  )
{
    QObject::connect( signals_box , SIGNAL(initializingFinished()) , object , method , type );
}

void Silicon::sendArgument( int app_tab_index , const QVariantList & args )
{
    signals_box->sendArgument( app_tab_index , args );
}

void Silicon::openFile( const QString & fileName )
{
    signals_box->openFile( fileName );
}

void Silicon::message( Silicon::MessageType type , const QString & title , const QString & message , const QIcon & icon )
{
    signals_box->message( type , title , message , icon );
}

void Silicon::showProcessess()
{
    signals_box->showProcessess();
}

const QList<SAboutData> & Silicon::appsList()
{
    return data_box->apps_list;
}

const QList<SAboutData> & Silicon::loadedApps()
{
    return data_box->loadedApps;
}

const QString & Silicon::config()
{
    return data_box->config;
}

const QString & Silicon::temp()
{
    return data_box->temp;
}

QString Silicon::requestTempFile()
{
    static int i = 0;
    QString main;

    do
    {
        main = temp() + "/Temp_File_" + QString::number(i) + ".silicon";
        i++;
    } while( QFile::exists(main) );

    return main;
}

QMainWindow *Silicon::mainWindow()
{
    return data_box->main_window;
}

QWidget *Silicon::mainPanel()
{
    return data_box->main_panel;
}

SPage *Silicon::processPage()
{
    return data_box->process_page;
}

void Silicon::applicationLoadedSignal( const QObject * receiver, const char * method, Qt::ConnectionType type )
{
    QObject::connect( signals_box , SIGNAL(applicationLoadedSignal(QString)) , receiver , method , type );
}

void Silicon::applicationGoingToLoadSignal( const QObject * receiver, const char * method, Qt::ConnectionType type )
{
    QObject::connect( signals_box , SIGNAL(applicationGoingToLoadSignal(QString)) , receiver , method , type );
}

void Silicon::writeConf( const QString & head , const QString & child , const QString & value )
{
    signals_box->writeConf( head , child , value );
}

QString Silicon::readConf( const QString & head , const QString & child )
{
    return signals_box->readConf( head , child );
}

void Silicon::saveAllConfs()
{
    signals_box->saveAllConfs();
}

const QString & Silicon::GPLLicense()
{
    if( gpl_str.isEmpty() )
        gpl_str = QObject::tr( "This program is free software: you can redistribute it and/or modify it\n"
                               "under the terms of the GNU General Public License as published by the\n"
                               "Free Software Foundation, either version 3 of the License, or (at your option)\n"
                               "any later version.\n"
                               "\n"
                               "This program is distributed in the hope that it will be useful,\n"
                               "but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY\n"
                               "or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License\n"
                               "for more details.\n"
                               "\n"
                               "You should have received a copy of the GNU General Public License along\n"
                               "with this program. If not, see <http://www.gnu.org/licenses/>." );

    return gpl_str;
}

const QString & Silicon::LesserGPLLicense()
{
    if( lesser_gpl_str.isEmpty() )
        lesser_gpl_str = QObject::tr( "This program is free software: you can redistribute it and/or modify it\n"
                                      "under the terms of the GNU Lesser General Public License as published by the\n"
                                      "Free Software Foundation, either version 3 of the License, or (at your option)\n"
                                      "any later version.\n"
                                      "\n"
                                      "This program is distributed in the hope that it will be useful,\n"
                                      "but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY\n"
                                      "or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License\n"
                                      "for more details.\n"
                                      "\n"
                                      "You should have received a copy of the GNU Lesser General Public License along\n"
                                      "with this program. If not, see <http://www.gnu.org/licenses/>." );

    return lesser_gpl_str;
}
