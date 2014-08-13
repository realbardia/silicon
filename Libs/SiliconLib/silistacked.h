/*
 * File:      silistacked.h
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

#ifndef SILISTACKED_H
#define SILISTACKED_H

#include <QWidget>
#include <QString>

#include "libsilicon_global.h"

class siliStackedPrivate;

/*! \class
 * The siliStacked class provides a stacked widget.
 */
class LIBSILICON_EXPORT siliStacked : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     siliStacked( QWidget *parent = 0 );
 *     ~siliStacked();
 *
 *     void addPage();
 *     void insertPage( int index );
 *     void removePage( int index );
 *
 *     void move( int from , int to );
 *
 *     QWidget *page( int index ) const;
 *     QWidget *currentPage() const;
 *     int currentIndex() const;
 *     int count() const;
 *
 *     void setCurrentWidget( QWidget *page );
 *
 * PUBLIC SLOTS:
 *     void setCurrentIndex( int index );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( siliStacked )

public:
    /*!
     * Constructs an stacked widget with "*parent" parent.
     */
    siliStacked( QWidget *parent = 0 );
    ~siliStacked();


    /*!
     * Add an Empty widget or page to stacked widget.
     * \sa insertPage
     * \sa removePage
     */
    void addPage();


    /*!
     * Insert an Empty widget or page to stacked widget at
     * position index.
     * \sa addPage
     * \sa removePage
     */
    void insertPage( int index );


    /*!
     * Remove widget or page position at index from
     * stacked widget.
     * \sa addPage
     * \sa insertPage
     */
    void removePage( int index );


    /*!
     * Move page or widget position at "from" to
     * position "to".
     * \sa insertPage
     */
    void move( int from , int to );


    /*!
     * Returns widget of the page position at index
     * in stacked widget.
     * \sa insertPage
     */
    QWidget *page( int index ) const;


    /*!
     * Return current selected and showen page on the stacked widget.
     * \sa setCurrentPage
     * \sa currentIndex
     */
    QWidget *currentPage() const;


    /*!
     * Returns position of the current selected and showen page
     * on the stacked widget.
     * \sa currentPage
     * \sa setCurrentIndex
     */
    int currentIndex() const;


    /*!
     * Returns number of page added to stacked widget.
     * \sa addPage
     * \sa insertPage
     */
    int count() const;


    /*!
     * Set Current selected and showen widget to "*page".
     * \sa setCurrentIndex
     * \sa currentWidget
     */
    void setCurrentWidget( QWidget *page );

public slots:
    /*!
     * Set Current selected and showen widget to widget
     * position at index in stacked widget.
     * \sa currentIndex
     * \sa setCurrentWidget
     * \sa count
     */
    void setCurrentIndex( int index );
    
private:
    siliStackedPrivate *p;
};

#endif // SILISTACKED_H
