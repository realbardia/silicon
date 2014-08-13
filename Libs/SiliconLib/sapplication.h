/*
 * File:      sapplication.h
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

#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QResizeEvent>

#include "saboutdata.h"
#include "spage.h"
#include "sprogresslistitem.h"
#include "smenupanel.h"
#include "silicon.h"
#include "libsilicon_global.h"

class QMessageBox;
class SProgressListItem;
class SApplicationPrivate;

/*! \class
 * The SApplication class provides a Silicon Application
 * class based on The QObject.
 */
class LIBSILICON_EXPORT SApplication : public QObject
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SApplication( const SAboutData & parent );
 *     ~SApplication();
 *
 *     const SAboutData & parent() const;
 *
 *     void addRuntimeArgs( const QStringList & args );
 *
 *     void addChild( SPage *page );
 *     const QList<SPage *> & childs() const;
 *
 *     int numberOfAllChilds() const;
 *     int numberOfAllSubObjects() const;
 *
 *     void addProgress( SProgressListItem *item );
 *     const QList<SProgressListItem *> & progresses() const;
 *
 *     void setQuitOnLastPageClosed( bool stt );
 *     bool quitOnLastPageClosed() const;
 *
 * PUBLIC SLOTS:
 *     bool quit();
 *     void focus();
 *
 * SIGNALS:
 *     void destroyed( SApplication *sapp );
 *     void runtimeArguments( const QVariantList & args );
 *
 * PROTECTED:
 *     virtual void runtimeArgsEvent( const QVariantList & args );
 *     virtual void closeEvent( QCloseEvent *event );
 *     virtual void FocusedEvent();
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SApplication )

public:
    /*!
     * Construct a SApplication with "parent" parent.
     */
    SApplication( const SAboutData & parent );
    ~SApplication();

    /*!
     * Returns Parents of the current page.
     */
    const SAboutData & parent() const;


    /*!
     * add "args" runtime arguments to the sapplication.
     * \sa runtimeArgsEvent
     */
    void addRuntimeArgs( const QVariantList &args );


    /*!
     * Add a child page to the SApplication.
     * \sa childs
     */
    void addChild( SPage *page );

    /*!
     * Returns list of the page childs.
     * \sa addChild
     */
    const QList<SPage *> & childs() const;

    /*!
     * Returns Number of all of the childs in the childs tree.
     */
    int numberOfAllChilds() const;
    int numberOfAllSubObjects() const;


    /*!
     * Add a Progress Item to the Application that showing
     * on the silicon processes page.
     * \sa progresses
     */
    void addProgress( SProgressListItem *item );

    /*!
     * Returns List of the SApplication Progresses.
     * \sa addProgress
     */
    const QList<SProgressListItem *> & progresses() const;


    /*!
     * If set true, application set quit when all of the page closed.
     * else application will still open.
     */
    void setQuitOnLastPageClosed( bool stt );
    bool quitOnLastPageClosed() const;

public slots:
    /*!
     * Quit SApplication.
     */
    bool quit();

    /*!
     * Set Focus on the SApplication. normaly when this functions is calling
     * silicon switch current tab to first child of the current application.
     * This function calling FocusedEvent() virtual function for
     * operations.
     */
    void focus();

signals:
    /*!
     * This Signal emitted when SApplication going to
     * destroy .
     */
    void destroyed( SApplication *sapp );

    /*!
     * This Signal emitted when Silicon send runtimeArgument to the
     * Application.
     * \sa runtimeArgsEvent
     */
    void runtimeArguments( const QVariantList & args );

protected:
    /*!
     * This Virtual Protected Function Call when Silicon
     * send some runtime arguments to the SApplication.
     * Runtime arguments is like the arguments that programs
     * give on start.
     * \sa addRuntimeArgs
     */
    virtual void runtimeArgsEvent( const QVariantList & args );

    /*!
     * This Event Calls when close() functions is calling or
     * user clicked on the close button.
     */
    virtual void closeEvent( QCloseEvent *event );

    /*!
     * This Event Calls when focus() function is calling.
     * \sa focus
     */
    virtual void FocusedEvent();

private slots:
    void pageDestroyed( SPage *page );
    void progressDestroyed( SProgressListItem *item );

private:
    void registerToSilicon();

private:
    SApplicationPrivate *p;
};

#endif // SAPPLICATION_H
