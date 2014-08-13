/*
 * File:      sinfowidget.h
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

#ifndef SINFOWIDGET_H
#define SINFOWIDGET_H

#include <QWidget>
#include <QObject>
#include <QSize>
#include <QPixmap>
#include <QString>

#include "libsilicon_global.h"

class SInfoWidgetPrivate;

/*! \class
 * The SInfoWidget class provides an Info Widget.
 * Info Widget Containts from Title,Picture and an
 * Information.
 */
class LIBSILICON_EXPORT SInfoWidget : public QWidget
{

/*! List of All Available Functions :
 *
 *     SInfoWidget(QWidget *parent = 0);
 *     ~SInfoWidget();
 *
 *     void setData(const QString & picAddress,const QString & title,const QString & description);
 *
 *     void setPicture(const QPixmap & pix);
 *     void setTitle(const QString & title);
 *     void setDescription(const QString & description);
 *     void clear();
 *
 *     void setMaximumPictureSize(const QSize & size);
 *     void setMinimumPictureSize(const QSize & size);
 *
 *     void setStyleSheet(const QString & style);
 *
 *     QPixmap picture() const;
 *     QString title() const;
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SInfoWidget )

public:
    /*!
     * Constructs an Info Widget with "*parent" parent.
     */
    SInfoWidget( QWidget *parent = 0 );
    ~SInfoWidget();


    /*!
     * Set Picture to photo in the picAddress , title to "title" string and
     * Description to "description" string.
     * \sa setPicture
     * \sa setTitle
     * \sa setDescription
     * \sa clear
     */
    void setData(const QString & picAddress,const QString & title,const QString & description);


    /*!
     * Set Picture of the Info Widget to pix.
     * \sa picture
     */
    void setPicture(const QPixmap & pix);


    /*!
     * Set Title of the Info Widget to title.
     * \sa title
     */
    void setTitle(const QString & title);


    /*!
     * Set Description of the Info Widget to desctiption.
     */
    void setDescription(const QString & description);


    /*!
     * Set Title , Picture and Description of the info widget empty.
     * \sa setData
     */
    void clear();


    /*!
     * Set Maximum size of the info widget's picture.
     * \sa setMaximumPictureSize
     */
    void setMaximumPictureSize(const QSize & size);


    /*!
     * Set Minimum size of the info widget's picture.
     * \sa setMinimumPictureSize
     */
    void setMinimumPictureSize(const QSize & size);


    /*!
     * Set Style sheet of the info widget.
     */
    void setStyleSheet(const QString & style);


    /*!
     * Returns picture of the info widget.
     * \sa setPicture
     */
    QPixmap picture() const;

    /*!
     * Returns title of the info widget.
     * \sa setTitle
     */
    QString title() const;

private:
    SInfoWidgetPrivate *p;
};

#endif // SINFOWIDGET_H
