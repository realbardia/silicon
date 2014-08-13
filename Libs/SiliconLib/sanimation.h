/*
 * File:      sanimation.h
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

#ifndef SANIMATION_H
#define SANIMATION_H

#include <QObject>
#include <QSize>

class QWidget;
class SAnimationPrivate;

/*! \class
 * The SAnimation Class provides animation class
 * to use graphical effect for the widgets.
 */
class Q_DECL_EXPORT SAnimation : public QObject
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SAnimation(QObject *parent=0);
 *     ~SAnimation();
 *
 *     enum AnimationType { RESIZE , OFF };
 *     enum ChangedSizeType { MIN_MAX , SIZE };
 *     enum FixesOptions { HEIGHT_FIX , WIDTH_FIX , NO_FIX };
 *
 *     void start( QWidget *widget , QSize target
 *                 , AnimationType animType
 *                 , int duration
 *                 , ChangedSizeType changeSize = SAnimation::SIZE);
 *
 *     void setWidget(QWidget *widget);
 *     void finish();
 *     bool isActive() const;
 *
 *     void setFrameRate( int rate );
 *
 *     void setFixOptions( FixesOptions fix_option );
 *     FixesOptions fixOptions() const;
 *
 * SIGNALS:
 *     void finished();
 *     void finished(QWidget *widget);
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SAnimation )

public:
    /*!
     * Constructs The SAnimation class with "*parent" parent.
     */
    SAnimation(QObject *parent=0);
    ~SAnimation();

    /*!
     * Enums to use for animation types.
     * \sa start
     */
    enum AnimationType {
                            RESIZE      // Scale widget to target size.
                           ,OFF         // No Effect
                       };

    /*!
     * Enums to Tell to the sanimation how to resize widget.
     * \sa start
     */
    enum ChangedSizeType {
                            MIN_MAX     // Change Widget Size with setMinimum and
                                        // Maximum functions
                           ,SIZE        // Change Widget size with resize functions.
                         };

    /*!
     * Enums to set fixes options.
     */
    enum FixesOptions {
                            HEIGHT_FIX  // Don't Take effect on Height of widget.
                           ,WIDTH_FIX   // Don't Take effect on Width of widget.
                           ,NO_FIX      // Take effect on width and height of widget.
                      };


    /*!
     * Start Animation on "*widget" widget from current size to "target"
     * size with "duration" duration and animType and changeSize options.
     * \sa isActive
     * \sa isActive
     * \sa finished
     * \sa finish
     */
    void start( QWidget *widget , QSize target
                , AnimationType animType
                , int duration
                , ChangedSizeType changeSize = SAnimation::SIZE);


    /*!
     * Set Started Animation finish.
     * \sa start
     * \sa finished
     */
    void finish();

    /*!
     * Returns True if animation started and Returns
     * false if animation finished.
     * \sa start
     * \sa finish
     */
    bool isActive() const;


    /*!
     * Set Number of frames per seconds.
     */
    void setFrameRate( int rate );
    int frameRate() const;


    /*!
     * FixOption holds option that set the sanimation
     * resize width or height or width and height.
     */
    void setFixOptions( FixesOptions fix_option );
    FixesOptions fixOptions() const;

signals:
    /*!
     * This Signal emitted when started animation finished.
     * \sa start
     * \sa finish
     */
    void finished();
    void finished(QWidget *widget);

private slots:
    void resizeAnimation();

protected:
    void autoResize(QWidget *widget,const QSize & size);
    void manualResize(QWidget *widget,const QSize & size);
    void minmaxResize(QWidget *widget,const QSize & size);

private:
    SAnimationPrivate *p;
};

#endif // SANIMATION_H
