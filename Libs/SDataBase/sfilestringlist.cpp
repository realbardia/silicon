/*
 * File:      sfilestringlist.cpp
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

#include "sfilestringlist.h"

#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QList>

#include <iostream>



/*================================*
 |                                |
 |     SFileStringListPrivate     |
 |                                |
 *================================*/

class SFileStringListPrivate
{
public:
    QFile *data_file;
    QTextStream *data_stream;

    QList<int> offsets;
    QString file_path;
    QString next_line;
    bool persistant;
    SFileStringList::Mode current_mode;
};



/*================================*
 |                                |
 |        SFileStringList         |
 |                                |
 *================================*/

SFileStringList::SFileStringList( const QString & path , SFileStringList::Mode mode , QString splitter  )
{
    p = new SFileStringListPrivate;

    p->file_path = path;
    p->next_line = splitter;

#ifdef Q_OS_LINUX
    if( p->next_line.isEmpty() )
        p->next_line = "\n";
#else
    if( p->next_line.isEmpty() )
        p->next_line = "\r\n";
#endif

    p->data_file = new QFile( p->file_path );
    p->data_stream = new QTextStream( p->data_file );
    p->data_file->open( QFile::ReadWrite );

    p->current_mode = mode ;
    p->persistant = true;

    this->loadOffsets();
    this->setPersistant();
}

void SFileStringList::loadOffsets()
{
    p->offsets.clear();
    p->offsets << 0;

    int data_size = p->data_file->size();
    bool ok;

    if( p->current_mode == SFileStringList::COMPACT_MODE )
        for( int i=0,j=0 ; i<data_size ; i++ )
        {
            p->data_stream->seek( i );
            if( QString(p->data_stream->read(1)) == "," )
            {
                p->data_stream->seek( j );
                QString str = p->data_stream->read( i-j );
                j += str.toInt( &ok );
                p->offsets << j;

                if( j > i )
                    i=j;
            }
        }
    else if ( p->current_mode == SFileStringList::NORMAL_MODE )
    {
        QFile file( p->file_path + ".fst" );
        file.open( QFile::ReadWrite );

        QStringList list = QString(file.readAll()).split(",",QString::SkipEmptyParts);
        for( int i=0 ; i<list.count() ; i++ )
            p->offsets << p->offsets.last() + list.at(i).toInt( &ok );

        file.close();
    }
    else if ( p->current_mode == SFileStringList::AUTO_DETECT_OFFSETS )
    {
        for( int i=0 ; i<data_size ; i++ )
        {
            p->data_stream->seek( i );
            int read_size = p->next_line.size() + 1000;
            if( i+read_size > data_size )
                read_size = data_size-i;

            QString cache = p->data_stream->read( read_size );
            for( int j=0 ; j<cache.count() ; j++ )
            {
                if( cache.mid(j,p->next_line.size()) == p->next_line )
                {
                    p->offsets << i + j + p->next_line.size() ;
                }
            }

            i+=1000;
        }
        p->offsets << data_size;
    }
}

QString SFileStringList::last()
{
    return this->at( this->count()-1 );
}

QString SFileStringList::first()
{
    return this->at(0);
}

QString SFileStringList::at( int index )
{
    QString result = QString::fromUtf8(this->readRawData( index ).toStdString().c_str());

    if( p->current_mode == SFileStringList::COMPACT_MODE )
    {
        int cnt = result.size();
        for( int i=0 ; i<cnt ; i++ )
        {
            if( result.at(0)==',' )
            {
                result.remove(0,1);
                break;
            }
            if( !result.at(0).isNumber() )
            {
                break;
            }
            result.remove(0,1);
        }
        result.remove( result.size()-1 , 1 );
    }
    else if( p->current_mode == SFileStringList::AUTO_DETECT_OFFSETS )
    {
        result.remove( p->next_line );
    }

    return result;
}

QString SFileStringList::operator[](int index)
{
    return this->at( index );
}

const QString &SFileStringList::operator<<( const QString & str )
{
    this->append( str );
    return str;
}

SFileStringList SFileStringList::operator<<( SFileStringList list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->append( list.at(i) );

    return list;
}

const QStringList &SFileStringList::operator<<( const QStringList & list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->append( list.at(i) );

    return list;
}

