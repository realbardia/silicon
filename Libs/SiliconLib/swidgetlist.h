/*
 * File:      swidgetlist.h
 * Author:    Bardia Daneshvar (bardia@getsilicon.org)
 * Copyright: (C) 2011 Bardia Daneshvar
 * From Silicon Project
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

#ifndef SWIDGETLIST_H
#define SWIDGETLIST_H

#include "libsilicon_global.h"
#include <QScrollArea>

class SWidgetListItem;
class SWidgetListPrivate;

/*! \class
 * The SWidgetList class provides a type of list
 * that showing widget in the list like item list.
 */
class LIBSILICON_EXPORT SWidgetList : public QScrollArea
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SWidgetList(QWidget *parent = 0);
 *     ~SWidgetList();
 *
 *     void insertWidget( int i , QWidget *widget );
 *
 *     int indexOf( QWidget *widget ) const;
 *     QWidget *widget( int index ) const;
 *
 *     QWidget *current() const;
 *     int currentIndex() const;
 *
 *     int count() const;
 *     int isEmpty() const;
 *
 *     void setSpacing( int a );
 *     int spacing() const;
 *
 *     void setSelected( QList<QWidget*> list );
 *     QList<QWidget*> currentSelected() const;
 *
 * PUBLIC SLOTS:
 *     void addWidget( QWidget *widget );
 *
 *     void remove( QWidget *widget );
 *     void removeAt( int index );
 *
 *     void clear();
 *     void deselectAll();
 *
 *     void click( int index );
 *     void click( QWidget *widget );
 *
 *     void doubleClick( int index );
 *     void doubleClick( QWidget *widget );
 *
 * SIGNALS:
 *     void clicked( QWidget *widget );
 *     void clicked( int index );
 *     void doubleClicked( QWidget *widget );
 *
 *     void selected( QWidget *widget );
 *     void rowChanged( int row );
 *
 *     void moved( int from , int to );
 *
 * PROTECTED FUNCTIONS:
 *     void resizeEvent( QResizeEvent *event );
 *
 */

    Q_OBJECT
public:
    /*!
     * Constructs a SWidgetList Class with "*parent" parent.
     */
    SWidgetList(QWidget *parent = 0);
    ~SWidgetList();


    /*!
     * Insert a widget to position i in the SWidgetList.
     * \sa addWidget
     * \sa remove
     */
    void insertWidget( int i , QWidget *widget );


    /*!
     * Return Position of the widget in the SWidgetList.
     * \sa widget
     */
    int indexOf( QWidget *widget ) const;

    /*!
     * Return widget in the position index.
     * \sa indexOf
     * \sa count
     */
    QWidget *widget( int index ) const;

    /*!
     * Return current selected widget or current
     * selected widget index.
     */
    QWidget *current() const;
    int currentIndex() const;

    /*!
     * Return number of added widget in the SWidgetList.
     * \sa isEmpty
     */
    int count() const;

    /*!
     * Returns True if the SWidgetList is empty, otherwise
     * returns False.
     * \sa count
     * \sa clear
     */
    int isEmpty() const;


    /*!
     * Hold value of the space between of the items in
     * the SWidgetList.
     */
    void setSpacing( int a );
    int spacing() const;


    /*!
     * Set selected items to the list.
     * \sa selected
     * \sa deselectAll
     */
    void setSelected( QList<QWidget*> list );

    /*!
     * Returns current selected widgets.
     * \sa setSelected
     * \sa selected
     */
    QList<QWidget*> currentSelected() const;

public slots:
    /*!
     * Append "*widget" widget to the SWidgetList.
     * \sa insertWidget
     */
    void addWidget( QWidget *widget );

    /*!
     * Removes a widget from SWidgetList.
     * \sa addWidget
     * \sa removeAt
     * \sa clear
     */
    void remove( QWidget *widget );

    /*!
     * Removes widget at position index from SWidgetList.
     * \sa addWidget
     * \sa remove
     * \sa clear
     */
    void removeAt( int index );

    /*!
     * Clear the SWidgetList from widgets.
     * \sa remove
     * \sa removeAt
     * \sa isEmpty
     */
    void clear();

    /*!
     * Deselect all selected widgets from the SWidgetList.
     * \sa setSelected
     * \sa selected
     */
    void deselectAll();


    /*!
     * Click on an widget or widget at position index.
     * \sa clicked
     */
    void click( int index );
    void click( QWidget *widget );

    /*!
     * Double Click on an widget or widget at position index.
     * \sa clicked
     */
    void doubleClick( int index );
    void doubleClick( QWidget *widget );


signals:
    /*!
     * This Signal emit when user clicked on an
     * Item in the SWidgetList.
     * \sa doubleClicked
     */
    void clicked( QWidget *widget );
    void clicked( int index );

    /*!
     * This Signal emit when user double clicked on an
     * Item in the SWidgetList.
     * \sa clicked
     */
    void doubleClicked( QWidget *widget );

    /*!
     * This Signal emit when user select an
     * Item in the SWidgetList.
     * \sa setSelected
     * \sa deselectAll
     */
    void selected( QWidget *widget );

    /*!
     * This Signal emit when current row changed.
     * \sa selected
     */
    void rowChanged( int row );


    /*!
     * This Signal emit when an item moved to
     * another place in the SWidgetList.
     */
    void moved( int from , int to );

private slots:
    void itemClicked( SWidgetListItem *item );
    void itemSelected( SWidgetListItem *item );
    void itemDoubleClicked( SWidgetListItem *item );
    void itemMoved( SWidgetListItem *item );
    void movingFinished( SWidgetListItem *item );

protected:
    void resizeEvent( QResizeEvent *event );

private:
    SWidgetListPrivate *p;
};

#endif // SWIDGETLIST_H
