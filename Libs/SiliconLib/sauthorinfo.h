/*
 * File:      sauthorinfo.h
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

#ifndef SAUTHORINFO_H
#define SAUTHORINFO_H

#include <QString>
#include <QLocale>
#include <QDate>
#include <QList>
#include <QStringList>
#include <QPixmap>
#include <QIcon>

#include "libsilicon_global.h"

/*! \class
 * The SAuthorInfo provides Author Info class that
 * holds applications authors informations.
 */
class LIBSILICON_EXPORT SAuthorInfo
{
public:
    SAuthorInfo();

    const QString & firstName() const;
    const QString & lastName() const;
    const QString & nickName() const;
    const QLocale & locale() const;
    const QDate & bornDate() const;

    const QStringList & rollList() const;

    const QPixmap & avatar() const;
    const QIcon & authorIcon() const;
    const QString & aboutAuthor() const;

    const QString & mail() const;
    const QString & homePage() const;
    const QString & blog() const;
    const QString & irc() const;

    const QString & gtalkIM() const;
    const QString & jabberIM() const;
    const QString & skypeIM() const;
    const QString & aimIM() const;
    const QString & msnIM() const;
    const QString & yahooIM() const;

    const QStringList & programingLanguagesList() const;
    const QStringList & projectsList() const;



    void setFirstName( const QString & first_name );
    void setLastName( const QString & last_name );
    void setNickName( const QString & nick_name );
    void setLocale( const QLocale & l );
    void setBornDate( const QDate & born_date );

    void setRollList( const QStringList & r );

    void setAvatarPixmap( const QPixmap & avatar );
    void setAuthorIcon( const QIcon & icon );
    void setAboutAuthor( const QString & about );

    void setMail( const QString & m );
    void setHomePage( const QString & home_page );
    void setBlog( const QString & b );
    void setIRC( const QString & i );

    void setGTalkIM( const QString & gtalk );
    void setJabberIM( const QString & jabber );
    void setSkypeIM( const QString & skype );
    void setAIM( const QString & aim );
    void setMSN( const QString & msn );
    void setYahooIM( const QString & yahoo );

    void setProgramingLanguagesList( const QStringList & programing_languages );
    void setProjectsLists( const QStringList & projects );

    bool operator==( SAuthorInfo author ) const;
    void operator=( SAuthorInfo author );

    void clear();
    bool isEmpty() const;

private:
    QString first_name_str;
    QString last_name_str;
    QString nick_name_str;
    QLocale locale_obj;
    QDate born_date;

    QStringList roll_list_str;

    QPixmap avatar_pixmap;
    QIcon author_icon;
    QString about_author;

    QString mail_str;
    QString home_page_str;
    QString blog_str;
    QString irc_str;

    QString gtalk_im_str;
    QString jabber_im_str;
    QString skype_im_str;
    QString aim_im_str;
    QString msn_im_str;
    QString yahoo_im_str;

    QStringList programing_languages_list;
    QStringList projects_list;
};

#endif // SAUTHORINFO_H
