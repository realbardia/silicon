/*
 * File:      sinnerprogressbar.h
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

#ifndef SINNERPROGRESSBAR_H
#define SINNERPROGRESSBAR_H

#include <QWidget>
#include <QPaintEvent>
#include <QColor>
#include <QString>

#include "libsilicon_global.h"

class SInnerProgressBarPrivate;

/*! \class
 * The SInnerProgressBar provides an inner progress bar.
 * Inner ProgressBar is a type of progress bar that used on
 * silicon applications . This make background darken and show a
 * progress bar and a progress indicator on widget.
 */
class LIBSILICON_EXPORT SInnerProgressBar : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SInnerProgressBar(QWidget *parent = 0);
 *     ~SInnerProgressBar();
 *
 *     void setWidth( int width );
 *     int width() const;
 *
 *     void setBarWidget( QWidget *widget );
 *     QWidget *barWidget() const;
 *
 *     void setColor( const QColor & color );
 *     QColor color() const;
 *
 * PUBLIC SLOTS:
 *     void setProgress( int percent );
 *     void start();
 *     void stop();
 *     void setText( const QString & str );
 *
 * SIGNALS:
 *     void valueChanged( int value );
 *     void textChanged( const QString & str );
 *     void started();
 *     void stopped();
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SInnerProgressBar )

public:
    /*!
     * Constructs an Inner Progress Bar with "*parent" parent.
     */
    SInnerProgressBar(QWidget *parent = 0);
    ~SInnerProgressBar();


    /*!
     * Set Width of ProgressBar and Text Label on the
     * Inner Progress Bar.
     * \sa width
     */
    void setWidth( int width );


    /*!
     * Returns Width of ProgressBar and Text Label on the
     * Inner Progress Bar.
     * \sa setWidth
     */
    int width() const;


    /*!
     * Nothing to do at the moment.
     * \sa barWidget
     */
    void setBarWidget( QWidget *widget );

    /*!
     * Nothing to do at the moment.
     * \sa setBarWidget
     */
    QWidget *barWidget() const;


    /*!
     * Set Background color of the Inner ProgressBar.
     * Text colors automaticaly set invert to "color" color.
     * \sa color
     */
    void setColor( const QColor & color );


    /*!
     * Returns Background color of the inner ProgressBar.
     */
    QColor color() const;

public slots:
    /*!
     * Set Value (Percent) of the Inner ProgressBar.
     * \sa valueChanged
     */
    void setProgress( int percent );


    /*!
     * Set Progress Indicator on the Inner ProgressBar started.
     * \sa stop
     * \sa started
     */
    void start();

    /*!
     * Set Progress Indicator on the Inner ProgressBar stopped.
     * \sa start
     * \sa stopped
     */
    void stop();


    /*!
     * Set Text of the label on the Inner ProgressBar to str.
     * \sa textChanged
     */
    void setText( const QString & str );

signals:
    /*!
     * This Signal emitted when value (percent) of the
     * ProgressBar on the Inner ProgressBar changed.
     * \sa setProgress
     */
    void valueChanged( int value );


    /*!
     * This Signal emitted when text of the Label
     * on the Inner ProgressBar changed.
     * \sa setText
     */
    void textChanged( const QString & str );


    /*!
     * This Signal emitted when the progress indicator on the
     * Inner Progressbar started.
     * \sa stopped
     * \sa start
     */
    void started();

    /*!
     * This Signal emitted when the progress indicator on the
     * Inner Progressbar stopped.
     * \sa started
     * \sa stop
     */
    void stopped();

protected:
    void paintEvent( QPaintEvent *event );

private:
    SInnerProgressBarPrivate *p;
};

#endif // SINNERPROGRESSBAR_H
