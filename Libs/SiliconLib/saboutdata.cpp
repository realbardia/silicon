/*
 * File:      saboutdata.cpp
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

#include "saboutdata.h"


QList<QHash<SAboutData::TranslatorDatas,QVariant> > translators_hash;

SAboutData::SAboutData()
{
    this->setPrivateStructure( true );
    this->setMultiRun( true );
}

SAboutData::SAboutData( const QString & name , const QString &  version , const QString & license ,const QString & description )
{
    this->setName( name );
    this->setVersion( version );
    this->setLicense( license );
    this->setDescription( description );
    this->setPrivateStructure( true );
    this->setMultiRun( true );
}



void SAboutData::setName(const QString & name)
{
    this->nameStr = name;
}

void SAboutData::setTranslatedName( const QString & name )
{
    this->tnameStr = name;
}

void SAboutData::setVersion(const QString &  version)
{
    this->versionStr = version;
}

void SAboutData::setOrganizer(const QString & organizer)
{
    this->organizerStr = organizer;
}

void SAboutData::setLicense(const QString & license)
{
    this->licenseStr = license;
}

void SAboutData::setDescription(const QString & description)
{
    this->descriptionStr = description ;
}

void SAboutData::setCopyRight(const QString & copyright)
{
    this->copyrightStr = copyright;
}

void SAboutData::setAbout(const QString & about)
{
    this->aboutStr = about;
}

void SAboutData::setToolTip( const QString & tooltip )
{
    this->tooltipStr = tooltip;
}

void SAboutData::setHomePage( const QString & home_page )
{
    this->home_page_str = home_page;
}

void SAboutData::setMail( const QString & mail )
{
    this->mail_str = mail;
}

void SAboutData::setType(const QList<SAboutData::Type> & type)
{
    this->typeStr = type;
}

void SAboutData::setCompatibleFileTypes( const QStringList & list )
{
    this->compatibe_file_types = list;
}

void SAboutData::setAuthors(const QList<SAuthorInfo> & list)
{
    this->authors_list = list;
}

void SAboutData::setBinaryDepends(const QStringList & list)
{
    this->binaryDependsLst = list;
}

void SAboutData::setSAppDepends(const QStringList & list)
{
    this->sappDependsLst = list;
}

void SAboutData::addTranslator( const QHash<SAboutData::TranslatorDatas,QVariant> & hash )
{
    translators_hash << hash;
}

void SAboutData::setPrivateStructure( bool state )
{
    this->prevStructure = state;
}

void SAboutData::setMultiRun( bool state )
{
    this->multiRunStt = state;
}

void SAboutData::setIcon(const QIcon & icon)
{
    this->iconObj = icon;
}

void SAboutData::setPixmap(const QPixmap & pixmap)
{
    this->pixmapObj = pixmap;
}

void SAboutData::setCommandMap( const QString & map )
{
    command_map = map;
}



const QString & SAboutData::name() const
{
    return this->nameStr;
}

const QString & SAboutData::translatedName() const
{
    return this->tnameStr;
}

const QString & SAboutData::version() const
{
    return this->versionStr;
}

const QString & SAboutData::organizer() const
{
    return this->organizerStr;
}

const QString & SAboutData::license() const
{
    return this->licenseStr;
}

const QString & SAboutData::description() const
{
    return this->descriptionStr;
}

const QString & SAboutData::copyRight() const
{
    return this->copyrightStr;
}

const QString & SAboutData::about() const
{
    return this->aboutStr;
}

const QString & SAboutData::tooltip() const
{
    return this->tooltipStr;
}

const QString & SAboutData::homePage() const
{
    return this->home_page_str;
}

const QString & SAboutData::mail() const
{
    return this->mail_str;
}

const QList<SAboutData::Type> & SAboutData::type() const
{
    return this->typeStr;
}

const QStringList & SAboutData::compatibleFileTypes() const
{
    return this->compatibe_file_types;
}

const QList<SAuthorInfo> &  SAboutData::authors() const
{
    return this->authors_list;
}

const QStringList & SAboutData::binaryDepends() const
{
    return this->binaryDependsLst;
}

const QStringList & SAboutData::sappDepends() const
{
    return this->sappDependsLst;
}

const QList<QHash<SAboutData::TranslatorDatas,QVariant> > &SAboutData::translators()
{
    return translators_hash;
}

bool SAboutData::privateStructure() const
{
    return this->prevStructure;
}

bool SAboutData::multiRun() const
{
    return this->multiRunStt ;
}

const QIcon & SAboutData::icon() const
{
    return this->iconObj;
}

const QPixmap & SAboutData::pixmap() const
{
    return this->pixmapObj;
}

const QString &SAboutData::commandMap() const
{
    return command_map;
}

bool SAboutData::operator ==(SAboutData about) const
{
    if(    this->authors_list      ==  about.authors_list
        && this->copyrightStr    ==  about.copyrightStr
        && this->descriptionStr  ==  about.descriptionStr
        && this->licenseStr      ==  about.licenseStr
        && this->nameStr         ==  about.nameStr
        && this->organizerStr    ==  about.organizerStr
        && this->aboutStr        ==  about.aboutStr         )
                return true;

    return false;
}

