/*
 * File:      stime.h
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

#ifndef STIME_H
#define STIME_H

#include <QString>

#include "SiDi_global.h"


/*================================*
 |                                |
 |          STimePrivate          |
 |                                |
 *================================*/

class STimePrivate
{
public:
    int mili_int;       // Variable that keep milisecond
    int sec_int;        // Variable that keep Second
    int min_int;        // Variable that keep Minute
    int hour_int;       // Variable that keep Hour
    int day_int;        // Variable that keep Day
};

/*! \class
 *  The STime class defines time with 5 integer (day , hour , minute , second , milisecond)
 *  It's not Timer and not Clock
 *  Default time is : 0 day 0 hour 0 minute 0 second and 0 mili second
 */
class SIDI_EXPORT STime
{
/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS :
 *     STime();
 *     STime( int hour , int minute , int second , int mili_second = 0 );
 *     ~STime();
 *
 *     void set( int hour , int minute , int second , int mili_second = 0 );
 *
 *     void reset();
 *
 *     void setMiliSecond( int value );
 *     void setSecond( int value );
 *     void setMinute( int value );
 *     void setHour( int value );
 *     void setDay( int value );
 *
 *     void addMiliSecond( int value );
 *     void addSecond( int value );
 *     void addMinute( int value );
 *     void addHour( int value );
 *     void addDay( int value );
 *
 *     int miliSecond() const;
 *     int second() const;
 *     int minute() const;
 *     int hour() const;
 *     int day() const;
 *
 *     int toMiliSecond() const;
 *     int toSecond() const;
 *     int toMinute() const;
 *     int toHour() const;
 *     int toDay() const;
 *
 *     QString toString() const;
 *
 *
 * OPERATORS :
 *     STime & operator=( STime time );
 *
 *     STime operator+( STime time );
 *     STime & operator+=( STime time );
 *     STime operator-( STime time );
 *     STime & operator-=( STime time );
 *     STime operator*( int num );
 *     STime operator/( int num );
 *
 *
 *     bool operator==( STime time ) const;
 *     bool operator!=( STime time ) const;
 *     bool operator>=( STime time ) const;
 *     bool operator>( STime time ) const;
 *     bool operator<=( STime time ) const;
 *     bool operator<( STime time ) const;
 */

public:
    /*!
     * Constructs an item and set hour , minute and second
     * \sa set
     */
    STime();
    STime( const STime & time );
    STime( int hour , int minute , int second , int mili_second = 0 );
    ~STime();


    /*!
     * set Function set hour , minute , second and if you want milisecond of class
     * mili_second default value is 0
     * \sa reset
     */
    void set( int hour , int minute , int second , int mili_second = 0 );


    /*!
     * set day , hour , minute , second and milisecond zero
     */
    void reset();


    /*!
     * Five blow functions set milisecond , second , minute , hour and day to value
     * \example
     *              STime time( 3 , 17 , 33 );      // now time set to : 0 day 03:17:33
     *              time.setSecond( 2 );            // result : 0 day 03:17:02
     *              time.setDay( 3 );               // result : 3 day 03:17:02
     */
    void setMiliSecond( int value );
    void setSecond( int value );
    void setMinute( int value );
    void setHour( int value );
    void setDay( int value );


    /*!
     * Five blow functions add value to milisecond , second , minute , hour and day
     * \example
     *              STime time( 3 , 17 , 33 );      // now time set to : 0 day 03:17:33
     *              time.addMinute( 2 );            // result : 0 day 03:19:33
     *              time.addSecond( 30 );           // result : 0 day 03:20:03
     */
    void addMiliSecond( int value );
    void addSecond( int value );
    void addMinute( int value );
    void addHour( int value );
    void addDay( int value );


    /*!
     * Five blow functions return value of milisecond , second , minute , hour and day
     * \example
     *              STime time( 3 , 17 , 33 );      // now time set to : 0 day 03:17:33
     *              time.second();                  // return : 33
     *              time.minute();                  // return : 17
     */
    int miliSecond() const;
    int second() const;
    int minute() const;
    int hour() const;
    int day() const;



    /*!
     * Five blow functions calculate and return time in
     * milisecond , second , minute , hour or day
     * \example
     *              STime time( 3 , 17 , 33 );      // now time set to : 0 day 03:17:33
     *              time.toSecond();                // return : 11853
     *              time.toMinute();                // return : 197
     *              time.toHour();                  // return : 3
     */
    int toMiliSecond() const;
    int toSecond() const;
    int toMinute() const;
    int toHour() const;
    int toDay() const;


    /*!
     * convert STime class to QString
     * \example
     *              STime time( 3 , 17 , 33 );      // now time set to : 0 day 03:17:33
     *              time.toString();                // return : "3:17:33"
     *
     *              time.setDay( 2 );
     *              time.toString();                // return : "2 Days 3:17:33"
     *
     *              time.setMiliSecond( 77 );
     *              time.toString();                // return : "2 Days 3:17:33:77"
     */
    QString toString() const;



    /*!
     * Operators Functions
     * \example
     *              STime tA( 3 , 17 , 33 );      // now tA set to : 0 day 03:17:33
     *              STime tB( 7 , 22 , 12 );      // now tB set to : 0 day 07:22:12
     *
     *              tA += tB;                     // now tA set to : 0 day 10:39:45
     *              tA -= tB;                     // now tA set to : 0 day 03:17:33
     *
     *              tB = tB * 2;                  // now tB set to : 0 day 14:44:24
     *
     *              tA > tB;                      // return false
     *              tA < tB;                      // return true
     */
    STime & operator=( STime time );

    STime operator+( STime time );
    STime & operator+=( STime time );
    STime operator-( STime time );
    STime & operator-=( STime time );
    STime operator*( int num );
    STime operator/( int num );


    bool operator==( STime time ) const;
    bool operator!=( STime time ) const;
    bool operator>=( STime time ) const;
    bool operator>( STime time ) const;
    bool operator<=( STime time ) const;
    bool operator<( STime time ) const;

private:
    STimePrivate p;
};

#endif // STIME_H
