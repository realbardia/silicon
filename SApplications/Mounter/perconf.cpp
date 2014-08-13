#include "perconf.h"

#include <QDir>

MounterConf::MounterConf(const QString & path)
{
    this->setPath(path);
}

MounterConf::~MounterConf()
{
}

void MounterConf::setPath(const QString & path)
{
    this->setDataBase(path);
    this->setup();
}

void MounterConf::setup()
{
    this->optionStr.clear();
    this->defaultsStr.clear();
    for(int i=0;i<MounterConf::END;i++)
    {
        this->optionStr<<"";
        this->defaultsStr<<"";
    }

    this->optionStr.replace(MounterConf::GENERAL,"General");
    this->optionStr.replace(MounterConf::MASTER_POINT,"Master_Point");

    this->optionStr.replace(MounterConf::AFTER,"After");
    this->optionStr.replace(MounterConf::OPEN_FILE_MANAGER,"Open_FileManager");
    this->optionStr.replace(MounterConf::CLICK_ACTION,"click_action");

    this->optionStr.replace(MounterConf::BEFORE,"Before");


    this->defaultsStr.replace(MounterConf::MASTER_POINT, QDir::homePath());
    this->defaultsStr.replace(MounterConf::OPEN_FILE_MANAGER,"false");
    this->defaultsStr.replace(MounterConf::CLICK_ACTION,"open");


    this->setupHeads();
}

void MounterConf::setupHeads()
{
    this->addHead(this->optionStr.at(MounterConf::GENERAL));
    this->addHead(this->optionStr.at(MounterConf::AFTER));
    this->addHead(this->optionStr.at(MounterConf::BEFORE));
}


void MounterConf::setOptionDefault(const int & type)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=this->optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->addChild(headStr,childStr);

    this->set(headStr,childStr,this->defaultsStr.at(type));
}

void MounterConf::setOption(const int & type,const QString & stt)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=this->optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->addChild(headStr,childStr);

    this->set(headStr,childStr,stt);
}

QString MounterConf::readOption(const int & type)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=this->optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->setOptionDefault(type);

    return this->read(headStr,childStr);
}

QString MounterConf::findHeadStr(const int & type)
{
    if(this->BEFORE<type)
        return this->optionStr.at(this->BEFORE);

    else if(this->AFTER<type)
        return this->optionStr.at(this->AFTER);

    else //if(this->GENERAL<type)
        return this->optionStr.at(this->GENERAL);
}

