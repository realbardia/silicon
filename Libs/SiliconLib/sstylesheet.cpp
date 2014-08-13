/*
 * File:      sstylesheet.cpp
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



/*================================*
 |                                |
 |           SStyleSheet          |
 |                                |
 *================================*/

#include "sstylesheet.h"

SStyleSheet::SStyleSheet()
{
}

void SStyleSheet::setStyle(const QString & style)
{
    styleSheet = style;
}

void SStyleSheet::addStyle(const QString & parent,const QString & style)
{
    styleSheet+=style;
    this->enter(parent);
}

void SStyleSheet::addStyle(const QString & style)
{
    styleSheet+=style;
}

void SStyleSheet::enter(const QString & parent)
{
    int brace=0,
        lastBrace=0,
        pointer=-1;

    for(int i=0;i<styleSheet.size();i++)
    {
        lastBrace=brace;
        if(styleSheet[i]=='{')
            brace++;
        if(styleSheet[i]=='}')
            brace--;

        if(brace==0 && pointer!=-1 && lastBrace==brace+1)
        {
            styleSheet.remove(pointer,i-pointer+1);
            i-=i-pointer;
            pointer=-1;
        }

        if(brace!=0 || pointer!=-1)
            continue;

        if(    (i>0 && styleSheet[i-1].isLetterOrNumber())
            || ( i+parent.size()<styleSheet.size() && styleSheet[i+parent.size()].isLetterOrNumber() )
          )
            continue;

        if(styleSheet.mid(i,parent.size()+1)==parent+':')
            styleSheet.remove(i, parent.size()+1);
        else if(styleSheet.mid(i,parent.size()+1)==parent+'{')
            pointer=i;
    }
}

void SStyleSheet::replace(const QString & from , const QString & to)
{
    int brace=0;

    for(int i=0;i<styleSheet.size();i++)
    {
        if(styleSheet[i]=='{')
            brace++;
        if(styleSheet[i]=='}')
            brace--;

        if(brace!=0)
            continue;

        if(    (i>0 && styleSheet[i-1].isLetterOrNumber())
            || ( i+from.size()<styleSheet.size() && styleSheet[i+from.size()].isLetterOrNumber() )
          )
            continue;

        if(styleSheet.mid(i,from.size())==from)
            styleSheet.replace(i, from.size(),to);
    }
}

void SStyleSheet::clear()
{
    styleSheet.clear();
}

QString SStyleSheet::filter(const QString & parent) const
{
    int brace=0,
        lastBrace=0,
        pointer=-1;

    QString result;

    for(int i=0;i<styleSheet.size();i++)
    {
        lastBrace=brace;
        if(styleSheet[i]=='{')
            brace++;
        if(styleSheet[i]=='}')
            brace--;

        if(brace==0 && pointer!=-1 && lastBrace==brace+1)
        {
            result+=styleSheet.mid(pointer,i-pointer+1);
            pointer=-1;
        }

        if(brace!=0 || pointer!=-1)
            continue;

        if(    (i>0 && styleSheet[i-1].isLetterOrNumber())
            || ( i+parent.size()<styleSheet.size() && styleSheet[i+parent.size()].isLetterOrNumber() )
          )
            continue;

        if(styleSheet.mid(i,parent.size())==parent)
            pointer=i;
    }
    return result;
}

QString SStyleSheet::style() const
{
    return this->styleSheet;
}
