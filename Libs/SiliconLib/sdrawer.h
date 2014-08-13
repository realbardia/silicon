/*
 * File:      sdrawer.h
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

#ifndef SDRAWER_H
#define SDRAWER_H

#include <QScrollArea>
#include <QString>

#include "libsilicon_global.h"

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

class SDrawerPrivate;

/*! \class
 * The SDrawer class provides a picture viewr widget
 * that help you to make an picture viewer.
 * The SDrawer class features are zooming on pictures and
 * scrolling and ... .
 */
class LIBSILICON_EXPORT SDrawer : public QScrollArea
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *    SDrawer(QWidget *parent = 0);
 *    ~SDrawer();
 *
 *    int zoomPercent() const;
 *
 *    void setMaximumZoom( int percent );
 *
 *    int maximumZoom() const;
 *
 *    void setDisableScrollZoom( bool stt );
 *    void setEnableScrollZoom( bool stt );
 *
 *    bool disableScrollZoom() const;
 *    bool enableScrollZoom() const;
 *
 * PUBLIC SLOTS:
 *    void draw( const QString & file );
 *
 *    void setOriginalSize();
 *    void setFitSize();
 *    void zoom( int percent );
 *
 * SIGNALS:
 *    void zoomed( int percent );
 *    void fitSize( bool stt );
 *    void originalSize( bool stt );
 *    void fullscreenRequest();
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SDrawer )

public:
    /*!
     * Constructs an SDrawer widget with "*parent" parent.
     */
    SDrawer(QWidget *parent = 0);
    ~SDrawer();

    /*!
     * Returns percent of current zoom level on current
     * picture in the SDrawer widget.
     * \sa setMaximumZoom
     * \sa zoom
     */
    int zoomPercent() const;


    /*!
     * Set Maxiumum zoom level.
     * \sa maximumZoom
     */
    void setMaximumZoom( int percent );

    /*!
     * Returns maximum zoom level in the SDrawer.
     * \sa setMaximumZoom
     */
    int maximumZoom() const;

    /*!
     * Set Enable or Disable scrolling to zoom in the SDrawer
     * widget.
     * \sa disableScrollZoom
     * \sa enableScrollZoom
     */
    void setDisableScrollZoom( bool stt );
    void setEnableScrollZoom( bool stt );

    /*!
     * Returns current status of the scrolling feature.
     * \sa setDisableScrollZoom
     * \sa enableScrollZoom
     */
    bool disableScrollZoom() const;
    bool enableScrollZoom() const;

public slots:
    /*!
     * Draws "file" file in the SDrawer widget.
     */
    void draw( const QString & file );

    /*!
     * Set current zoom level to the Original size of the
     * drawed picture.
     * \sa setFitSize
     * \sa zoom
     */
    void setOriginalSize();

    /*!
     * Set current zoom level to the best fit zoom level.
     * \sa setOriginalSize
     * \sa zoom
     */
    void setFitSize();

    /*!
     * Sets zoom level to the percent value.
     * \sa setFitSize
     * \sa setOriginalSize
     */
    void zoom( int percent );

signals:
    /*!
     * This Signal emitted when current zoom level chenged
     * to the new value.
     * \sa zoom
     */
    void zoomed( int percent );

    /*!
     * this signals emitted when current zoom  level.
     * stt boolian os true when photos is in the best zoom level and fit
     * and Return false if is not.
     * \sa setFitSize
     */
    void fitSize( bool stt );

    /*!
     * this signals emitted when current zoom level.
     * stt boolian os true when photos is in the original zoom level
     * and Return false if is not.
     * \sa setFitSize
     */
    void originalSize( bool stt );

    /*!
     * This signal emitted when user double clicking on the picture.
     * Normaly its happend when user want to see picture in fullscreen
     * mode.
     */
    void fullscreenRequest();

protected:
    void wheelEvent( QWheelEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );

private:
    SDrawerPrivate *p;
};

#endif // SDRAWER_H
