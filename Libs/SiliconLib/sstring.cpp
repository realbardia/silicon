#include "sstring.h"

int cutFirstNumber( const QString & str )
{
    for( int i=0 ; i<str.size() ; i++ )
        if( !str.at(i).isNumber() )
            return str.mid( 0 , i ).toInt();

    return 0;
}


SString::SString () {}
SString::SString ( QChar ch ) : QString( ch ) {}
SString::SString ( int size, QChar ch ) : QString( size , ch ) {}
SString::SString ( const QLatin1String & str ) : QString( str ) {}
SString::SString ( const QString & other ) : QString( other ) {}
SString::SString ( const char * str ) : QString( str ) {}
SString::SString ( const QByteArray & ba ) : QString( ba ) {}
SString::~SString() {}

bool SString::operator< ( const QString & other ) const
{
    for( int i=0 ; i<other.size() && i<this->size() ; i++ )
    {
        if( this->at(i).isNumber() && other.at(i).isNumber() )
        {
            int left = cutFirstNumber(this->mid(i));
            int right = cutFirstNumber(other.mid(i));

            if( left > right )
                return false;
            else if( left < right )
                return true;
        }
        else
        {
            if( this->at(i) > other.at(i) )
                return false;
            else if( this->at(i) < other.at(i) )
                return true;
        }
    }

    return false;
}

bool SString::operator< ( const QLatin1String & other ) const
{

}

bool SString::operator< ( const QByteArray & other ) const
{

}

bool SString::operator< ( const char * other ) const
{

}

bool SString::operator<= ( const QString & other ) const
{
    return !this->operator >( other );
}

bool SString::operator<= ( const QLatin1String & other ) const
{

}

bool SString::operator<= ( const QByteArray & other ) const
{

}

bool SString::operator<= ( const char * other ) const
{

}

bool SString::operator> ( const QString & other ) const
{
    for( int i=0 ; i<other.size() && i<this->size() ; i++ )
    {
        if( this->at(i).isNumber() && other.at(i).isNumber() )
        {
            int left = cutFirstNumber(this->mid(i));
            int right = cutFirstNumber(other.mid(i));

            if( left > right )
                return true;
            else if( left < right )
                return false;
        }
        else
        {
            if( this->at(i) > other.at(i) )
                return true;
            else if( this->at(i) < other.at(i) )
                return false;
        }
    }

    return false;
}

bool SString::operator> ( const QLatin1String & other ) const
{

}

bool SString::operator> ( const QByteArray & other ) const
{

}

bool SString::operator> ( const char * other ) const
{

}

bool SString::operator>= ( const QString & other ) const
{
    return !this->operator <( other );
}

bool SString::operator>= ( const QLatin1String & other ) const
{

}

bool SString::operator>= ( const QByteArray & other ) const
{

}

bool SString::operator>= ( const char * other ) const
{

}
