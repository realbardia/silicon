#ifndef MANAGERGUI_H
#define MANAGERGUI_H

#include <SApplication>
#include <SProgressListItem>
#include <QProgressIndicator/QProgressIndicator.h>
#include <SProgressListItem>
#include <SIsoInfo>
#include <STabItem>

#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QFrame>
#include <QIcon>
#include <QSize>
#include <QColor>
#include <QMenu>
#include <QCursor>
#include <QAction>
#include <QFileDialog>
#include <QToolBar>
#include <QPoint>
#include <QTextBrowser>
#include <QQueue>

#include "automount.h"
#include "perconf.h"
#include "itempreview.h"
#include "configure.h"

class SAbstractImageMounter;

class managerGUI : public SPage {
    Q_OBJECT
public:
    managerGUI( const QString & name , SApplication *parent , const QString & confPath , MounterConf *conf = 0 );
    ~managerGUI();

    void setupActions();

public slots:
    void loadList();
    void setConfigure( ConfigureWidget *conf );

    void newManualMount();
    void newAutoMount();

    void unmountAll();
    void mount( const QString & fileName , const QString & mountPoint = NULL );

    void addRuntimeArgs( const QVariantList &args );

private slots:
    void manualMount( const QString & fileName );
    void automaticMount( const QString & fileName );

    void processManualCurrent();
    void processCurrent();
    void itemClick( QListWidgetItem *item );

    void burnImage();
    void showLibrary();
    void catChanged();
    void showContextMenu( const QPoint & point );
    void itemChanged( int row );
    void openCurrentItemLocation();

    void mounted( bool stt );
    void unmounted( bool stt );
    void error( const QString & error );
    void processFinished( int exitCode );
    void reloadConfigs();

    SProgressListItem *processItem();

protected:
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );
    void closeEvent( QCloseEvent *event );

private:
    QVBoxLayout *layout;
    QMenu *menu;
    QMenu *open_with_menu;
    QTextBrowser *logText;
    QHBoxLayout *logLayout;
    QListWidget *list;
    QToolBar *toolbar;
    QStatusBar *status_bar;
    QProgressIndicator *progress_indicator;

    QAction *newMountAct;
    QAction *newManualMountAct;
    QAction *showLibraryAct;
    QAction *unmountAllAct;
    QAction *openFileManagerAct;
    QAction *mountAct;
    QAction *manualMountAct;
    QAction *umountAct;
    QAction *reloadAct;
    QAction *burnAct;

    SIsoInfo *isoinfo;
    SAbstractImageMounter *mounter;
    AutoMount *autoMount;

    STabItem *tab_item;

    ConfigureWidget   *configureUI;
    ItemPreview *item_preview;
    SProgressListItem *process_item;

    MounterConf *personal;

    QString confPath;
    QQueue<QString> queue;

    QString last_mounted_adrs;
    bool open_fm_stt;
    bool click_to_open_stt;
    bool busy_stt;
};

#endif // MANAGERGUI_H
