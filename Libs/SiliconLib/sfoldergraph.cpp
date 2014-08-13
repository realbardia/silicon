/*
 * File:      sfoldergraph.cpp
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

#include "sfoldergraph.h"

#include <QFileInfo>
#include <QCoreApplication>




/*================================*
 |                                |
 |             SFile              |
 |                                |
 *================================*/

struct LIBSILICON_EXPORT SFile
{
    QString name;
    QFile *file;
};




/*================================*
 |                                |
 |            SFolder             |
 |                                |
 *================================*/

struct LIBSILICON_EXPORT SFolder
{
    QString name;
    SFolder *up;
    QList<SFolder *> folders;
    QList<SFile *> files;

    long long size;

    ~SFolder()
    {
        for( int i=0 ; i<files.count() ; i++ )
            delete files[i];

        for( int i=0 ; i<folders.count() ; i++ )
            delete folders[i];
    }
};




/*================================*
 |                                |
 |      SFolderGraphPrivate       |
 |                                |
 *================================*/

class SFolderGraphPrivate
{
public:
    SFolderGraphPrivate();
    ~SFolderGraphPrivate();

    void addFolder( const QDir & dir );
    void addFile( QFile *file );
    void up();
    void cd( QString dirName );
    void cdRoot();

    SFolder *root;
    SFolder *current;

    bool lock;
    int files_number;
    int folders_number;

    long long disk_size;
};




SFolderGraphPrivate::SFolderGraphPrivate()
{
    root = new SFolder;
        root->name = "root";
        root->up = root;

    current = root;

    files_number = 0;
    folders_number = 1;
    disk_size = 0;
    lock = false;
}

void SFolderGraphPrivate::addFile( QFile *file )
{
    QString fileName = QFileInfo(file->fileName()).fileName();

    for( int i=0 ; i<current->files.count() ; i++ )
        if( fileName == current->files.at(i)->name  )
            return ;

    for( int i=0 ; i<current->folders.count() ; i++ )
        if( fileName == current->folders.at(i)->name  )
            return ;

    SFile *new_file = new SFile;
        new_file->file = file;
        new_file->name = fileName;

    current->files << new_file ;

    current->size += file->size();
    disk_size += file->size();
    files_number++;
}

void SFolderGraphPrivate::addFolder( const QDir & dir )
{
    SFolder *newDir;
    QString dirName = dir.dirName();
    bool is_exist = false;

    for( int i=0 ; i<current->files.count() ; i++ )
        if( dirName == current->files.at(i)->name  )
            return ;

    for( int i=0 ; i<current->folders.count() ; i++ )
        if( current->folders.at(i)->name == dirName )
        {
            is_exist = true;
            newDir = current->folders.at(i);
            break;
        }

    if( !is_exist )
    {
        newDir = new SFolder;
        newDir->name = dir.dirName();
        newDir->up = current;
        newDir->size = 0;
        current->folders << newDir;
    }

    this->cd( newDir->name );

    QStringList dirList = dir.entryList( QStringList()<<"*" , QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks );
    for( int i=0 ; i<dirList.size() ; i++ )
    {
        QCoreApplication::processEvents();
        this->addFolder( QDir( dir.path() + '/' + dirList.at(i) ) );
    }

    QStringList fileList = dir.entryList( QDir::Files );
    for( int i=0 ; i<fileList.size() ; i++ )
        this->addFile( new QFile(dir.path() + '/' + fileList.at(i)) );

    this->up();
    folders_number++;
}

void SFolderGraphPrivate::cd( QString dirName )
{
    if( dirName[0] == '/' )
    {
        this->cdRoot();
        dirName.remove( 0 , 1 );
        this->cd( dirName );
        return ;
    }
    else if( dirName.contains( "/" ) )
    {
        QString tmp;
        for( int i=0 ; i<dirName.count() ; i++ )
            if( dirName[i] == '/' )
            {
                tmp = dirName.mid( 0 , i );
                dirName.remove( 0 , i+1 );
                break ;
            }

        this->cd( tmp );
        this->cd( dirName );
        return ;
    }

    for( int i=0 ; i<current->folders.count() ; i++ )
        if( current->folders.at(i)->name == dirName )
        {
            current = current->folders.at(i);
            break;
        }
}

void SFolderGraphPrivate::cdRoot()
{
    current = root;
}

void SFolderGraphPrivate::up()
{
    current = current->up;
}


SFolderGraphPrivate::~SFolderGraphPrivate()
{
    delete root;
}



/*================================*
 |                                |
 |          SFolderGraph          |
 |                                |
 *================================*/

SFolderGraph::SFolderGraph()
{
    p = new SFolderGraphPrivate();
}

SFolderGraph::~SFolderGraph()
{
    delete p;
}

void SFolderGraph::addFile( QFile *file )
{
    p->addFile( file );
}

void SFolderGraph::addFiles( const QList<QFile *> & list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->addFile( list.at(i) );
}

void SFolderGraph::addFile( const QString & file )
{
    this->addFile( new QFile( file ) );
}

void SFolderGraph::addFiles( const QStringList & list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->addFile( list.at(i) );
}

void SFolderGraph::addFolder( const QDir & dir )
{
    p->lock = true;

    p->addFolder( dir );
    p->lock = false;
}

