#ifndef SDEFAULTS_H
#define SDEFAULTS_H

#include <QString>
#include <QDir>
#include <sys/stat.h>
#include <QProcess>
#include <QIcon>
#include <QStringList>
#include <QApplication>

#include <SAboutData>

class SDefaults
{
public:
    SDefaults();

    QString Home_Path;
    QString Share_Path;

    QString Personal_Theme_Directory_Path;
    QString Share_Theme_Directory_Path;

    QString Personal_Icons_Directory_Path;
    QString Share_Icons_Directory_Path;

    QString Default_Theme;

    QString Silicon_Libs_Directory;
    QString Silicon_Application_Directory;
    QString Silicon_Plugins_Directory;

    QString Personal_Configs_file;
    QString Share_Configs_file;

    QString Default_Theme_Name;

    QString Desktop_Environment;

    static QIcon catIcons( SAboutData::Type type );
    static QString catGroup( SAboutData::Type type );
private:
    void makeDirectorys();
};

#endif // SDEFAULTS_H