void SFileStringList::append( const QString & str )
{
    this->insert( this->count() , str );
}

void SFileStringList::prepend( const QString & str )
{
    this->insert( 0 , str );
}

const QStringList &SFileStringList::mid( int index , int len )
{
    QStringList *list = new QStringList();
    for( int i=index ; i<index+len ; i++ )
        *list << this->at(i);

    return *list;
}

const QStringList & SFileStringList::toQStringList()
{
    return this->mid( 0 , this->count() );
}

void SFileStringList::insert( int index , const QString & str )
{
    QString record = this->strToRecord( str );

    int shift_size = record.toUtf8().count();
    QString temp;
    for( int i=this->count() ; i>index ; i-- )
    {
        temp = this->readRawData(i-1);
        p->offsets[i] += shift_size;

        p->data_file->seek( p->offsets[i-1]+shift_size );
        p->data_file->write( temp.toUtf8() );
    }
    p->offsets.insert( index+1 , p->offsets[index]+shift_size );

    p->data_file->seek( p->offsets[index] );
    p->data_file->write( record.toUtf8() );
}

void SFileStringList::replace( int index , const QString & str )
{
    QString record = this->strToRecord( str );

    int shift_size = record.toUtf8().count() - this->readRawData(index).count();
    QString temp;
    if( shift_size > 0 )
        for( int i=this->count() ; i>index ; i-- )
        {
            temp = this->readRawData(i-1);
            p->offsets[i] += shift_size;

            p->data_file->seek( p->offsets[i-1]+shift_size );
            p->data_file->write( temp.toUtf8() );
        }
    else if( shift_size < 0 )
    {
        for( int i=index+1 ; i<this->count() ; i++ )
        {
            temp = this->readRawData(i);
            p->offsets[i] += shift_size;

            p->data_file->seek( p->offsets[i] );
            p->data_file->write( temp.toUtf8() );
        }
        p->offsets.last() += shift_size;
        p->data_file->resize( p->offsets.last() );
    }

    p->data_file->seek( p->offsets[index] );
    p->data_file->write( record.toUtf8() );
}

void SFileStringList::remove( int index )
{
    this->replace( index , QString() );
    p->offsets.removeAt( index );
}

void SFileStringList::removeLast()
{
    this->remove( this->count()-1 );
}

void SFileStringList::removeFirst()
{
    this->remove(0);
}

void SFileStringList::removeAll()
{
    this->removeFile();
}

void SFileStringList::removeFile()
{
    p->data_file->remove();
    QFile( p->file_path + ".fst" ).remove();

    p->data_file->open( QFile::ReadWrite );
    this->loadOffsets();
}

void SFileStringList::clear()
{
    this->removeFile();
}

const QString & SFileStringList::takeAt( int index )
{
    QString *str = new QString( this->at(index) );
    this->remove( index );
    return *str;
}

const QString & SFileStringList::takeLast()
{
    return this->takeAt( this->count()-1 );
}

const QString & SFileStringList::takeFirst()
{
    return this->takeAt( 0 );
}

void SFileStringList::pop_back()
{
    this->removeLast();
}

void SFileStringList::pop_front()
{
    this->removeFirst();
}

void SFileStringList::push_back( const QString & str )
{
    this->append( str );
}

void SFileStringList::push_front( const QString & str )
{
    this->prepend( str );
}

void SFileStringList::swap( int i , int j )
{
    QString first = this->at(i);
    QString second = this->at(j);

    this->replace( i , second );
    this->replace( j , first );
}

void SFileStringList::move( int i , int j )
{
    this->insert( j , this->takeAt(i) );
}

int SFileStringList::count()
{
    return p->offsets.count()-1;
}

int SFileStringList::size()
{
    return this->count();
}

void SFileStringList::setFieldSplitter( const QString & str )
{
    p->next_line = str;
    this->loadOffsets();
}

QString SFileStringList::fieldSplitter()
{
    return p->next_line;
}

int SFileStringList::lenght()
{
    return this->count();
}

const QString &SFileStringList::path()
{
    return p->file_path;
}

bool SFileStringList::isEmpty()
{
    return ( this->count() == 0 );
}

int SFileStringList::diskSize()
{
    return p->data_file->size();
}

