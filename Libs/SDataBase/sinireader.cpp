#include "sinireader.h"

#include <iostream>

SIniReader::SIniReader( const QString & path )
{
    list = new SFileStringList( path , SFileStringList::AUTO_DETECT_OFFSETS );
    this->loadOffsets();
}

void SIniReader::addHead( const QString & head )
{
    int head_index = this->findHead( head );
    if( head_index != -1 )
        return ;

    list->append( "[" + head + "]" );
    this->loadOffsets();
}

void SIniReader::addChild( const QString & head , const QString & child )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return ;

    int child_index = this->findChild( head , child );
    if( child_index != -1 )
        return ;

    list->insert( heads.at(head_index)+1 , child + "=" );
    this->loadOffsets();
}

void SIniReader::set( const QString & head , const QString & child , const QString & str , bool AutoAdd )
{
    if( AutoAdd )
    {
        this->addHead( head );
        this->addChild( head , child );
    }

    int head_index = this->findHead( head );
    if( head_index == -1 )
        return ;

    int child_index = this->findChild( head , child );
    if( child_index == -1 )
        return ;

    list->replace( heads.at(head_index) + child_index + 1 , child + "=" + str );
}

void SIniReader::remove( const QString & head )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return ;

    int child_count = this->childCounts( head );
    for( int i=0 ; i<child_count+1 ; i++ )
        list->remove( heads.at(head_index) );

    this->loadOffsets();
}

void SIniReader::remove( const QString & head , const QString & child )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return ;

    int child_index = this->findChild( head , child );
    if( child_index == -1 )
        return ;

    list->remove( heads.at(head_index) + child_index + 1 );
    this->loadOffsets();
}

void SIniReader::rename( const QString & head , const QString & newHead )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return ;

    list->replace( heads.at( head_index ) , "[" + newHead + "]" );
}

void SIniReader::rename( const QString & head , const QString & child , const QString & newChild )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return ;

    int child_index = this->findChild( head , child );
    if( child_index == -1 )
        return ;

    list->replace( heads.at(head_index) + child_index + 1 , newChild + "=" + this->read( head , child ) );
}

QString SIniReader::head( int index )
{
    QString result( list->at( heads.at(index) ) );
        result.remove( 0 , 1 );
        result.remove( result.size()-1 , 1 );

    return result;
}

QString SIniReader::child( const QString & head , int index )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return QString();

    int row = heads.at(head_index) + index + 1;
    if( row >= list->count() )
        return QString();
    if( heads.count() > head_index+1 && row >= heads.at(  head_index+1 )  )
        return QString();

    QString tmp( list->at(row) );
    if( this->isChild( tmp ) )
        for( int i=0 ; i<tmp.size() ; i++ )
            if( tmp[i] == '=' )
                return tmp.left( i );

    return tmp;
}

QString SIniReader::read( const QString & head , const QString & child )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return QString() ;

    int child_index = this->findChild( head , child );
    if( child_index == -1 )
        return QString() ;

    QString tmp( list->at( heads.at(head_index) + child_index + 1 ) );
        tmp.remove( 0 , child.size()+1 );
    return tmp;
}

int SIniReader::headsCount()
{
    return heads.count();
}

int SIniReader::childCounts( const QString & head )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return -1;

    int cnt = 0;
    for( int i=heads.at(head_index)+1 ; i<list->count() ; i++ )
    {
        QString tmp( list->at(i) );
        if( this->isHead( tmp ) )
            return cnt;

        if( this->isChild( tmp ) )
            cnt++;
    }

    return cnt;
}

void SIniReader::save()
{
    list->save();
}

void SIniReader::reload()
{
    list->reload();
    this->loadOffsets();
}

void SIniReader::loadOffsets()
{
    heads.clear();
    for( int i=0 ; i<list->count() ; i++ )
        if( this->isHead( list->at(i) ) )
            heads << i;
}

bool SIniReader::isHead( const QString & head )
{
    if( head.isEmpty() )
        return false;

    return ( head.left(1) == "[" && head.right(1) == "]" );
}

bool SIniReader::isChild( const QString & child )
{
    if( child.isEmpty() )
        return false;

    return child.contains( "=" );
}

int SIniReader::findHead( const QString & head )
{
    for( int i=0 ; i<heads.count() ; i++ )
    {
        if( list->at( heads.at(i) ) == "[" + head + "]" )
            return i;
    }

    return -1;
}

int SIniReader::findChild( const QString & head , const QString & child )
{
    int head_index = this->findHead( head );
    if( head_index == -1 )
        return -1;

    for( int i=heads.at(head_index)+1 ; i<list->count() ; i++ )
    {
        QString tmp( list->at(i) );
        if( this->isHead( tmp ) )
            return -1;

        if( tmp.left(  child.size()+1 ) == child + "=" )
            return i-heads.at(head_index)-1;
    }

    return -1;
}

SIniReader::~SIniReader()
{
    delete list;
}
