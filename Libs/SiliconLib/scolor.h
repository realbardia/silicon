/*
 * File:      scolor.h
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

#ifndef SCOLOR_H
#define SCOLOR_H

#include <QColor>

#include "libsilicon_global.h"

/*!
 * The SColor class provides a special QColor class.
 * It's Based on The QColor class.
 */
class LIBSILICON_EXPORT SColor : public QColor
{
public:
    SColor();
    SColor( const QColor & color );

    QColor operator+ (QColor colorB);
    QColor operator+ (int num);

    QColor operator- (int num);
    QColor operator* (double num);
    QColor operator/ (double num);

    QColor operator= (const QColor & colorB);
    QColor operator= (const SColor & colorB);

    void invert();
    void setColor(const QColor & colorB);
};

#endif // SCOLOR_H
