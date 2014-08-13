/*
 * File:      scolorbutton.h
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

#ifndef SCOLORBUTTON_H
#define SCOLORBUTTON_H

#include <QWidget>
#include <QString>
#include "libsilicon_global.h"

class SColorButtonPrivate;

/*! \class
 * The SColorButton provides special push button for
 * selecting color.
 */
class LIBSILICON_EXPORT SColorButton : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SColorButton( const QString & name , QWidget *parent = 0 );
 *     ~SColorButton();
 *
 *     void setColor( const QColor & color );
 *     const QColor & color() const;
 *
 *     void setCheckable( bool stt );
 *     bool isCheckable() const;
 *
 *     bool isChecked() const;
 *
 * PUBLIC SLOTS:
 *     void colorDialog();
 *     void setChecked( bool stt );
 *
 * SIGNALS:
 *     void colorSelected( const QColor & color  );
 *     void colorSelected( const QString & color );
 *     void colorSelected( SColorButton *button );
 *
 *     void checked( bool stt );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SColorButton )

public:
    /*!
     * Construct SColorButton with "name" name and "*parent" parent.
     */
    SColorButton( const QString & name , QWidget *parent = 0 );
    ~SColorButton();


    /*!
     * Set Current color of the SColoButton.
     * this function also emitted colorSelected() signals.
     * \sa colorSelected
     * \sa color
     */
    void setColor( const QColor & color );

    /*!
     * Returns Current color of the SColorButton.
     * \sa setColor
     */
    const QColor & color() const;


    /*!
     * Set the SColorButton checkable.
     * This function showing a QCheckBox object in
     * front of the ColorButton.
     * \sa isCheckable
     * \sa setChecked
     */
    void setCheckable( bool stt );

    /*!
     * If the SColorButton is checkable returns true and
     * else returns false.
     * \sa setCheckable
     * \sa isChecked
     */
    bool isCheckable() const;


    /*!
     * If the SColorButton set checked returns true and
     * else returns false.
     * \sa isCheckable
     * \sa setChecked
     */
    bool isChecked() const;


public slots:
    /*!
     * Showing the select color dialog without clicking on the
     * color button.
     * \sa setColor
     */
    void colorDialog();

    /*!
     * Set Color Button checked or unchecked.
     * \sa isChecked
     * \sa setCheckable
     */
    void setChecked( bool stt );


signals:
    /*!
     * This signals emitted when current selected color
     * changed.
     * \sa setColor
     * \sa color
     * \sa colorDialog
     */
    void colorSelected( const QColor & color  );
    void colorSelected( const QString & color );
    void colorSelected( SColorButton *button );

    /*!
     * This Signal emitted when the SColorButton set to
     * checked or unchecked .
     * \sa setChecked
     */
    void checked( bool stt );

private:
    SColorButtonPrivate *p;
};

#endif // SCOLORBUTTON_H
