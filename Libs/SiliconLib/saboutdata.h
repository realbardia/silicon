/*
 * File:      saboutdata.h
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

#ifndef SABOUTDATA_H
#define SABOUTDATA_H

#include <QString>
#include <QIcon>
#include <QStringList>
#include <QPixmap>
#include <QList>
#include <QVariant>
#include <QHash>

#include "sauthorinfo.h"

#include "libsilicon_global.h"

/*! \class
 * The SAboutData provides About Data class that holds
 * SApplications About datas.
 */
class LIBSILICON_EXPORT SAboutData
{
public:
    SAboutData();
    SAboutData( const QString & name , const QString &  version , const QString & license ,const QString & description );

    enum Type {
        Sound,
        Video,
        DVD,
        CD,
        BluRay,
        Image,
        Photo,
        Tools,
        Other
    };

    void setName(const QString & name);
    void setTranslatedName( const QString & name );
    void setVersion(const QString &  version);
    void setOrganizer(const QString & organizer);
    void setLicense(const QString & license);
    void setDescription(const QString & description);
    void setCopyRight(const QString & copyright);
    void setAbout(const QString & about);
    void setToolTip( const QString & tooltip );

    void setHomePage( const QString & home_page );
    void setMail( const QString & mail );

    void setType(const QList<Type> & type);
    void setCompatibleFileTypes( const QStringList & list );

    void setAuthors(const QList<SAuthorInfo> & list);
    void setBinaryDepends(const QStringList & list);
    void setSAppDepends(const QStringList & list);

    enum TranslatorDatas{
        Name,
        Country,
        Language,
        Picture,
        HomePage,
        Email
    };

    static void addTranslator( const QHash<TranslatorDatas,QVariant> & hash );

    void setPrivateStructure( bool state );     /*! This Feature disabled at the moment */
    void setMultiRun( bool state );

    void setIcon(const QIcon & icon);
    void setPixmap(const QPixmap & pixmap);

    void setCommandMap( const QString & map );

    const QString & name() const;
    const QString & translatedName() const;
    const QString & version() const;
    const QString & organizer() const;
    const QString & license() const;
    const QString & description() const;
    const QString & copyRight() const;
    const QString & about() const;
    const QString & tooltip() const;

    const QString & homePage() const;
    const QString & mail() const;

    const QList<Type> & type() const;
    const QStringList & compatibleFileTypes() const;

    const QList<SAuthorInfo> & authors() const;
    const QStringList & binaryDepends() const;
    const QStringList & sappDepends() const;
    static const QList<QHash<TranslatorDatas,QVariant> > & translators();

    bool privateStructure() const;
    bool multiRun() const;

    const QIcon & icon() const;
    const QPixmap & pixmap() const;

    const QString & commandMap() const;

    bool operator== (SAboutData about) const;
private:
    QString nameStr;
    QString tnameStr;
    QString versionStr;
    QString organizerStr;
    QString licenseStr;
    QString descriptionStr;
    QString copyrightStr;
    QString aboutStr;
    QString tooltipStr;

    QString home_page_str;
    QString mail_str;

    QList<Type> typeStr;
    QStringList compatibe_file_types;
    QList<SAuthorInfo> authors_list;
    QStringList sappDependsLst;
    QStringList binaryDependsLst;

    bool multiRunStt;
    bool prevStructure;

    QIcon iconObj;
    QPixmap pixmapObj;

    QString command_map;
};

#endif // SABOUTDATA_H
