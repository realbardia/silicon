/*
 * File:      smenupanel.h
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

#ifndef SMENUPANEL_H
#define SMENUPANEL_H

#include <QObject>
#include <QAction>
#include <QMenu>

#include "libsilicon_global.h"

class SMenuPanelPrivate;

/*! \class
 * The SMenuPanel class provids a menu panel.
 * Menu Panel is a set of menus like menubar. It's showen on silicon settings menu.
 */
class LIBSILICON_EXPORT SMenuPanel : public QObject
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SMenuPanel( QObject *parent = 0);
 *     ~SMenuPanel();
 *
 *     void addAction( QAction *action );
 *     QAction *addMenu( QMenu *menu );
 *     QAction *addSeparator();
 *
 *     void insertAction( int i , QAction *action );
 *     QAction *insertMenu( int i , QMenu *menu );
 *     QAction *insertSeparator( int i );
 *
 *     QAction *at( int i ) const;
 *     int indexOf( QAction *action ) const;
 *
 *     int count() const;
 *     bool isEmpty() const;
 *
 *     void removeAt( int i );
 *     void remove( QAction *action );
 *
 *     void clear();
 *     void update();
 *
 *     QMenu *toQMenu();
 *
 *     bool isEnabled() const;
 *     bool isDisabled() const;
 *
 * PUBLIC SLOTS:
 *     void setEnabled( bool stt );
 *     void setDisabled( bool stt );
 *
 * SIGNALS:
 *     void updated();
 *     void updated( SMenuPanel *panel );
 *
 *     void enabled( bool stt );
 *     void disabled( bool stt );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SMenuPanel )

public:
    /*!
     * Constructs a Menu Panel with parent "parent".
     */
    SMenuPanel( QObject *parent = 0);
    ~SMenuPanel();


    /*!
     * Add "*action" action to menu panel.
     * \sa addMenu
     * \sa addSeparator
     * \sa insertAction
     * \sa remove
     */
    void addAction( QAction *action );

    /*!
     * Add "*menu" menu to menu panel.
     * \sa addAction
     * \sa addSeparator
     * \sa insertMenu
     */
    QAction *addMenu( QMenu *menu );

    /*!
     * Add a separator to menu panel.
     * \sa addMenu
     * \sa addAction
     * \sa insertSeparator
     */
    QAction *addSeparator();


    /*!
     * Insert "*action" action to menu panel at position i.
     * \sa insertMenu
     * \sa insertSeparator
     * \sa addAction
     * \sa removeAt
     */
    void insertAction( int i , QAction *action );

    /*!
     * Insert "*menu" menu to menu panel at position i.
     * \sa insertAction
     * \sa insert Separator
     * \sa addMenu
     */
    QAction *insertMenu( int i , QMenu *menu );

    /*!
     * Add a separator to menu panel at position i.
     * \sa insertMenu
     * \sa insertAction
     * \sa addSeparator
     */
    QAction *insertSeparator( int i );


    /*!
     * Returns Action Pointer at position i.
     * \sa count
     * \sa indexOf
     */
    QAction *at( int i ) const;

    /*!
     * Returns position of "*action" action.
     * \sa at
     */
    int indexOf( QAction *action ) const;

    /*!
     * Returns number of items (action ,menu and separator) added to menu panel.
     * \sa at
     */
    int count() const;

    /*!
     * Returns true if menu panel is empty, else retruen false.
     */
    bool isEmpty() const;


    /*!
     * Remove item (action ,menu or separator) at position i from menu panel.
     * \sa insertAction
     * \sa remove
     */
    void removeAt( int i );


    /*!
     * Remove "*action" action from menu panel.
     * \sa addAction
     * \sa removeAt
     */
    void remove( QAction *action );


    /*!
     * Set menu panel empty and remove all of item added to manu panel.
     * \sa remove
     * \sa removeAt
     */
    void clear();


    /*!
     * Send Updated Signal and then Silicon refresh or update panel.
     * \sa updated
     */
    void update();


    /*!
     * Returns a QMenu object that containts all of the SMenuPanel actions.
     */
    QMenu *toQMenu();


    /*!
     * Returns status of the menu (Disabled or Enabled).
     */
    bool isEnabled() const;
    bool isDisabled() const;

public slots:
    /*!
     * Sets status of the menu (Disabled or Enabled).
     */
    void setEnabled( bool stt );
    void setDisabled( bool stt );

signals:
    /*!
     * This Signal emitted when menu panel changed and need to
     * refresh on silicon ui.
     * "*panel" point to this.
     * \sa update
     */
    void updated();
    void updated( SMenuPanel *panel );


    /*!
     * This Signals emitted when menu panel set to enabled or
     * disabled.
     * \sa setEnable
     * \sa setDisable
     */
    void enabled( bool stt );
    void disabled( bool stt );

private:
    SMenuPanelPrivate *p;
};

#endif // SMENUPANEL_H
