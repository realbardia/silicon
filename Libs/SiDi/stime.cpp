/*
 * File:      stime.cpp
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

#include "stime.h"



/*================================*
 |                                |
 |             STime              |
 |                                |
 *================================*/

STime::STime()
{
    //p = new STimePrivate;
}

STime::STime( const STime & time )
{
    setMiliSecond( time.toMiliSecond() );
}

STime::STime( int hour , int minute , int second , int mili_second )
{
    //p = new STimePrivate;
    this->set( hour , minute , second , mili_second );
}

STime::~STime()
{
    //delete p;
}

void STime::set( int hour , int minute , int second , int mili_second )
{
    this->reset();

    this->addHour( hour );
    this->addMinute( minute );
    this->addSecond( second );
    this->addMiliSecond( mili_second );
}

void STime::reset()
{
    p.mili_int = 0 ;
    p.sec_int = 0 ;
    p.min_int = 0 ;
    p.hour_int = 0 ;
    p.day_int = 0 ;
}

void STime::setMiliSecond( int value )
{
    p.mili_int = value % 1000 ;
    this->setSecond( value/1000 );
}

void STime::setSecond( int value )
{
    p.sec_int = value % 60 ;
    this->setMinute( value/60 );
}

void STime::setMinute( int value )
{
    p.min_int = value % 60 ;
    this->setHour( value/60 );
}

void STime::setHour( int value )
{
    p.hour_int = value % 24 ;
    this->setDay( value/24 );
}

void STime::setDay( int value )
{
    p.day_int = value ;
}

void STime::addMiliSecond( int value )
{
    value += p.mili_int;

    p.mili_int = value % 1000 ;
    this->addSecond( value/1000 );
}

void STime::addSecond( int value )
{
    value += p.sec_int;

    p.sec_int = value % 60 ;
    this->addMinute( value/60 );
}

void STime::addMinute( int value )
{
    value += p.min_int;

    p.min_int = value % 60 ;
    this->addHour( value/60 );
}

void STime::addHour( int value )
{
    value += p.hour_int;

    p.hour_int = value % 24 ;
    this->addDay( value/24 );
}

void STime::addDay( int value )
{
    p.day_int += value ;
}

int STime::miliSecond() const
{
    return p.mili_int;
}

int STime::second() const
{
    return p.sec_int;
}

int STime::minute() const
{
    return p.min_int;
}

int STime::hour() const
{
    return p.hour_int;
}

int STime::day() const
{
    return p.day_int;
}

int STime::toMiliSecond() const
{
    int result = this->toSecond()*1000 + p.mili_int;
    return result;
}

int STime::toSecond() const
{
    int result = this->toMinute()*60 + p.sec_int;
    return result;
}

int STime::toMinute() const
{
    int result = this->toHour()*60 + p.min_int;
    return result;
}

int STime::toHour() const
{
    int result = this->toDay()*24 + p.hour_int;
    return result;
}

int STime::toDay() const
{
    int result = p.day_int;
    return result;
}

QString STime::toString() const
{
    QString str;

    if( p.day_int > 0 )    // If Day is zero , dont add Day to result //
        str = str + QString::number( p.day_int ) + "Day ";

    str = str + QString::number( p.hour_int ) + ":";
    str = str + QString::number( p.min_int ) + ":";
    str = str + QString::number( p.sec_int );

    if( p.mili_int > 0 )   // If MiliSecond is zero , dont add MiliSecond to result //
        str = str + ":" + QString::number( p.mili_int );

    return str;
}

STime & STime::operator=( STime time )
{
    this->setMiliSecond( time.toMiliSecond() );

    return *this;
}

STime STime::operator+( STime time )
{
    STime result = *this;
        result.addMiliSecond( time.toMiliSecond() );

    return result;
}

STime & STime::operator+=( STime time )
{
    this->addMiliSecond( time.toMiliSecond() );
    return *this;
}

STime STime::operator-( STime time )
{
    int ms = this->toMiliSecond() - time.toMiliSecond();

    STime result;
        result.setMiliSecond( ms );

    return result;
}

STime & STime::operator-=( STime time )
{
    int ms = this->toMiliSecond() - time.toMiliSecond();
    this->reset();
    this->setMiliSecond( ms );

    return *this;
}

STime STime::operator*( int num )
{
    int ms = this->toMiliSecond() * num;

    STime result;
        result.setMiliSecond( ms );

    return result;
}

STime STime::operator/( int num )
{
    if( num == 0 )
        return STime();

    int ms = this->toMiliSecond() / num;

    STime result;
        result.setMiliSecond( ms );

    return result;
}

bool STime::operator==( STime time ) const
{
    return this->toMiliSecond() == time.toMiliSecond();
}

bool STime::operator!=( STime time ) const
{
    return this->toMiliSecond() != time.toMiliSecond();
}

bool STime::operator>=( STime time ) const
{
    return this->toMiliSecond() >= time.toMiliSecond();
}

bool STime::operator>( STime time ) const
{
    return this->toMiliSecond() > time.toMiliSecond();
}

bool STime::operator<=( STime time ) const
{
    return this->toMiliSecond() <= time.toMiliSecond();
}

bool STime::operator<( STime time ) const
{
    return this->toMiliSecond() < time.toMiliSecond();
}
