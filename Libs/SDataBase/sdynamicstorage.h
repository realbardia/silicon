#ifndef SDYNAMICSTORAGE_H
#define SDYNAMICSTORAGE_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QStringList>

#include "sdatabase_global.h"

struct SDChest
{
    SDChest(int chestNum,const QString & fileAdrs,const QString & pass){ chestNumber=chestNum; fileAddress=fileAdrs; password=pass; }
    SDChest(int chestNum,const QString & pass){ chestNumber=chestNum; password=pass; }

    int chestNumber;
    QString fileAddress;
    QString password;
    QString data;
};

class SDATABASESHARED_EXPORT SDynamicStorage : public QObject
{
    Q_OBJECT
public:
    SDynamicStorage(QWidget *parent = 0);
    ~SDynamicStorage();

    int registerChest(const QString & file,const QString & pass);
    int registerChest(const QString & pass);
    void unRegisterChest(int chestNumber,const QString & pass);

    void updateAll();
    void update(int chestNumber);

    QString chestData(int chestNumber,const QString & pass);

protected:
    int chestIndex(int chestNumber);
    int getAddress();
    bool checkPassword(int chestIndex,const QString & pass);

    QList<SDChest *> &list();

private:
};

#endif // SDYNAMICSTORAGE_H
