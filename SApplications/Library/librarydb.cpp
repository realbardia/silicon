#include "librarydb.h"

Librarydb::Librarydb()
{
}

Librarydb::Librarydb(const QString & path)
{
    this->setPath(path);
}

void Librarydb::setPath(const QString & path)
{
    this->setDataBase( path );

    for( int i=0 ; i<Librarydb::END ; i++ )
        options << "" ;

    options.replace( Librarydb::TAGS , "Tags" );
    options.replace( Librarydb::NAME , "Name" );
}

void Librarydb::setOption( const QString & fileName , int type , const QString & str )
{
    if( !this->checkChildExist(fileName,"Tags") )
        this->addChild(fileName,"Tags");

    if( !this->checkChildExist(fileName,"Name") )
        this->addChild(fileName,"Name");

    this->set( fileName , options.at(type) , str );
}

QString Librarydb::readOption( const QString & fileName , int type )
{
    return this->read( fileName , options.at(type) );
}

