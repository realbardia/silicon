/*
 * File:      svariableconverter.cpp
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

#include "svariableconverter.h"
#include <cstdio>


/*================================*
 |                                |
 |       SVariableConverter       |
 |                                |
 *================================*/

QString SVariableConverter::intToString( int num )
{
    char chr[13];
    sprintf( chr , "%d" , num );
    return chr;
}

QString SVariableConverter::doubleToString( double num )
{
    char chr[13];
    sprintf( chr , "%.2f" , num );
    return chr;
}

