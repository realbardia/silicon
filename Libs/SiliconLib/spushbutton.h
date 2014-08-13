/*
 * File:      spushbutton.h
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

#ifndef SPUSHBUTTON_H
#define SPUSHBUTTON_H

#include <QPushButton>

#include "libsilicon_global.h"

class SPushButtonPrivate;

/*! \class
 * The SPushButton class provides push button based on
 * the QPushButton class.
 */
class LIBSILICON_EXPORT SPushButton : public QPushButton
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SPushButton( QWidget *parent = 0 );
 *     ~SPushButton();
 *
 *     void repeatAnim( int delay , int step , bool click_at_end = false );
 *
 * signals:
 *     void repeatAnimationFinished();
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SPushButton )

public:
    /*!
     * Constructs a SPushButton class with "*parent" parent.
     */
    SPushButton( QWidget *parent = 0 );
    ~SPushButton();


    /*!
     * It's an Animation Function.
     * Set Downed Button (like click but without clicked and toggled signals) in
     * "step" step and every step with "delay" delay.
     * If click_at_end is true , a clicked signal will be emitted at end of animation.
     * \sa repeatAnimationFinished
     */
    void repeatAnim( int delay , int step , bool click_at_end = false );

signals:
    /*!
     * This Signal emitted when Repeat Animation finished.
     * \sa repeatAnim
     */
    void repeatAnimationFinished();

private slots:
    void anim_prev();

private:
    SPushButtonPrivate *p;
};

#endif // SPUSHBUTTON_H
