#ifndef SDATABASE_H
#define SDATABASE_H

#include "sbuffer.h"
#include "sdatabase_global.h"

#include <QString>

class SDATABASESHARED_EXPORT SDataBase
{
public:
    SDataBase();
    SDataBase(const QString & path);
    ~SDataBase();

    int findHead(QString head);
    int findChild(QString head,QString child);

    void setDataBase(const QString & path);
    void setBuffer(const QList<QString> & buffer);
    bool addHead(const QString & Head);
    bool addChild(const QString & head,const QString & child);

    void remove(const QString & head);
    void remove(const QString & head,const QString & child);

    void rename(const QString & head,const QString & name);
    void rename(const QString & head,const QString & child,const QString & name);

    QString head(const unsigned int & num);
    QString child(const QString & head,const unsigned int & num);

    void setReadOnly(bool stt);
    void setReadWrite(bool stt);
    bool isReadOnly();
    bool isReadWrite();

    unsigned int headsNumber();
    unsigned int childsNumber(QString head);

    QString read(const QString & head,const QString & child);
    void set(const QString & head,const QString & child,const QString & newStr);

    bool checkHeadExist(QString item);
    bool checkChildExist(QString head,QString child);

    void open();
    void reset();
    void save();

protected:
    bool isChild(const QString & str);

private:
    QString databasePath;
    long itemsNumber;

    SBuffer buffer;

    void findItemsNumber();
    bool readOnly;
};

#endif // SDATABASE_H
