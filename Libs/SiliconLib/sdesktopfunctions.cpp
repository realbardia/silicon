/*
 * File:      sdesktopfunctions.cpp
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

#include "sdesktopfunctions.h"

#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QStringList>
#include <QList>

void SDesktopFunctions::openFile( const QString & fileName )
{
#ifdef Q_OS_LINUX
    QString cm = "mimeopen" ;
    QStringList args;
        args<<"-Ln"<<fileName;
    QProcess::startDetached( cm , args);
#else
    QProcess::startDetached( fileName );
#endif
}

void SDesktopFunctions::openDirectory( const QString & dirName )
{
#ifdef Q_OS_LINUX
    SDesktopFunctions::DesktopType type = SDesktopFunctions::desktopType();
    switch( static_cast<int>(type) )
    {
    case SDesktopFunctions::KDE :
        QProcess::startDetached( "kioclient" , QStringList()<< "exec" << dirName );
        break;
    case SDesktopFunctions::Gnome :
        QProcess::startDetached( "nautilus" , QStringList()<< dirName );
        break;
    case SDesktopFunctions::Xfce :
        QProcess::startDetached( "Thunar" , QStringList()<< dirName );
        break;
    }

#else
    QProcess::startDetached( "explorer" , QStringList()<<dirName );
#endif
}

QString SDesktopFunctions::binaryPath()
{
    return QApplication::applicationFilePath();
    /*
    QString binaryPath = QApplication::argv()[0];
#ifdef Q_OS_UNIX
    if( binaryPath[0] != '/' )
        binaryPath = QDir::currentPath() + '/' + binaryPath;

    return QDir( binaryPath ).canonicalPath();
#else
    if( binaryPath[1] != ':' )
        binaryPath = QDir::currentPath() + '/' + binaryPath;

    return QDir( binaryPath ).canonicalPath();
#endif*/
}

QString SDesktopFunctions::findEnvVariable( const QString & variable )
{
    QList<QString> list = QProcess::systemEnvironment();
    for(int i=0;i<list.count();i++)
        if( list.at(i).left(variable.size()+1) == variable+'=' )
            return list.at(i).right( list.at(i).size() - variable.size() -1);

    return QString();
}

#ifdef Q_OS_LINUX
SDesktopFunctions::DesktopType SDesktopFunctions::desktopType()
{
    QString dekstop_session = SDesktopFunctions::findEnvVariable( "DESKTOP_SESSION" ).toLower();
    QString kde_full_session = SDesktopFunctions::findEnvVariable( "KDE_FULL_SESSION" ).toLower();

    if( kde_full_session.contains( "true" ) )
        return SDesktopFunctions::KDE;
    else if( dekstop_session.contains( "gnome" ) )
        return SDesktopFunctions::Gnome;
    else if( dekstop_session.contains( "ubuntu" ) )
        return SDesktopFunctions::Ubuntu;
    else if( dekstop_session.contains( "xfce" ) )
        return SDesktopFunctions::Xfce;

    return SDesktopFunctions::Unknown;
}
#endif
