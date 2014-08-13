/*
 * File:      scontextbutton.h
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

#ifndef SCONTEXTBUTTON_H
#define SCONTEXTBUTTON_H

#include <QPushButton>
#include <QMenu>
#include <QString>
#include <QAction>

#include "smenupanel.h"
#include "libsilicon_global.h"

class SContextButtonPrivate;

/*! \class
 * The SContextButton provides a Context Button that
 * when user clicked on button show a menu.
 */
class LIBSILICON_EXPORT SContextButton : public QPushButton
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SContextButton(QWidget *parent = 0);
 *     ~SContextButton();
 *
 *     void addItem(QAction *action);
 *     void addSeparator();
 *     void addMenu(QMenu *menu);
 *     void addMenuPanel( SMenuPanel *menu );
 *     void removeItem(QAction *action);
 *
 *     void clear();
 *
 *     void setColorizedMenu( bool stt );
 *     bool colorizedMenu() const;
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SContextButton )

public:
    /*!
     * Construct a Context Button with "*parent" parent.
     */
    SContextButton(QWidget *parent = 0);
    ~SContextButton();


    /*!
     * Add "*action" action to the Context Button.
     * \sa addMenu
     * \sa removeItem
     */
    void addItem(QAction *action);


    /*!
     * Add a Separator to the Context Button.
     * \sa addAction
     * \sa removeItem
     */
    void addSeparator();


    /*!
     * Add "*menu" menu to the Context Button.
     * \sa addItem
     * \sa removeItem
     */
    void addMenu(QMenu *menu);


    /*!
     * Adding "*menu" Menu Panel Items to the
     * Context Button.
     * \sa addMenu
     * \sa addItem
     */
    void addMenuPanel( SMenuPanel *menu );


    /*!
     * Remove "*action" action from the Context Button.
     * \sa addItem
     * \sa clear
     */
    void removeItem(QAction *action);


    /*!
     * Set the Context Button Emtpy and Clear all of the items
     * added to the Context Button.
     * \sa removeItem
     */
    void clear();


    /*!
     * set Menu Colorized to Highlight color.
     */
    void setColorizedMenu( bool stt );
    bool colorizedMenu() const;

protected:
    void paintEvent( QPaintEvent *event );

private slots:
    void showMenu();

private:
    SContextButtonPrivate *p;
};

#endif // SCONTEXTBUTTON_H
