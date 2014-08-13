#ifndef FILELOADERENGINE_H
#define FILELOADERENGINE_H

#include <QWidget>
#include <QFileInfo>
#include <QList>
#include <QVariantList>

#include <saboutdata.h>

class ApplicationManager;
class FileLoaderEnginePrivate;

class FileLoaderEngine : public QWidget
{
    Q_OBJECT
public:
    FileLoaderEngine( ApplicationManager *app_manager , QWidget *parent = 0 );
    ~FileLoaderEngine();

    void refresh();
    void load();

public slots:
    void open( const QStringList & files );
    void open( const QString & file );
    void open( const QFileInfo & file );

signals:
    void loadAppRequest( SAboutData app , const QVariantList & args );
    void runtimeArgumentsRequest( int index , const QVariantList & args );

private slots:
    void currentRowChanged( int row );
    void upPrefred();
    void downPrefred();
    void save();

private:
    FileLoaderEnginePrivate *p;
};

#endif // FILELOADERENGINE_H
