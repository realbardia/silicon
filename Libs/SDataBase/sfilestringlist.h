/*
 * File:      sfilestringlist.h
 * Author:    Bardia Daneshvar (bardia@getsilicon.org)
 * Copyright: (C) 2010 Bardia Daneshvar
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SFILESTRINGLIST_H
#define SFILESTRINGLIST_H

#include <QString>
#include <QStringList>
#include <string>
#include <QTextCodec>

#include "sdatabase_global.h"

class SFileStringListPrivate;

/*!
 * The SFileStringList class provides a list of string
 * that work on files instead of main memory.
 */
class SDATABASESHARED_EXPORT SFileStringList
{

/*! List of All Available Functions :
 *
 *     enum Mode { COMPACT_MODE , NORMAL_MODE , AUTO_DETECT_OFFSETS };
 *
 * PUBLIC FUNCTIONS:
 *     SFileStringList( const QString & path , Mode mode = SFileStringList::COMPACT_MODE , QString splitter = QString()  );
 *
 *     QString last();
 *     QString first();
 *     QString at( int index );
 *     QString operator[](int index);
 *     const QStringList & mid( int index , int len );
 *     const QStringList & toQStringList();
 *
 *     const QString & operator<<( const QString & str );
 *     SFileStringList operator<<( SFileStringList list );
 *     const QStringList & operator<<( const QStringList & list );
 *     void append( const QString & str );
 *     void prepend( const QString & str );
 *
 *     void insert( int index , const QString & str );
 *     void replace( int index , const QString & str );
 *
 *     void remove( int index );
 *     void removeLast();
 *     void removeFirst();
 *     void removeAll();
 *     void removeFile();
 *     void clear();
 *
 *     const QString & takeAt( int index );
 *     const QString & takeLast();
 *     const QString & takeFirst();
 *
 *     void pop_back();
 *     void pop_front();
 *     void push_back( const QString & str );
 *     void push_front( const QString & str );
 *
 *     void swap( int i , int j );
 *     void move( int i , int j );
 *
 *     int count();
 *     int lenght();
 *     int size();
 *
 *     void setFieldSplitter( const QString & str );
 *     QString fieldSplitter();
 *
 *     const QString &path();
 *     bool isEmpty();
 *     int diskSize();
 *     bool operator==( SFileStringList list );
 *     bool operator==( const QStringList & list );
 *     const QStringList & operator+( SFileStringList list );
 *     const QStringList & operator+( const QStringList & list );
 *     const SFileStringList & operator+=( SFileStringList list );
 *     const SFileStringList & operator+=( const QStringList & list );
 *
 *     void save();
 *     void reload();
 *
 *     void setCodec( QTextCodec *codec );
 *     void setCodec( const char *codecName );
 *     const QTextCodec *codec();
 *     void setAutoDetectUnicode( bool enabled );
 *     bool autoDetectUnicode();
 *     void setPersistant( bool state = true );
 *     void setVolatile( bool state = true );
 *     void setMode( Mode mode );
 *     bool isPersistant();
 *     bool isVolatile();
 *     Mode currentMode();
 *
 *     ~SFileStringList();
 *
 */

public:
    enum Mode {   COMPACT_MODE                  // Write Offsets and Records in one file.
                , NORMAL_MODE                   // Write Offstes and Records in two diffrent files.
                , AUTO_DETECT_OFFSETS           // Offsets auto detect by \n parametr in file.
              };

    /*!
     * Constructs an SFileStringList on "path" path , mode Mode
     * and with "splitter" record splitter.
     */
    SFileStringList( const QString & path , Mode mode = SFileStringList::COMPACT_MODE , QString splitter = QString()  );


    /*!
     * Returns last records of the fileStringList.
     * \sa first
     * \sa at
     */
    QString last();

    /*!
     * Returns first records of the fileStringList.
     * \sa last
     * \sa at
     */
    QString first();

    /*!
     * Returns records at position index on fileStringList.
     * \sa last
     * \sa operator[]
     */
    QString at( int index );

    /*!
     * Returns records at position index on fileStringList.
     * \sa at
     */
    QString operator[](int index);


    /*!
     * Returns a stringList whose elements are copied from this list,
     * starting at position index , with len lenght.
     * \sa toQStringList
     */
    const QStringList & mid( int index , int len );

    /*!
     * Convert FileStringList to the QStringList.
     * \sa mid
     */
    const QStringList & toQStringList();


    /*!
     * Add str string , list FileStringList or list StringList
     * to end of the fileStringList.
     * \sa append
     * \sa prepend
     */
    const QString & operator<<( const QString & str );
    SFileStringList operator<<( SFileStringList list );
    const QStringList & operator<<( const QStringList & list );


    /*!
     * Append str string to the file string list.
     * \sa prepend
     * \example
     *  SFileStringList list("a.txt");
     *       list.append("one");
     *       list.append("two");
     *       list.append("three");
     *
     *  // list: ["one", "two", "three"]
     */
    void append( const QString & str );

