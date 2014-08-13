/*
 * File:      silicon.h
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

#ifndef SILICON_H
#define SILICON_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantList>

#include "saboutdata.h"
#include "libsilicon_global.h"

class SPage;
class SApplication;
class QMainWindow;


/*! \class
 * The Silicon class provides a class that
 * make connections and transfare datas between of
 * the sapplications and silicon kernel with static functions.
 */
class LIBSILICON_EXPORT Silicon
{

/*! List of All Available Functions :
 *
 * ENUM TYPES:
 *     enum MessageType{
 *         Warning,
 *         Question,
 *         Information,
 *         Critical
 *     };
 *
 * PUBLIC FUNCTIONS:
 *     static void loadApp( SAboutData about , const QStringList & args = QStringList() );
 *     static void loadApp( const QString & name , const QStringList & args = QStringList() );
 *
 *     static bool onInitialize();
 *     static void initializeFinished( QObject *object , const char *method, Qt::ConnectionType type = Qt::AutoConnection );
 *
 *     static void openFile( const QString & fileName );
 *
 *     static void sendArgument( int app_tab_index , const QStringList & args );
 *     static void message( Silicon::MessageType type , const QString & title , const QString & message , const QIcon & icon = QIcon() );
 *     static void showProcessess();
 *
 *     static const QList<SAboutData> & appsList();
 *     static const QList<SAboutData> & loadedApps();
 *
 *     static const QString & config();
 *     static const QString & temp();
 *     static const QString & requestTempFile();
 *
 *     static QMainWindow *mainWindow();
 *     static QWidget *mainPanel();
 *     static SPage *processPage();
 *
 *     static void applicationLoadedSignal( const QObject * receiver, const char * method, Qt::ConnectionType type = Qt::AutoConnection );
 *     static void applicationGoingToLoadSignal( const QObject * receiver, const char * method, Qt::ConnectionType type = Qt::AutoConnection );

 *     static void writeConf( const QString & head , const QString & child , const QString & value );
 *     static QString readConf( const QString & head , const QString & child );
 *     static void saveAllConfs();
 *
 *     static const QString & GPLLicense();
 *     static const QString & LesserGPLLicense();
 *
 */

public:
    enum MessageType{
        Warning,        /*! For reporting non-critical errors. */
        Question,       /*! For asking a question during normal operations. */
        Information,    /*! For reporting information about normal operations. */
        Critical        /*! For reporting critical errors. */
    };

    /*!
     * Send a request to Silicon kernel for loading or
     * Running another SApplication with args argumants.
     * \sa appsList
     */
    static SApplication *loadApp( SAboutData about , const QVariantList & args = QVariantList() );
    static SApplication *loadApp( const QString & name , const QVariantList & args = QVariantList() );


    /*!
     * \brief onInitialize
     * \return true if silicon is on startup initialize, else returns false.
     */
    static bool onInitialize();

    /*!
     * \brief initializeFinished
     * \param object
     * \param method Call method from object when Silicon startup initialize process finished.
     */
    static void initializeFinished( QObject *object , const char *method, Qt::ConnectionType type = Qt::AutoConnection );


    /*!
     * Send Arguments to a Runned or loaded SApplication that
     * placed in app_tab_index in silicon tabbar or loadedInnerApps list.
     * This is onlye for silicons inner applications.
     * \sa loadedInnerApps
     */
    static void sendArgument( int app_tab_index , const QVariantList &args );


    /*!
     * Open "fileName" file with prefred sapplication.
     */
    static void openFile( const QString & fileName );


    /*!
     * Show a Message with silicon message center system.
     */
    static void message( Silicon::MessageType type , const QString & title , const QString & message , const QIcon & icon = QIcon() );


    /*!
     * Show Processes List in Silicon UI.
     */
    static void showProcessess();


    /*!
     * Returns list of all SApplications of Silicon.
     * \sa loadApp
     */
    static const QList<SAboutData> & appsList();


    /*!
     * Returns list of loaded Inner Apps in Silicon.
     * \sa sendArgument
     * \sa loadedWindowApp
     */
    static const QList<SAboutData> & loadedApps();


    /*!
     * Returns list of loaded Window Apps in Silicon.
     * \sa sendArgument
     * \sa loadedInnerApps
     */
    static const QList<SAboutData> & loadedWindowApp();


    /*!
     * Returns root of silicon config directory path.
     */
    static const QString & config();


    /*!
     * Returns global silicon temp directory address.
     */
    static const QString & temp();


    /*!
     * Find and return temp file address.
     */
    static QString requestTempFile();


    /*!
     * Returns Silicon MainWindow Pointer.
     */
    static QMainWindow *mainWindow();


    /*!
      * Returns pointer of the Main Panel widget.
      */
    static QWidget *mainPanel();


    /*!
     * Returns Silicon Process Page Pointer.
     */
    static SPage *processPage();


    /*!
     * Connect and Call method slot from receiver object when a new SApplication
     * loaded in the silicon.
     */
    static void applicationLoadedSignal( const QObject * receiver, const char * method, Qt::ConnectionType type = Qt::AutoConnection );

    /*!
     * Connect and Call method slot from receiver object when a new SApplication
     * going to load in the silicon.
     */
    static void applicationGoingToLoadSignal( const QObject * receiver, const char * method, Qt::ConnectionType type = Qt::AutoConnection );


    /*!
     * Write, Read and save configures in silicon main configure file that
     * usually hold in the : $Home/.config/silicon/config
     */
    static void writeConf( const QString & head , const QString & child , const QString & value );
    static QString readConf( const QString & head , const QString & child );
    static void saveAllConfs();


    /*!
     * Returns GPL or LGPL License texts.
     */
    static const QString & GPLLicense();
    static const QString & LesserGPLLicense();
};

#endif // SILICON_H
