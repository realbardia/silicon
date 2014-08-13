#include "hpath.h"

#include <QDir>

Hpath::Hpath()
{
}

Hpath::Hpath(const QString & path)
{
    this->setPath(path);
}

void Hpath::setPath(const QString & path)
{
    this->filePath=path;
}

void Hpath::setFixPath()
{
    QString Address(this->filePath);
    int i=0;
    while(i<Address.size()-1)
    {
        if(Address[i]==Address[i+1] && Address[i]=='/')
        {
            Address.replace(i,2,"/");
            i=-1;
        }

        else if(Address[i]=='.' && Address[i+1]=='/')
        {
            Address.replace(i,2,"");
            i=-1;
        }

        else if(Address[i]=='.' && Address[i+1]=='.' && Address[i+2]=='/' && i>2)
        {
            int j=i-2;

            while(Address[j]!='/' && j>0)
                j--;

            Address.replace(j,i-j+1,"/");
            i=-1;
        }
        i++;
    }

    this->filePath=Address;
    if(this->filePath[this->filePath.size()-1]=='/')
        this->filePath.resize(this->filePath.size()-1);
}

void Hpath::setFixCotation()
{
    int i=0;
    int max=this->filePath.size();
    while(i<max)
    {
        if(this->filePath[i]=='\'')
        {
            this->filePath.replace(i,0,"'\\'");
            max++;
            i+=3;
        }
        i++;
    }
}

void Hpath::setFullpath()
{
    if(this->filePath[0]!='/')
        this->filePath=QDir::currentPath()+"/"+this->filePath;
}

void Hpath::setRemoveFirstSlash()
{
    for(int i=0;this->filePath[i]=='/';i++)
        this->filePath.remove(0,1);
}

void Hpath::setFindName()
{
    this->setRemoveDotIso();
    this->filePath=QDir(this->filePath).dirName();

    for(int i=0;i<this->filePath.size();i++)
        if(this->filePath[i]=='-' || this->filePath[i]=='_')
            this->filePath.replace(i,1," ");

    this->filePath.replace(0,1,this->filePath[0].toUpper());
}

void Hpath::setWrap(const int & size)
{
    if(this->filePath.size()<=size)
        return ;

    this->filePath.resize(size);
    this->filePath.append("...");
}

void Hpath::setSystem()
{
    int i=0;
    while(i<this->filePath.size())
    {
        if(this->filePath[i]==' ')
            this->filePath.replace(i,1,"\\040");
        i++;
    }
}

void Hpath::setFullAct()
{
    this->setFullpath();
    this->setFixPath();
    this->setRemoveDotIso();
}

QString Hpath::path()
{
    return this->filePath;
}

QString Hpath::fullPath()
{
    this->setFullAct();
    return this->filePath;
}

bool Hpath::error()
{
    for(int i=0;i<this->filePath.size();i++)
        if(this->filePath[i].isLetterOrNumber())
            return false;
    return true;
}

void Hpath::setRemoveDotIso()
{
    if( this->filePath.right(4).toLower() == "iso ")
        this->filePath.resize(this->filePath.size()-4);
}