    /*!
     * Prepend str string to the file string list.
     * \sa append
     * \example
     *  SFileStringList list("a.txt");
     *       list.prepend("one");
     *       list.prepend("two");
     *       list.prepend("three");
     *
     *  // list: ["three", "two", "one"]
     */
    void prepend( const QString & str );


    /*!
     * Inserts str string on the fileStringList at position index.
     * \sa replace
     * \sa remove
     * \sa operator<<
     * \example
     *  SFileStringList list("a.txt");       // list: ["three", "one"]
     *       list.insert( 1 , "two" );      // list: ["three", "two", "one"]
     */
    void insert( int index , const QString & str );


    /*!
     * Replaces string at position index with str string.
     * \sa insert
     * \sa remove
     * \example
     *  SFileStringList list("a.txt");       // list: ["three", "four", "one"]
     *       list.replace( 1 , "two" );      // list: ["three", "two", "one"]
     */
    void replace( int index , const QString & str );


    /*!
     * Removes record at position index from fileStringList.
     * \sa insert
     * \sa replace
     * \example
     *  SFileStringList list("a.txt");       // list: ["one", "two", "four", "three"]
     *       list.remove( 2 );      // list: ["one", "two", "three"]
     */
    void remove( int index );

    /*!
     * Removes Last Records from the fileStringList.
     * \sa remove
     * \sa removeFirst
     */
    void removeLast();

    /*!
     * Removes First Records from the fileStringList.
     * \sa remove
     * \sa removeLast
     */
    void removeFirst();

    /*!
     * Removes All Records from the fileStringList.
     * This three functions are effectively the same.
     * \sa remove
     */
    void removeAll();
    void removeFile();
    void clear();


    /*!
     * Take Record at position index from the fileStringList and
     * Returns in QString type.
     * \sa takeLast
     */
    const QString & takeAt( int index );

    /*!
     * Take Last Record from the fileStringList and Returns in QString type.
     * \sa takeFirst
     */
    const QString & takeLast();

    /*!
     * Take First Record from the fileStringList and Returns in QString type.
     * \sa takeAt
     */
    const QString & takeFirst();


    /*!
     * This function is provided for STL compatibility.
     * It is equivalent to removeLast(). The list must not be empty. If the
     * list can be empty, call isEmpty() before calling this function.
     * \sa pop_front
     * \sa push_back
     * \sa removeLast
     */
    void pop_back();

    /*!
     * This function is provided for STL compatibility.
     * It is equivalent to removeFirst(). The list must not be empty. If the
     * list can be empty, call isEmpty() before calling this function.
     * \sa pop_back
     * \sa push_front
     * \sa removeFirst
     */
    void pop_front();


    /*!
     * This function is provided for STL compatibility. It is equivalent
     * to append(value).
     * \sa append
     * \sa push_front
     * \sa pop_back
     */
    void push_back( const QString & str );

    /*!
     * This function is provided for STL compatibility. It is equivalent
     * to prepend(value).
     * \sa prepend
     * \sa push_back
     * \sa pop_front
     */
    void push_front( const QString & str );


    /*!
     * Exchange the item at index position i with the item at index position j.
     * \sa move
     * \example
     *  SFileStringList list("a.txt");
     *      list << "A" << "B" << "C" << "D" << "E" << "F";
     *      list.swap(1, 4);
     *
     *  // list: ["A", "E", "C", "D", "B", "F"]
     */
    void swap( int i , int j );

    /*!
     * Moves the item at index position i to index position j.
     * \sa swap
     * \example
     *  SFileStringList list("a.txt");
     *      list << "A" << "B" << "C" << "D" << "E" << "F";
     *      list.move(1, 4);
     *
     *  // list: ["A", "C", "D", "E", "B", "F"]
     */
    void move( int i , int j );

    int count();
    int lenght();
    int size();

    void setFieldSplitter( const QString & str );
    QString fieldSplitter();

    const QString &path();
    bool isEmpty();
    int diskSize();
    bool operator==( SFileStringList list );
    bool operator==( const QStringList & list );
    const QStringList & operator+( SFileStringList list );
    const QStringList & operator+( const QStringList & list );
    const SFileStringList & operator+=( SFileStringList list );
    const SFileStringList & operator+=( const QStringList & list );

    void save();
    void reload();

    void setCodec( QTextCodec *codec );
    void setCodec( const char *codecName );
    const QTextCodec *codec();
    void setAutoDetectUnicode( bool enabled );
    bool autoDetectUnicode();
    void setPersistant( bool state = true );
    void setVolatile( bool state = true );
    void setMode( Mode mode );
    bool isPersistant();
    bool isVolatile();
    Mode currentMode();

    ~SFileStringList();

private:
    SFileStringListPrivate *p;

    void loadOffsets();
    QString strToRecord( const QString & str );
    const QString & readRawData( int index );
    void saveOffsets();
};

#endif // SFILESTRINGLIST_H
