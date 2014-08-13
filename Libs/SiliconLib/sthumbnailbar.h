/*
 * File:      sthumbnailbar.h
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

#ifndef STHUMBNAILBAR_H
#define STHUMBNAILBAR_H

#include <QWidget>
#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QSize>

#include "libsilicon_global.h"

class SThumbnailBarPrivate;
class SPictureWidget;

/*! \class
 * SThumbnailBar class provides a thumbnailbar that show
 * small preview of number of pictures.
 */
class LIBSILICON_EXPORT SThumbnailBar : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *    SThumbnailBar( QWidget *parent = 0 );
 *    ~SThumbnailBar();
 *
 *    void addPath( const QString & path );
 *    void addPathList( const QStringList & list );
 *    void setPathList( const QStringList & list );
 *    void addFile( const QString & file );
 *
 *    void removeAt( int index );
 *    void removeOne( const QString & file );
 *
 *    void setCurrentIndex( const QString & path );
 *    void setCurrentIndex( int i );
 *
 *    int currentIndex() const;
 *    QString currentItem() const;
 *
 *    int count() const;
 *
 *    QRect currentItemRect();
 *
 *    void setThumbnailSize( const QSize & size );
 *    const QSize & thumbnailSize() const;
 *
 * PUBLIC SLOTS:
 *    void next();
 *    void previous();
 *
 *    void refresh();
 *    void clear();
 *
 * SIGNALS:
 *    void currentItemChanged( const QPixmap & pixmap );
 *    void currentItemChanged( const QString & string );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SThumbnailBar )

public:
    /*!
     * Constructs a SThumbnailBar widget with "*parent" parent.
     */
    SThumbnailBar( QWidget *parent = 0 );
    ~SThumbnailBar();


    /*!
     * Adds pictures in "path" path (a direcotry or folder)
     * to the thumbnail bar.
     * \sa addPathList
     * \sa setPathList
     * \sa addFile
     */
    void addPath( const QString & path );


    /*!
     * Adds pictures in number of paths (direcotries or folders)
     * to the thumbnail bar.
     * \sa addPath
     * \sa setPathList
     * \sa addFile
     */
    void addPathList( const QStringList & list );

    /*!
     * Clears Thumbnail bar and Adds pictures in number of
     * paths (direcotries or folders) to the thumbnail bar.
     * \sa addPath
     * \sa addPathList
     * \sa addFile
     */
    void setPathList( const QStringList & list );


    /*!
     * Adds and Preview "file" file to the thumbnail bar.
     * \sa addPath
     * \sa addPathList
     * \sa setPathList
     */
    void addFile( const QString & file );


    /*!
     * Removes Item placed in the index position or with "file" path
     * from the thumbnail bar.
     * \sa addFile
     * \sa addPath
     */
    void removeAt( int index );
    void removeOne( const QString & file );


    /*!
     * Sets current index to "path" path or to the position
     * i in the thumbnail bar.
     * \sa currentIndex
     * \sa count
     */
    void setCurrentIndex( const QString & path );
    void setCurrentIndex( int i );


    /*!
     * Returns position of the current selected item in
     * the thumbnail bar.
     * \sa setCurrentIndex
     */
    int currentIndex() const;


    /*!
     * Returns current selected item file path.
     * \sa setCurrentIndex
     * \sa currentIndex
     */
    QString currentItem() const;


    /*!
     * Returns number of items added to the thumbnailbar.
     * \sa setCurrentIndex
     */
    int count() const;


    /*!
     * Returns drawed rect of the current selected item in
     * the thumbnail bar.
     * \sa setCurrentIndex
     */
    QRect currentItemRect();


    /*!
     * Sets size of the every added item in the thumbnail bar.
     */
    void setThumbnailSize( const QSize & size );
    const QSize & thumbnailSize() const;

public slots:
    /*!
     * Set current selected item to the next item
     * in the thumbnail bar.
     * \sa setCurrentIndex
     * \sa previous
     */
    void next();

    /*!
     * Set current selected item to the previous item
     * in the thumbnail bar.
     * \sa setCurrentIndex
     * \sa next
     */
    void previous();


    /*!
     * Refresh list of the thumbnail bar and redrawed
     * all of them.
     */
    void refresh();


    /*!
     * Clear all of the item added to the thumbnail bar.
     * \sa addPath
     * \sa addFile
     */
    void clear();

signals:
    /*!
     * This signals emitted when current selected item chenged in
     * the thumbnail bar.
     * \sa setCurrentIndex
     */
    void currentItemChanged( const QPixmap & pixmap );
    void currentItemChanged( const QString & string );

protected:
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );

private slots:
    void item_clicked( SPictureWidget *picture_widget );
    void removeProcess();

private:
    bool addProcess();
    bool check_path_added( const QString & path );
    bool check_file_added( const QString & file );
    void refreshBase( const QStringList & list );

private:
    SThumbnailBarPrivate *p;
};

#endif // STHUMBNAILBAR_H
