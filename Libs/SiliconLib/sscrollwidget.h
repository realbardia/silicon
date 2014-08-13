/*
 * File:      sscrollwidget.h
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

#ifndef SSCROLLWIDGET_H
#define SSCROLLWIDGET_H

#include <QWidget>
#include <QSize>
#include <QString>

#include "libsilicon_global.h"

class SScrollWidgetPrivate;

/*! \class
 *  The SScrollWidget Class is QScrollArea like widget , But without any scrollbars
 *  SScrollWidget scroll with buttons .
 */
class LIBSILICON_EXPORT SScrollWidget : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SScrollWidget(QWidget *parent = 0);
 *     ~SScrollWidget();
 *
 *     void setWidget(QWidget *widget);
 *
 *     void setArrowsSize(QSize size);
 *     void setLeftArrowSize(QSize size);
 *     void setRightArrowSize(QSize size);
 *     void setTopArrowSize(QSize size);
 *     void setBottomArrowSize(QSize size);
 *
 *     void setHorizontalStep(int size);
 *     void setVerticalStep(int size);
 *     void setEnableHorizontalArrow(bool stt);
 *     void setEnableVerticalArrow(bool stt);
 *     void setStyleSheet(const QString & styleSheet);
 *
 *     QSize leftArrowSize() const;
 *     QSize rightArrowSize() const;
 *     QSize topArrowSize() const;
 *     QSize bottomArrowSize() const;
 *
 *     int horizontalStep() const;
 *     int verticalStep() const;
 *
 *     int horizontalValue() const;
 *     int verticalValue() const;
 *
 *     int maximumHorizontal() const;
 *     int maximumVertical() const;
 *
 *     void setHorizontalValue( int value );
 *     void setVerticalValue( int value );
 *
 * SIGNALS:
 *     void setHScroll(int value);
 *     void setVScroll(int value);
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SScrollWidget )

public:
    /*!
     * Constructs a scroll widget with parent.
     */
    SScrollWidget(QWidget *parent = 0);
    ~SScrollWidget();


    /*!
     * Sets the scroll area's widget.
     * The widget becomes a child of the scroll area, and will be destroyed when
     * the scroll area is deleted or when a new widget is set.
     */
    void setWidget(QWidget *widget);


    /*!
     * Set Arrows Buttons size . if you want set Speciality size use setLeftArrowSize ,
     * setRightArrowSize , setTopArrowSize or setBottomArrowSize.
     * \sa leftArrowSize
     * \sa rightArrowSize
     */
    void setArrowsSize(QSize size);
    void setLeftArrowSize(QSize size);
    void setRightArrowSize(QSize size);
    void setTopArrowSize(QSize size);
    void setBottomArrowSize(QSize size);


    /*!
     * Set Horizontal or Vertical step of scroll widget .
     */
    void setHorizontalStep(int size);
    void setVerticalStep(int size);


    /*!
     * Set Enable or Disable Horizontal or Vertical Scrolling
     */
    void setEnableHorizontalArrow(bool stt);
    void setEnableVerticalArrow(bool stt);


    /*!
     * Set style sheet of the scroll widget to styleSheet.
     */
    void setStyleSheet(const QString & styleSheet);


    /*!
     * Returns size of left , right , top or bottom Arrow Button in QSize class .
     */
    QSize leftArrowSize() const;
    QSize rightArrowSize() const;
    QSize topArrowSize() const;
    QSize bottomArrowSize() const;


    /*!
     * Returns step size of Horizontal and Vertical scrolling in the scroll widget .
     */
    int horizontalStep() const;
    int verticalStep() const;


    /*!
     * Returns current value of Horizontal or Vertical of the Scroll Widget.
     * \sa setHorizontalValue
     * \sa maximumVertical
     * \sa maximumHorizontal
     * \sa maximumVertical
     */
    int horizontalValue() const;
    int verticalValue() const;


    /*!
     * Returns Maximum Horizontal and Vertical value that the
     * SScrolWidget can take it.
     * \sa horizontalValue
     * \sa verticalValue
     */
    int maximumHorizontal() const;
    int maximumVertical() const;


    /*!
     * Set Current Horizontal or Vertical value of the Scroll Widget.
     * \sa horizontalValue
     * \sa maximumVertical
     */
    void setHorizontalValue( int value );
    void setVerticalValue( int value );

signals:
    /*!
     * This signal emitted when Horzintal scroll step chenged .
     */
    void setHScroll(int value);

    /*!
     * This signal emitted when Vertical scroll step chenged .
     */
    void setVScroll(int value);

private slots:
    void leftArrowClick();
    void rightArrowClick();
    void topArrowClick();
    void bottomArrowClick();

private:
    void setupOptions();
    SScrollWidgetPrivate *p;
};

#endif // SSCROLLWIDGET_H