bool SFileStringList::operator==( SFileStringList list )
{
    int list_count = list.count(),
        this_count = this->count();

    if( list_count != this_count )
        return false;

    for( int i=0 ; i<list_count ; i++ )
        if( list.at(i) != this->at(i) )
            return false;

    return true;
}

bool SFileStringList::operator==( const QStringList & list )
{
    int list_count = list.count(),
        this_count = this->count();

    if( list_count != this_count )
        return false;

    for( int i=0 ; i<list_count ; i++ )
        if( list.at(i) != this->at(i) )
            return false;

    return true;
}

const QStringList & SFileStringList::operator+( SFileStringList list )
{
    QStringList *str_list = new QStringList();
    *str_list << this->toQStringList() << list.toQStringList();
    return *str_list;
}

const QStringList & SFileStringList::operator+( const QStringList & list )
{
    QStringList *str_list = new QStringList();
    *str_list << this->toQStringList() << list;
    return *str_list;
}

const SFileStringList & SFileStringList::operator+=( SFileStringList list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->append( list.at(i) );

    return *this;
}

const SFileStringList & SFileStringList::operator+=( const QStringList & list )
{
    for( int i=0 ; i<list.count() ; i++ )
        this->append( list.at(i) );

    return *this;
}

void SFileStringList::save()
{
    p->data_file->close();
    p->data_file->open( QFile::ReadWrite );
}

void SFileStringList::reload()
{
    this->save();
    this->loadOffsets();
}

void SFileStringList::setCodec( QTextCodec *codec )
{
    p->data_stream->setCodec( codec );
}

void SFileStringList::setCodec( const char *codecName )
{
    p->data_file->close();
    p->data_stream->setCodec( codecName );
    p->data_file->open( QFile::ReadWrite );
}

const QTextCodec *SFileStringList::codec()
{
    return p->data_stream->codec();
}

void SFileStringList::setAutoDetectUnicode( bool enabled )
{
    p->data_stream->setAutoDetectUnicode( enabled );
}

bool SFileStringList::autoDetectUnicode()
{
    return p->data_stream->autoDetectUnicode();
}

void SFileStringList::setPersistant( bool state )
{
    p->persistant = state;
}

void SFileStringList::setVolatile( bool state )
{
    p->persistant = !state;
}

void SFileStringList::setMode( SFileStringList::Mode mode )
{
    p->current_mode = mode;
}

SFileStringList::Mode SFileStringList::currentMode()
{
    return p->current_mode;
}

bool SFileStringList::isPersistant()
{
    return p->persistant;
}

bool SFileStringList::isVolatile()
{
    return !p->persistant;
}

QString SFileStringList::strToRecord( const QString & str )
{
    if( p->current_mode == SFileStringList::COMPACT_MODE )
    {
        char str_size[13];
        QString record("0," + str);
        int size;

        do
        {
            size = record.toUtf8().count();
            sprintf( str_size , "%d" , size  );
            record = "," + str + p->next_line;
            record.prepend( str_size );
        }while( size != record.toUtf8().count() );

        return record;
    }
    else if( p->current_mode == SFileStringList::AUTO_DETECT_OFFSETS )
    {
        QString record( QString(str).remove( p->next_line ) + p->next_line );
        if( record == p->next_line )
            record.clear();
        return record;
    }
    else
        return QString(str);
}

const QString & SFileStringList::readRawData( int index )
{
    int fst = p->offsets.at(index);
    int len = p->offsets.at(index+1) - fst;

    p->data_file->seek( fst );
    QString *result = new QString( p->data_file->read(len) );

    return *result;
}

void SFileStringList::saveOffsets()
{
    QFile file( p->file_path + ".fst" );
    file.remove();
    file.open( QFile::ReadWrite );

    char str_size[13];
    QString str;
    for( int i=1 ; i<p->offsets.count() ; i++ )
    {
        sprintf( str_size , "%d" , p->offsets[i] - p->offsets[i-1] );
        str = str + str_size + ",";
    }

    file.write( str.toStdString().c_str() );
    file.close();
}

SFileStringList::~SFileStringList()
{
    p->data_file->close();
    if( !p->persistant )
        this->removeFile();
    if( p->current_mode == SFileStringList::NORMAL_MODE )
        this->saveOffsets();

    delete p->data_file;
    delete p->data_stream;
    delete p;
}
