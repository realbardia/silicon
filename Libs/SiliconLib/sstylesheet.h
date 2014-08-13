/*
 * File:      sstylesheet.h
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

#ifndef SSTYLESHEET_H
#define SSTYLESHEET_H

#include <QString>

#include "libsilicon_global.h"

/*! \class
 * The SStyleSheet class provides a class that keep style sheet
 * and do some effects os style sheet.
 */
class LIBSILICON_EXPORT SStyleSheet
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SStyleSheet();
 *
 *     void setStyle( const QString & style );
 *     void addStyle( const QString & parent , const QString & style );
 *     void addStyle( const QString & style );
 *
 *     void enter( const QString & parent );
 *     void replace( const QString & from , const QString & to );
 *     void clear();
 *
 *     QString filter( const QString & parent ) const;
 *     QString style() const;
 */

public:
    /*!
     * Creates a new style sheet.
     */
    SStyleSheet();


    /*!
     * set Style Sheet of the class.
     * \sa addStyle
     */
    void setStyle( const QString & style );

    /*!
     * add style to end of Style Sheet of the class.
     * Use parent string is like to call enter function .
     * \sa setStyle
     * \sa enter
     */
    void addStyle( const QString & parent , const QString & style );
    void addStyle( const QString & style );


    /*!
     * enter to an item in stylesheet .
     * \example
     *      SStyleSheet style;
     *      style.setStyle( "STabBar:SiliTab{....}" );   // Style is : "STabBar:SiliTab{....}"
     *
     *      style.enter( "STabBar" );                    // Style is : "SiliTab{....}"
     *
     * \sa addStyle
     * \sa replace
     */
    void enter( const QString & parent );


    /*!
     * Replace "from" item to "to" item in stylesheet .
     * \example
     *      SStyleSheet style;
     *      style.setStyle( "STabBar:SiliTab{....}" );   // Style is : "STabBar:SiliTab{....}"
     *
     *      style.enter( "STabBar" , "MyTabBar" );       // Style is : "MyTabBar:SiliTab{....}"
     *
     * \sa enter
     */
    void replace( const QString & from , const QString & to );

    /*!
     * Set style sheet Empty . Its similar to call : setStyle( QString() );
     * \sa setStyle
     */
    void clear();


    /*!
     * Filter parent items in stylesheet and return style string .
     * \example
     *      SStyleSheet style;
     *
     *      QString style = "STabBar:SiliTab{....} SListWidget:list{....}"
     *      style.setStyle( style );
     *
     *      style.enter( "STabBar" );          // Style is : "STabBar:SiliTab{....}"
     *
     * \sa enter
     * \sa replace
     */
    QString filter( const QString & parent ) const;


    /*!
     * Returns style sheet in string .
     */
    QString style() const;

private:
    QString styleSheet;
};

#endif // SSTYLESHEET_H
