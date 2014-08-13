#include "fileloaderengine.h"
#include "ui_fileloaderengine.h"
#include "applicationmanager.h"

#include <QHash>
#include <QFile>
#include <QMessageBox>
#include <smastericons.h>

#include "sdefaults.h"

struct SuffixItem
{
    QList<SAboutData> list;
};

class FileLoaderEnginePrivate
{
public:
    QHash<QString,SuffixItem> suffix_hash;

    Ui::FileLoaderEngine *ui;
    SDefaults defaults;

    ApplicationManager *app_manager;

    void appendStringList( const QString & suffix , const QStringList & list );
};

void FileLoaderEnginePrivate::appendStringList( const QString & suffix , const QStringList & list )
{
    for( int i=0 ; i<list.count() ; i++ )
    {
        QString str = list.at(i);
        SAboutData about;
        int founded = false;
        for( int j=0 ; j<app_manager->applicationsList().count() ; j++ )
            if( app_manager->applicationsList().at(j).name() == str )
            {
                about = app_manager->applicationsList().at(j);
                founded = true;
                break;
            }

        if( !founded )
            continue;

        SuffixItem item = this->suffix_hash.value( suffix );
        if( item.list.isEmpty() )
            this->suffix_hash.insert( suffix ,item );

        if( !this->suffix_hash[suffix].list.contains( about ) )
            this->suffix_hash[suffix].list.append( about );
    }
}



FileLoaderEngine::FileLoaderEngine( ApplicationManager *app_manager , QWidget *parent )
    : QWidget( parent )

{
    p = new FileLoaderEnginePrivate;
        p->app_manager = app_manager;

    p->ui = new Ui::FileLoaderEngine;
    p->ui->setupUi( this );

    p->ui->upButton->setIcon( SMasterIcons::icon( QSize(48,48) , "arrow-up.png" ) );
    p->ui->downButton->setIcon( SMasterIcons::icon( QSize(48,48) , "arrow-down.png" ) );

    QObject::connect( p->ui->typeListWidget , SIGNAL(currentRowChanged(int)) , SLOT(currentRowChanged(int)) );
    QObject::connect( p->ui->upButton , SIGNAL(clicked()) , SLOT(upPrefred()) );
    QObject::connect( p->ui->downButton , SIGNAL(clicked()) , SLOT(downPrefred()) );
}

void FileLoaderEngine::refresh()
{
    p->suffix_hash.clear();
    this->load();

    QStringList list;
    for( int i=0 ; i<p->app_manager->applicationsList().count() ; i++ )
    {
        list = p->app_manager->applicationsList()[i].compatibleFileTypes();
        for( int j=0 ; j<list.count() ; j++ )
        {
            QString current = list.at(j).toLower();
            SuffixItem item = p->suffix_hash.value( current );
            if( item.list.isEmpty() )
                p->suffix_hash.insert( current ,item );

            if( !p->suffix_hash[current].list.contains( p->app_manager->applicationsList().at(i) ) )
                p->suffix_hash[current].list.prepend( p->app_manager->applicationsList().at(i) );
        }
    }

    list = p->suffix_hash.keys();
        list.sort();

    for( int i=1 ; i<list.count() ; i++ )
        if( list.at(i-1) == list.at(i) )
        {
            i--;
            list.removeAt( i );
        }

    p->ui->typeListWidget->clear();
    p->ui->typeListWidget->addItems( list );
}

void FileLoaderEngine::open( const QStringList & files )
{
    for( int i=0 ; i<files.count() ; i++ )
        open( files.at(i) );
}

void FileLoaderEngine::open( const QString & file )
{
    open( QFileInfo(file) );
}

