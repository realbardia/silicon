/*
 * File:      ssliderwidget.h
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

#ifndef SSLIDERWIDGET_H
#define SSLIDERWIDGET_H

#include <QWidget>
#include <QObject>

#include "libsilicon_global.h"

class QResizeEvent;
class SSliderWidgetPrivate;

/*! \class
 *  The SSliderWidget class provides a slider widget .
 *  SliderWidget is like tabbars , but show all widget in same time in one page
 *  and side by side .
 *  But in one time one widget is active or Enable and other widgets is disable .
 */
class LIBSILICON_EXPORT SSliderWidget : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SSliderWidget(QWidget *parent = 0);
 *     ~SSliderWidget();
 *
 *     void addWidget( QWidget *widget , int width = -1 );
 *     void insertWidget( QWidget *widget , int index , int width = -1 );
 *     void removeWidget( QWidget *widget );
 *     void removeAt( int index );
 *
 *     void setFocus( int index );
 *     void setFocus( QWidget *widget );
 *
 *     QWidget *widgetAt( int index ) const;
 *     QWidget *focusedWidget() const;
 *     int focusedIndex() const;
 *
 *     int count() const;
 *
 * PUBLIC SLOTS:
 *     void next();
 *     void previous();
 *
 *     void setShowingAll( bool stt );
 */

    Q_OBJECT
    Q_DISABLE_COPY( SSliderWidget )

public:
    /*!
     *  Creates a new slider widget with the given parent.
     */
    SSliderWidget(QWidget *parent = 0);
    ~SSliderWidget();


    /*!
     * Add a widget to slider widget .
     * If you set width integer , your widget width in side by side view set to width .
     * \sa insertWidget
     * \sa removeWidget
     */
    void addWidget( QWidget *widget , int width = -1 );


    /*!
     * Insert a widget to slider widget at index position.
     * If you set width integer , your widget width in side by side view set to width .
     * \sa addWidget
     * \sa removeWidget
     */
    void insertWidget( QWidget *widget , int index , int width = -1 );


    /*!
     * Remove a widget from slider widget.
     * You can remove widget with index or pointer of widget .
     * \sa addWidget
     * \sa insertWidget
     */
    void removeWidget( QWidget *widget );
    void removeAt( int index );


    /*!
     * Set current available widget to widget or to index position .
     * When Slider Widget focus on a widget , other widgets set to disable (unavailable).
     */
    void setFocus( QWidget *widget );
    void setFocus( int index );


    /*!
     * Return widget of slider widget at index position .
     */
    QWidget *widgetAt( int index ) const;

    /*!
     * Return pointer of focused widget in slider widget.
     */
    QWidget *focusedWidget() const;

    /*!
     * Return index of focused widget in slider widget.
     */
    int focusedIndex() const;


    /*!
     * Returns the number of added widget in the slider widget.
     */
    int count() const;


    /*!
     * Returns status of the focues feature in the Slider Widget.
     * If its enable return true and else return false.
     * \sa setShowingAll
     */
    bool isShowingAll() const;

public slots:
    /*!
     * Focus to the next widget ,
     * If there is no next widget ,function dont do any thing.
     */
    void next();

    /*!
     * Focus to the previous widget ,
     * If there is no previous widget ,function dont do any thing.
     */
    void previous();


    /*!
     * Show All Widget in the Slider Widget and disable focuse
     * feature .
     * \sa isShowingAll
     */
    void setShowingAll( bool stt );

protected:
    void resizeEvent( QResizeEvent *event );

private:
    SSliderWidgetPrivate *p;
};

#endif // SSLIDERWIDGET_H
