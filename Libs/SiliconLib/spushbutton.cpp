/*
 * File:      spushbutton.cpp
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

#include "spushbutton.h"

#include <QTimer>



/*================================*
 |                                |
 |       SPushButtonPrivate       |
 |                                |
 *================================*/

class SPushButtonPrivate
{
public:
    QTimer *timer;

    int repeat_step;
    int repeat_delay;

    int step_number;
    bool click_at_end;
};



/*================================*
 |                                |
 |           SPushButton          |
 |                                |
 *================================*/

SPushButton::SPushButton(QWidget *parent) :
    QPushButton(parent)
{
    p = new SPushButtonPrivate;
    p->timer = new QTimer( this );

    QObject::connect( p->timer , SIGNAL(timeout()) , this , SLOT(anim_prev()) );
}

void SPushButton::repeatAnim( int delay , int step , bool click_at_end )
{
    if( p->timer->isActive() )
    {
        p->timer->stop();
        p->step_number = p->repeat_step;
        this->anim_prev();
    }

    p->repeat_delay = delay;
    p->repeat_step = step*2;
    p->step_number = 0;
    p->click_at_end = click_at_end;

    p->timer->start( p->repeat_delay );
}

void SPushButton::anim_prev()
{
    p->step_number++;
    if( p->step_number > p->repeat_step )
    {
        p->timer->stop();
        if( p->click_at_end )
            emit this->clicked();

        emit this->repeatAnimationFinished();

        return;
    }

    this->setDown( p->step_number % 2 );
}

SPushButton::~SPushButton()
{
    delete p;
}
