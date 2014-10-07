#ifndef SILICONUI_H
#define SILICONUI_H

#include <QMainWindow>
#include <QVariantList>

#include "perconf.h"

class SArgumentList;
class QListWidgetItem;
class SAboutData;
class SPage;
class SApplication;
class SDialog;
class SiliconUIPrivate;

class SiliconUI : public QMainWindow
{
    Q_OBJECT
public:
    SiliconUI( perConf *prcnf , QWidget *parent = 0 );
    ~SiliconUI();

public slots:
    void load( const QString & name     , const QVariantList & args = QVariantList() , void *pack = 0 );
    void load( const SAboutData & about , const QVariantList &args = QVariantList() , void *pack = 0 );

    void runtimeArgRequest( int index , const QVariantList & args );

    void closeAt( int index );

    void aboutQt();
    void aboutSialan();
    void showApplications();
    void showLoadedPlugins();
    void showConfigure();
    void showAbout();
    void showProcesses();

    void writeConf( const QString & head , const QString & child , const QString & value );
    void readConf( const QString & head , const QString & child , QString & result );
    void saveConfs();

    void readMessage( const QString & message );
    void loadArguments( const SArgumentList & list , const QString & current_path = QString() );

    void initializeFinished();

private slots:
    void newApplicationFound( const SAboutData & about );
    void newPageRequest( SPage *page );

    void addPageToTab( SPage *page );
    void addPageToWindow( SPage *page );

    void closePageRequest( SPage *page );
    void pageTypeChanged( SPage *page );
    void pageSettingsChanged( SPage *page );
    void focusOn( SPage *page , bool stt );

    SApplication *loadApp( QListWidgetItem *item , const QVariantList &args = QVariantList() );

    void currentTabChanged( int index );

    void menuShowHide();

    void aboutPageClosed( SPage *page );
    void configurePageClosed( SPage *page );
    void processPageClosed( SPage *page );

    void addToFavorites();
    void favoriteSettingUp( QListWidgetItem *item );
    void previewApp(QListWidgetItem *item);

    void switchPageType();
    void set_current_menupanel();
    void viewModeChanged( QString name );

#ifdef Q_OS_WIN32
signals:
    void winEventSignal( MSG * msg, long * result );

protected:
    bool winEvent( MSG * msg, long * result );
#endif

signals:
    void resized( const QSize & size );

protected:
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
    void closeEvent( QCloseEvent *event );
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );
    bool event( QEvent *event );

private slots:
    void init_ui();
    void init_system();
    void init_actions();
    void init_about_data();
    void init_presonal_confs();
    void init_graphic_effects();
    void init_style_sheet();
    void init_connections();
    void init_last_chance();

private:
    SiliconUIPrivate *p;
};

#endif // SILICONUI_H