void FileLoaderEngine::open( const QFileInfo & file )
{
    SuffixItem item = p->suffix_hash.value( file.suffix().toLower() );
    if( item.list.isEmpty() )
    {
        QString error;
            error = "<b>" + tr("Application Not Found") + "</b><br />" +
                    tr("Silicon can't find any Application to open \"%1\"").arg( file.filePath() );

        QMessageBox message_box( this );
            message_box.setStandardButtons( QMessageBox::Ok );
            message_box.setWindowTitle( tr("Silicon Message") );
            message_box.setText( error );
            message_box.setIconPixmap( SMasterIcons::icon( QSize(48,48), "dialog-information.png" ).pixmap(QSize(48,48)) );
            message_box.exec();

        return ;
    }

    SAboutData app = item.list.first();
    int index = p->app_manager->loadedApplicationsAbouts().indexOf( app );

    QVariantList arguments;
        arguments << file.absoluteFilePath();

    if( app.multiRun() || index == -1 )
    {
        emit loadAppRequest( app , arguments );
    }
    else
    {
        emit runtimeArgumentsRequest( index , arguments );
        p->app_manager->loadedApplications().at(index)->focus();
    }
}

void FileLoaderEngine::currentRowChanged( int row )
{
    if( row == -1 )
        return;

    QString type = p->ui->typeListWidget->currentItem()->text();
    QList<SAboutData> list = p->suffix_hash.value( type ).list;

    p->ui->appsListWidget->clear();
    for( int i=0 ; i<list.count() ; i++ )
        p->ui->appsListWidget->addItem( new QListWidgetItem( list[i].icon(), list[i].name()) );
}

void FileLoaderEngine::upPrefred()
{
    int row = p->ui->appsListWidget->currentRow();
    if( row < 1 )
        return ;

    QListWidgetItem *item = p->ui->appsListWidget->takeItem(row);
    p->ui->appsListWidget->insertItem( row-1 , item );
    p->ui->appsListWidget->setCurrentRow( row-1 );

    QString suffix = p->ui->typeListWidget->currentItem()->text();
    p->suffix_hash[suffix].list.swap( row , row-1 );

    this->save();
}

void FileLoaderEngine::downPrefred()
{
    int row = p->ui->appsListWidget->currentRow();
    int count = p->ui->appsListWidget->count();

    if( row < 0 || row >= count-1 )
        return ;

    QListWidgetItem *item = p->ui->appsListWidget->takeItem(row);
    p->ui->appsListWidget->insertItem( row+1 , item );
    p->ui->appsListWidget->setCurrentRow( row+1 );

    QString suffix = p->ui->typeListWidget->currentItem()->text();
    p->suffix_hash[suffix].list.swap( row , row+1 );

    this->save();
}

void FileLoaderEngine::load()
{
    QFile file( p->defaults.Home_Path + "/file_associations" );
        file.open( QFile::ReadOnly );

    QStringList list = QString(file.readAll()).split( "\n" , QString::SkipEmptyParts );

    file.close();

    for( int i=0 ; i<list.count() ; i++ )
    {
        QString str = list.at(i);
        QString suffix;
        for( int j=0 ; j<str.count() ; j++ )
            if( str[j] == '=' )
            {
                suffix = str.mid(0,j);
                str.remove(0,j+1);
                break;
            }

        p->appendStringList( suffix , str.split("," , QString::SkipEmptyParts) );
    }
}

void FileLoaderEngine::save()
{
    QString suffix = p->ui->typeListWidget->currentItem()->text();
    QStringList types;
    for( int i=0 ; i<p->ui->appsListWidget->count() ; i++ )
        types << p->ui->appsListWidget->item(i)->text();


    QFile file( p->defaults.Home_Path + "/file_associations" );
    file.open( QFile::ReadWrite );

    QStringList list = QString(file.readAll()).split( "\n" , QString::SkipEmptyParts );

    bool ok = false;
    for( int i=0 ; i<list.count() ; i++ )
        if( list.at(i).mid(0, suffix.count()+1) == suffix + "=" )
        {
            list.replace( i , suffix + "=" + types.join(",") );
            ok = true;
        }

    if( !ok )
        list.append( suffix + "=" + types.join(",") );

    file.resize(0);
    file.reset();
    file.write( list.join("\n").toUtf8() );
    file.close();
}

FileLoaderEngine::~FileLoaderEngine()
{
    delete p;
}