void SFolderGraph::addFolders( const QList<QDir> & list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->addFolder( list.at(i) );
}

void SFolderGraph::newFolder( const QString & name )
{
    SFolder *newDir;

    for( int i=0 ; i<p->current->files.count() ; i++ )
        if( name == p->current->files.at(i)->name  )
            return ;

    for( int i=0 ; i<p->current->folders.count() ; i++ )
        if( name == p->current->folders.at(i)->name )
            return ;

    newDir = new SFolder;
        newDir->name = name;
        newDir->up = p->current;
        newDir->size = 0;
        p->current->folders << newDir;
}

void SFolderGraph::removeFile( const QString & fileName )
{
    for( int i=0 ; i<p->current->files.count() ; i++ )
        if( p->current->files.at(i)->name == fileName )
        {
            p->current->size -= p->current->files.at(i)->file->size();
            p->disk_size -= p->current->files.at(i)->file->size();
            p->files_number--;

            delete p->current->files.at(i)->file;
            delete p->current->files.takeAt(i);
        }
}

void SFolderGraph::removeFolder( const QString & folderName )
{
    for( int i=0 ; i<p->current->folders.count() ; i++ )
        if( p->current->folders.at(i)->name == folderName )
        {
            p->folders_number--; // I'm Bug , Fix Me :D
            delete p->current->folders.takeAt(i);
        }
}

void SFolderGraph::rename( const QString & currentName , const QString & newName )
{
    for( int i=0 ; i<p->current->folders.count() ; i++ )
        if( p->current->folders.at(i)->name == currentName )
        {
            p->current->folders.at(i)->name = newName;
            return ;
        }

    for( int i=0 ; i<p->current->files.count() ; i++ )
        if( p->current->files.at(i)->name == currentName )
        {
            p->current->files.at(i)->name = newName;
            return ;
        }
}

void SFolderGraph::cd( QString dirName )
{
    p->cd( dirName );
}

void SFolderGraph::up()
{
    p->up();
}

void SFolderGraph::cdRoot()
{
    p->cdRoot();
}

QStringList SFolderGraph::currentFolders() const
{
    QStringList result;
    for( int i=0 ; i<p->current->folders.count() ; i++ )
        result << p->current->folders.at(i)->name;

    return result;
}

QStringList SFolderGraph::currentFiles() const
{
    QStringList result;
    for( int i=0 ; i<p->current->files.count() ; i++ )
        result << p->current->files.at(i)->name;

    return result;
}

QString SFolderGraph::fileAddress( const QString & fileName ) const
{
    for( int i=0 ; i<p->current->files.count() ; i++ )
        if( p->current->files.at(i)->name == fileName )
            return p->current->files.at(i)->file->fileName();

    return QString();
}

QString SFolderGraph::currentPath() const
{
    return this->pathOf( p->current );
}

QString SFolderGraph::pathOf( SFolder *folder ) const
{
    if( folder == p->root )
        return QString("/");
    else
        return this->pathOf( folder->up ) + folder->name + '/';
}

QStringList SFolderGraph::gaugingCurrent() const
{
    return this->gauging( p->current );
}

QStringList SFolderGraph::gaugingRoot() const
{
    return this->gauging( p->root );
}

QMap<QString,QString> SFolderGraph::mapOfCurrent() const
{
    return this->mapping( p->current );
}

QMap<QString,QString> SFolderGraph::mapOfRoot() const
{
    return this->mapping( p->root );
}

bool SFolderGraph::isEmpty() const
{
    return p->root->folders.isEmpty() && p->root->files.isEmpty();
}

bool SFolderGraph::currentIsEmpty() const
{
    return p->current->folders.isEmpty() && p->current->files.isEmpty();
}

bool SFolderGraph::isBusy() const
{
    return p->lock;
}

int SFolderGraph::filesCount() const
{
    return p->current->files.count();
}

int SFolderGraph::foldersCount() const
{
    return p->current->folders.count();
}

int SFolderGraph::count() const
{
    return this->filesCount() + this->foldersCount();
}

int SFolderGraph::rootFilesCount() const
{
    return p->root->files.count();
}

int SFolderGraph::rootFoldersCount() const
{
    return p->root->folders.count();
}

int SFolderGraph::rootCount() const
{
    return this->rootFilesCount() + this->rootFoldersCount();
}

QStringList SFolderGraph::gauging( SFolder *folder ) const
{
    QStringList out;
    QString dir = this->pathOf( folder );

    for( int i=0 ; i<folder->files.count() ; i++ )
        out << dir + folder->files.at(i)->name + "=" + folder->files.at(i)->file->fileName();

    for( int i=0 ; i<folder->folders.count() ; i++ )
        out << this->gauging( folder->folders.at(i) );

    return out;
}

QMap<QString,QString> SFolderGraph::mapping( SFolder *folder ) const
{
    QMap<QString,QString> out;
    QString dir = this->pathOf( folder );

    for( int i=0 ; i<folder->files.count() ; i++ )
        out.insert( dir + folder->files.at(i)->name , folder->files.at(i)->file->fileName() );

    for( int i=0 ; i<folder->folders.count() ; i++ )
        out.unite( this->mapping(folder->folders.at(i)) );

    return out;
}

void SFolderGraph::clear()
{
    delete  p;
    p = new SFolderGraphPrivate();
}
