#ifndef PERCONF_H
#define PERCONF_H

#include <QApplication>
#include <QList>

#include <sdatabase.h>


class MounterConf : public SDataBase
{
public:
    MounterConf(const QString & path);
    ~MounterConf();

    void setPath(const QString & path);

    QString readOption(const int & type);
    void setOption(const int & type,const QString & stt);
    void setOptionDefault(const int & type);
    QString findHeadStr(const int & type);

    enum type {
               GENERAL
                   ,MASTER_POINT

              ,AFTER
                   ,OPEN_FILE_MANAGER
                   ,CLICK_ACTION

              ,BEFORE

              ,END
          };

    void setup();
    void setupHeads();
private:
    QList<QString> optionStr;
    QList<QString> defaultsStr;
};

#endif // PERCONF_H
