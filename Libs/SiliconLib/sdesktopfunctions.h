/*
 * File:      sdesktopfunctions.h
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

#ifndef SDESKTOPFUNCTIONS_H
#define SDESKTOPFUNCTIONS_H

#include <QString>
#include "libsilicon_global.h"

/*! \class
 * The SDesktopFunctions Provides some desktop functions
 * to use in programs.
 */
class LIBSILICON_EXPORT SDesktopFunctions
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     static void openFile( const QString & fileName );
 *     static void openDirectory( const QString & dirName );
 *     static QString binaryPath();
 *     static QString findEnvVariable( const QString & variable );
 *
 *     enum DesktopType{
 *         KDE,
 *         Gnome,
 *         Xfce,
 *         Ubuntu,
 *         Unknown
 *     };
 *
 *     static DesktopType desktopType();
 */

public:
    /*!
     * Open fileName file with default setted program
     * in the System.
     * \sa openDirectory
     */
    static void openFile( const QString & fileName );

    /*!
     * Open dirName directory with default file manager
     * in the system.
     \sa openFile
     */
    static void openDirectory( const QString & dirName );

    /*!
     * Returns path of silicon binary file places.
     */
    static QString binaryPath();

    /*!
     * Returns value of "variable" system Environment.
     */
    static QString findEnvVariable( const QString & variable );


#ifdef Q_OS_LINUX
    /*!
     * The Type of Linux Desktops.
     */
    enum DesktopType{
        KDE,
        Gnome,
        Xfce,
        Ubuntu,
        Unknown
    };

    /*!
     * Returns Desktop Type in the Linux.
     */
    static DesktopType desktopType();
#endif

};

#endif // SDESKTOPFUNCTIONS_H
