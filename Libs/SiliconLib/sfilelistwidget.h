/*
 * File:      sfilelistwidget.h
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

#ifndef SFILELISTWIDGET_H
#define SFILELISTWIDGET_H

#include <QListWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QString>
#include <QStringList>
#include <QIcon>

#include "sfoldergraph.h"

#include "libsilicon_global.h"

class SFileListWidgetPrivate;

/*! \class
 * The SFileListWidget class provides a Graphical Folder Graph.
 * In fact It's user Interface of SFolderGraph class.
 * The SFileListWidget Based on QListWidget.
 * \sa SFolderGraph
 */
class LIBSILICON_EXPORT SFileListWidget : public QListWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SFileListWidget(QWidget *parent = 0);
 *     ~SFileListWidget();
 *
 *     void setFoldersIcon( const QIcon & icon );
 *     void setFilesIcon( const QIcon & icon );
 *
 *     void addFile( const QString & file );
 *     void addFiles( const QStringList & files );
 *     void addFolder( const QString & folder );
 *     void newFolder( const QString & name );
 *     void rename( const QString & currentName , const QString & newName );
 *
 *     void setFolderGraph( SFolderGraph *folderGraph );
 *
 *     bool isEmpty() const;
 *     bool isBusy() const;
 *
 *     QStringList gaugingRoot() const;
 *     QStringList gaugingCurrent() const;
 *
 * PUBLIC SLOTS:
 *     void removeCurrent();
 *     void reload();
 *     void folderDoubleClicked( QListWidgetItem *item );
 *     void cdUp();
 *     void cd( const QString & address );
 *
 * SIGNALS:
 *     void addressChanged( const QString & str );
 *     void reloaded();
 *     void busy( bool stt );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SFileListWidget )

public:
    /*!
     * Constructs a File List Widget class with "*parent" parent.
     */
    SFileListWidget(QWidget *parent = 0);
    ~SFileListWidget();


    /*!
     * Set The FileListWidget Folders Icon to "icon".
     */
    void setFoldersIcon( const QIcon & icon );

    /*!
     * Set The FileListWidget Files Icon to "icon".
     */
    void setFilesIcon( const QIcon & icon );


    /*!
     * Add file or files to the FileListWidget.
     * \addFolder
     */
    void addFile( const QString & file );
    void addFiles( const QStringList & files );


    /*!
     * Add a folder to the FileListWidget.
     * \sa addFile
     * \sa newFolder
     */
    void addFolder( const QString & folder );


    /*!
     * Create New folder with "name" name to the FileListWidget.
     * \sa addFile
     * \sa addFolder
     */
    void newFolder( const QString & name );


    /*!
     * Rename file or folder "currentName" to "newName" name.
     * \sa addFile
     * \sa addFolder
     */
    void rename( const QString & currentName , const QString & newName );


    /*!
     * Set a Special Folder Graph to FileListWidget for
     * using on the core of FileListWidget.
     */
    void setFolderGraph( SFolderGraph *folderGraph );


    /*!
     * Returns True if FileListWidget is Empty and
     * Returns False if FileListWidget is not Empty.
     * \sa removeCurrent
     */
    bool isEmpty() const;


    /*!
     * Returns True if the FileListWidget is busy and Returns
     * false if is not Busy.
     * \sa busy
     */
    bool isBusy() const;


    /*!
     * Gauaging Recersevly Root Directory or Current Directory
     * and Returns result in pathSpec type.
     * \sa SFolderGraph::gaugingRoot()
     * \sa SFolderGraph::gaugingCurrent()
     */
    QStringList gaugingRoot() const;
    QStringList gaugingCurrent() const;

public slots:
    /*!
     * Removes Current Selected Directories and Files from
     * ListWidget of the SFileListWidget class.
     * \sa addFile
     */
    void removeCurrent();


    /*!
     * Reload or Refresh current Directory of FileListWidget.
     * Usualy Its use when you want to change the FolderGraph manualy.
     * \sa setFolderGraph
     * \sa reloaded
     */
    void reload();


    /*!
     * It's use when user double clicked on an folder.
     * \sa SFolderGraph::cd()
     */
    void folderDoubleClicked( QListWidgetItem *item );


    /*!
     * cd or Change Directory to the parent directory of FileListWidget.
     * Parent directory of the root directory is root directory.
     * \sa cd
     */
    void cdUp();


    /*!
     * Set Current directory to "address" address.
     * \sa cdUp
     */
    void cd( const QString & address );

signals:
    /*!
     * This Signal emitted when current FileListWidget or FolderGraph
     * changed to another directory.
     * "str" QString is path of the changed directory.
     */
    void addressChanged( const QString & str );


    /*!
     * This Signal emitted when FileListWidget reloaded or refreshed.
     * \sa reload
     */
    void reloaded();


    /*!
     * This Signal emitted when the FileListWidget class is busy and
     * any change to class is dangres.
     * \sa isBusy
     */
    void busy( bool stt );

protected:
    virtual void dropEvent( QDropEvent *event );
    virtual void dragEnterEvent( QDragEnterEvent *event );
    virtual void dragMoveEvent( QDragMoveEvent *event );
    virtual void resizeEvent( QResizeEvent *event );

private:
    SFileListWidgetPrivate *p;
};

#endif // SFILELISTWIDGET_H
