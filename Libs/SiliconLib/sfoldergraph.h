/*
 * File:      sfoldergraph.h
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

#ifndef SFOLDERGRAPH_H
#define SFOLDERGRAPH_H

#include <QFile>
#include <QString>
#include <QDir>
#include <QList>
#include <QMap>

#include "libsilicon_global.h"

struct SFolder;
struct SFile;
class SFolderGraphPrivate;

/*! \class
 * The SFolderGraph class provides an Folder Modeling graph
 * or tree class.
 */
class LIBSILICON_EXPORT SFolderGraph
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SFolderGraph();
 *     ~SFolderGraph();
 *
 *     void addFile( QFile *file );
 *     void addFiles( const QList<QFile *> & list );
 *     void addFile( const QString & file );
 *     void addFiles( const QStringList & list );
 *
 *     void addFolder( const QDir & dirName );
 *     void addFolders( const QList<QDir> & list );
 *
 *     void newFolder( const QString & name );
 *
 *     void removeFile( const QString & fileName );
 *     void removeFolder( const QString & fileName );
 *
 *     void rename( const QString & currentName , const QString & newName );
 *
 *     void cd( QString dirName );
 *     void up();
 *     void cdRoot();
 *
 *     QStringList currentFolders() const;
 *     QStringList currentFiles() const;
 *     QString fileAddress( const QString & fileName ) const;
 *
 *     QString currentPath() const;
 *     QStringList gaugingCurrent() const;
 *     QStringList gaugingRoot() const;
 *
 *     bool isEmpty() const;
 *     bool currentIsEmpty() const;
 *
 *     bool isBusy() const;
 *
 *     int filesCount() const;
 *     int foldersCount() const;
 *     int count() const;
 *
 *     int rootFilesCount() const;
 *     int rootFoldersCount() const;
 *     int rootCount() const;
 *
 *     void clear();
 *
 */

public:
    /*!
     * Constructs a Folder Graph class.
     */
    SFolderGraph();
    ~SFolderGraph();


    /*!
     * Adding a File or Number of Files from system to Folder Graph
     * at Current Folder or Directory.
     * \sa addFolder
     * \sa removeFile
     * \cd
     */
    void addFile( QFile *file );
    void addFiles( const QList<QFile *> & list );
    void addFile( const QString & file );
    void addFiles( const QStringList & list );


    /*!
     * Adding a Folder or Number of Folders from system to Folder Graph
     * at Current Folder or Directory.
     * addFolder() Function scan recursively folders and directories from
     * files and other folders and also added them to folder graph.
     * \sa addFile
     * \sa newFolder
     * \sa removeFolder
     * \cd
     */
    void addFolder( const QDir & dirName );
    void addFolders( const QList<QDir> & list );


    /*!
     * Adding an Empty Folder with "name" name at current Directory.
     * \sa addFolder
     * \sa removeFolder
     * \cd
     */
    void newFolder( const QString & name );


    /*!
     * Removes File or Folder with "name" names from current directory
     * from the Folder Graph.
     * \sa addFile
     * \sa addFolder
     * \sa newFolder
     * \cd
     */
    void removeFile( const QString & name );
    void removeFolder( const QString & name );


    /*!
     * Rename name of "currentName" file or directory to "newName".
     * \sa cd
     */
    void rename( const QString & currentName , const QString & newName );


    /*!
     * cd or Change Directory to the dirName directory or folder.
     * dirName must be containts of the current directory.
     * \sa up
     * \sa cdRoot
     * \sa currentPath
     */
    void cd( QString dirName );


    /*!
     * Change Directory or Folder to parent Directory of the
     * current Directory.
     * \sa cd
     * \sa cdRoot
     */
    void up();


    /*!
     * Change Directory to the root directory. parent of root directory
     * is root directory.
     * \sa cd
     * \sa up
     */
    void cdRoot();


    /*!
     * Returns name list of all folders or directories in the current directory.
     * \sa currentFiles
     */
    QStringList currentFolders() const;

    /*!
     * Returns name list of all files in the current directory.
     * \sa currentFolders
     */
    QStringList currentFiles() const;


    /*!
     * Returns real path of fileName file in the
     * system (not in the folder graph)
     * \sa currentPath
     */
    QString fileAddress( const QString & fileName ) const;


    /*!
     * Return path of current directory in folder graph.
     * \sa cd
     */
    QString currentPath() const;


    /*!
     * Gauging recursively current or root directory.
     * output type :
     * [path of file in folder graph]=[path of file in the system]
     * \sa mapOfCurrent
     * \sa mapOfRoot
     * \example
     * \abcd\text.txt=\home\brdn\myFile.txt
     */
    QStringList gaugingCurrent() const;
    QStringList gaugingRoot() const;



    /*!
     * Detect Files Map of the current or root directory recursively.
     * output type :
     * \sa gaugingCurrent
     * \sa gaugingRoot
     */
    QMap<QString,QString> mapOfCurrent() const;
    QMap<QString,QString> mapOfRoot() const;


    /*!
     * If folder graph is Empty return true and else return false.
     * \sa currentIsEmpty
     * \sa filesCount
     * \sa foldersCount
     */
    bool isEmpty() const;

    /*!
     * If current directory is Empty return true and else return false.
     * \sa isEmpty
     * \sa filesCount
     * \sa foldersCount
     */
    bool currentIsEmpty() const;


    /*!
     * Returns True if the folder graph is busy and Returns false
     * if is not busy.
     */
    bool isBusy() const;


    /*!
     * Returns number of files in the current directory.
     * \sa foldersCount
     * \sa count
     */
    int filesCount() const;

    /*!
     * Returns number of folders in the current directory.
     * \sa filesCount
     * \sa count
     */
    int foldersCount() const;

    /*!
     * Returns number of files and folders in the current directory.
     * \sa foldersCount
     * \sa filesCount
     */
    int count() const;


    /*!
     * Returns number of files in the root directory.
     * \sa rootFoldersCount
     * \sa rootCount
     */
    int rootFilesCount() const;

    /*!
     * Returns number of folders in the root directory.
     * \sa rootFilesCount
     * \sa rootCount
     */
    int rootFoldersCount() const;

    /*!
     * Returns number of files and folders in the root directory.
     * \sa rootFoldersCount
     * \sa rootFilesCount
     */
    int rootCount() const;

    /*!
     * Set Folder Graph clear.
     */
    void clear();

protected:
    QString pathOf( SFolder *folder ) const;
    QStringList gauging( SFolder *folder ) const;
    QMap<QString,QString> mapping( SFolder *folder ) const;

private:
    SFolderGraphPrivate *p;
};

#endif // SFOLDERGRAPH_H
