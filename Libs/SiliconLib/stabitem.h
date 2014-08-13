/*
 * File:      stabitem.h
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

#ifndef STABITEM_H
#define STABITEM_H

#include <QPushButton>
#include <QIcon>
#include <QObject>
#include <QString>

class STabItemPrivate;

#include "libsilicon_global.h"

/*! \class
 *  The STabItem class provides an item for use with the SiliTab TabBar
 */
class LIBSILICON_EXPORT STabItem : public QPushButton
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     STabItem( QWidget *parent = 0 );
 *     STabItem( const QString & name , QWidget *parent = 0 );
 *     STabItem( const QIcon & icon , const QString & name , QWidget *parent = 0 );
 *
 *     QPushButton *closeButton const;
 *
 * SIGNALS:
 *     void closeRequest( STabItem *item );
 *     void clicked( STabItem *item );
 *     void entered( STabItem *item );
 *     void leaved( STabItem *item );
 *
 * PRIVATE SLOTS:
 *     void sendCloseRequestSignal();
 *     void dragTimeFinished();
 *     void stopTimer();
 *
 * PROTECTED FUNCTIONS:
 *     void dragEnterEvent( QDragEnterEvent *event );
 *     void dragLeaveEvent( QDragLeaveEvent *event );
 *     void dragMoveEvent( QDragMoveEvent *event );
 *     void dropEvent( QDropEvent *event );
 *     void enterEvent( QEvent *event );
 *     void leaveEvent( QEvent *event );
 */

    Q_OBJECT
    Q_DISABLE_COPY( STabItem )

public:
    /*!
     *  Constructs an item and Set Icon , name or title and parent of tab Item
     */
    STabItem( QWidget *parent = 0  );
    STabItem( const QString & name , QWidget *parent = 0  );
    STabItem( const QIcon & icon , const QString & name , QWidget *parent = 0 );
    ~STabItem();


    /*!
     *  return Close Button Object that is on the Item , Its a QPushButton
     *  \sa closeRequest
     */
    QPushButton *closeButton() const;

signals:
    /*!
     *   When user click on close button on tabItem , class emit closeRequest signal
     */
    void closeRequest( STabItem *item );

    /*!
     * This Signal emitted  when clicking on the tab item.
     */
    void clicked( STabItem *item );

    /*!
     * This Signal emitted when mouse pointer enter the item.
     * \sa leaved
     */
    void entered( STabItem *item );

    /*!
     * This Signal emitted when mouse pointer leave the item.
     * \sa entered
     */
    void leaved( STabItem *item );

private slots:
    void sendClickSignal();
    void sendCloseRequestSignal();
    void dragTimeFinished();
    void stopTimer();

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dragLeaveEvent( QDragLeaveEvent *event );
    void dragMoveEvent( QDragMoveEvent *event );
    void dropEvent( QDropEvent *event );
    void enterEvent( QEvent *event );
    void leaveEvent( QEvent *event );

private:
    /*!
     *   setup class
     */
    void setup( QWidget *parent );

    STabItemPrivate *p;
};

#endif // STABITEM_H
