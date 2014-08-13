/*
 * File:      svariableconverter.h
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

#ifndef SVARIABLECONVERTER_H
#define SVARIABLECONVERTER_H

#include <QString>

#include "libsilicon_global.h"

/*! \class
 *  Convert some types to some other types
 */
class LIBSILICON_EXPORT SVariableConverter
{
/*! List of All Available functions
 *
 * STATIC PUBLIC FUNCTIONS :
 *     static QString intToString( int num );
 *     static QString doubleToString( double num );
 */

public:
    /*!
     * Convert an integer to QString
     */
    static QString intToString( int num );


    /*!
     * Convert an double to QString
     */
    static QString doubleToString( double num );
};

#endif // SVARIABLECONVERTER_H
