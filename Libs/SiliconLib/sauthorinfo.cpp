/*
 * File:      sauthorinfo.cpp
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

#include "sauthorinfo.h"
#include "smastericons.h"

SAuthorInfo::SAuthorInfo()
{
    setAuthorIcon( SMasterIcons::icon( QSize(48,48) , "im-user.png" ) );
    setAvatarPixmap( SMasterIcons::icon( QSize(48,48) , "im-user.png" ).pixmap(128,128) );
}

const QString & SAuthorInfo::firstName() const
{
    return this->first_name_str;
}

const QString & SAuthorInfo::lastName() const
{
    return this->last_name_str;
}

const QString & SAuthorInfo::nickName() const
{
    return this->nick_name_str;
}

const QLocale & SAuthorInfo::locale() const
{
    return this->locale_obj;
}

const QDate & SAuthorInfo::bornDate() const
{
    return this->born_date;
}

const QStringList & SAuthorInfo::rollList() const
{
    return this->roll_list_str;
}

const QPixmap & SAuthorInfo::avatar() const
{
    return this->avatar_pixmap;
}

const QIcon & SAuthorInfo::authorIcon() const
{
    return this->author_icon;
}

const QString & SAuthorInfo::aboutAuthor() const
{
    return this->about_author;
}

const QString & SAuthorInfo::mail() const
{
    return this->mail_str;
}

const QString & SAuthorInfo::homePage() const
{
    return this->home_page_str;
}

const QString & SAuthorInfo::blog() const
{
    return this->blog_str;
}

const QString & SAuthorInfo::irc() const
{
    return this->irc_str;
}

const QString & SAuthorInfo::gtalkIM() const
{
    return this->gtalk_im_str;
}

const QString & SAuthorInfo::jabberIM() const
{
    return this->jabber_im_str;
}

const QString & SAuthorInfo::skypeIM() const
{
    return this->skype_im_str;
}

const QString & SAuthorInfo::aimIM() const
{
    return this->aim_im_str;
}

const QString & SAuthorInfo::msnIM() const
{
    return this->msn_im_str;
}

const QString & SAuthorInfo::yahooIM() const
{
    return this->yahoo_im_str;
}

const QStringList & SAuthorInfo::programingLanguagesList() const
{
    return this->programing_languages_list;
}

const QStringList & SAuthorInfo::projectsList() const
{
    return this->projects_list;
}



void SAuthorInfo::setFirstName( const QString & first_name )
{
    this->first_name_str = first_name;
}

void SAuthorInfo::setLastName( const QString & last_name )
{
    this->last_name_str = last_name;
}

void SAuthorInfo::setNickName( const QString & nick_name )
{
    this->nick_name_str = nick_name;
}

void SAuthorInfo::setLocale( const QLocale & l )
{
    this->locale_obj = l;
}

void SAuthorInfo::setBornDate( const QDate & born_date )
{
    this->born_date = born_date;
}

void SAuthorInfo::setRollList( const QStringList & r )
{
    this->roll_list_str = r;
}

void SAuthorInfo::setAvatarPixmap( const QPixmap & avatar )
{
    this->avatar_pixmap = avatar;
}

void SAuthorInfo::setAuthorIcon( const QIcon & icon )
{
    this->author_icon = icon;
}

void SAuthorInfo::setAboutAuthor( const QString & about )
{
    this->about_author = about;
}

void SAuthorInfo::setMail( const QString & m )
{
    this->mail_str = m;
}

void SAuthorInfo::setHomePage( const QString & home_page )
{
    this->home_page_str = home_page;
}

void SAuthorInfo::setBlog( const QString & b )
{
    this->blog_str = b;
}

void SAuthorInfo::setIRC( const QString & i )
{
    this->irc_str = i;
}

void SAuthorInfo::setGTalkIM( const QString & gtalk )
{
    this->gtalk_im_str = gtalk;
}

void SAuthorInfo::setJabberIM( const QString & jabber )
{
    this->jabber_im_str = jabber;
}

void SAuthorInfo::setSkypeIM( const QString & skype )
{
    this->skype_im_str = skype;
}

void SAuthorInfo::setAIM( const QString & aim )
{
    this->aim_im_str = aim;
}

void SAuthorInfo::setMSN( const QString & msn )
{
    this->msn_im_str = msn;
}

void SAuthorInfo::setYahooIM( const QString & yahoo )
{
    this->yahoo_im_str = yahoo;
}

void SAuthorInfo::setProgramingLanguagesList( const QStringList & programing_languages )
{
    this->programing_languages_list = programing_languages;
}

void SAuthorInfo::setProjectsLists( const QStringList & projects )
{
    this->projects_list = projects;
}

bool SAuthorInfo::operator==( SAuthorInfo author ) const
{
    bool result = ( author.firstName() == this->firstName() ) &&
                  ( author.lastName() == this->lastName() ) &&
                  ( author.nickName() == this->nickName() ) &&
                  ( author.locale() == this->locale() ) &&
                  ( author.bornDate() == this->bornDate() ) ;

    return result;
}

void SAuthorInfo::operator=( SAuthorInfo author )
{
    this->setFirstName( author.firstName() );
    this->setLastName( author.lastName() );
    this->setNickName( author.nickName() );
    this->setLocale( author.locale() );
    this->setBornDate( author.bornDate() );

    this->setAvatarPixmap( author.avatar() );
    this->setAuthorIcon( author.authorIcon() );
    this->setAboutAuthor( author.aboutAuthor() );

    this->setMail( author.mail() );
    this->setHomePage( author.homePage() );
    this->setBlog( author.blog() );
    this->setIRC( author.irc() );

    this->setGTalkIM( author.gtalkIM() );
    this->setJabberIM( author.jabberIM() );
    this->setSkypeIM( author.skypeIM() );
    this->setAIM( author.aimIM() );
    this->setMSN( author.msnIM() );
    this->setYahooIM( author.yahooIM() );

    this->setProgramingLanguagesList( author.programingLanguagesList() );
    this->setProjectsLists( author.projectsList() );
}

void SAuthorInfo::clear()
{
    this->operator =( SAuthorInfo() );
}

bool SAuthorInfo::isEmpty() const
{
    bool result = first_name_str.isEmpty() &&
                  last_name_str.isEmpty() &&
                  nick_name_str.isEmpty() &&
                  born_date.isNull() &&

                  roll_list_str.isEmpty() &&

                  avatar_pixmap.isNull() &&
                  author_icon.isNull() &&
                  about_author.isEmpty() &&

                  mail_str.isEmpty() &&
                  home_page_str.isEmpty() &&
                  blog_str.isEmpty() &&
                  irc_str.isEmpty() &&

                  gtalk_im_str.isEmpty() &&
                  jabber_im_str.isEmpty() &&
                  skype_im_str.isEmpty() &&
                  aim_im_str.isEmpty() &&
                  msn_im_str.isEmpty() &&
                  yahoo_im_str.isEmpty() &&

                  programing_languages_list.isEmpty() &&
                  projects_list.isEmpty();

    return result;
}
