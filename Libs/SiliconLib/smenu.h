/*
 * File:      smenu.h
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

#ifndef SMENU_H
#define SMENU_H

#include <QWidget>
#include <QList>
#include <QFrame>

#include "libsilicon_global.h"
#include "sanimation.h"

class SMenuPrivate;

/*! \class
 * The SMenu class privodes a silicon master menu like class.
 */
class LIBSILICON_EXPORT SMenu : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SMenu(QWidget *parent = 0);
 *     ~SMenu();
 *
 *     void snapTo(QWidget *widget,int top,int left,int right,int bottom);
 *     void setAnimation(bool stt);
 *     void setMainWidget(QWidget *widget);
 *     void setStyleSheet(const QString & style);
 *
 *     QWidget *widget() const;
 *     QString styleSheet() const;
 *     bool animationState() const;
 *
 *     bool menuIsHidden() const;
 *
 * PUBLIC SLOTS:
 *     void autoShowHide();
 *     void showMenu();
 *     void hideMenu();
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SMenu )

public:
    /*!
     * Constructs a menu with "*parent" parent.
     */
    SMenu(QWidget *parent = 0);
    ~SMenu();


    /*!
     * Snap to widget with top , left , right and bottom padding from sides
     * of widget.
     */
    void snapTo(QWidget *widget,int top,int left,int right,int bottom);


    /*!
     * Set Animation on/off state to stt.
     * This is only work with showMenu and HideMenu functions
     * and dont work with show() and hide().
     * \sa showMenu
     * \sa hideMenu
     * \sa autoShowHide
     * \sa animationState
     */
    void setAnimation(bool stt);


    /*!
     * Set widget that you want to show on menu.
     * \sa widget
     */
    void setMainWidget(QWidget *widget);


    /*!
     * Set style sheet of the menu.
     * \sa styleSheet
     */
    void setStyleSheet(const QString & style);


    /*!
     * Returns Main Widget [that show on menu] pointer.
     * \sa setMainWidget
     */
    QWidget *widget() const;


    /*!
     * Returns Style Sheet of the menu.
     * \sa setStyleSheet
     */
    QString styleSheet() const;


    /*!
     * Returns animation on/off state.
     * \sa setAnimation
     */
    bool animationState() const;


    /*!
     * Returns true if the widget is hidden, otherwise returns false.
     */
    bool menuIsHidden() const;

public slots:
    /*!
     * If Menu is showen set it to hidden and else
     * set it to Showen.
     * This Functions work with animation if animation state is Enable.
     * \sa setAnimation
     * \sa showMenu
     * \sa hideMenu
     */
    void autoShowHide();


    /*!
     * Show Menu if its hidden.
     * This Functions work with animation if animation state is Enable.
     * \sa setAnimation
     * \sa autoShowHide
     * \sa hideMenu
     */
    void showMenu();


    /*!
     * Hide Menu if its showen.
     * This Functions work with animation if animation state is Enable.
     * \sa setAnimation
     * \sa autoShowHide
     * \sa showMenu
     */
    void hideMenu();

private slots:
    void snapCheck();
    void finishAnim();

protected:
    void setDefaults();
    void setupFrames();

private:
    SMenuPrivate *p;
};

#endif // SMENU_H
