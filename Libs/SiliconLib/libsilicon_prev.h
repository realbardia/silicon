#ifndef LIBSILICON_PREV_H
#define LIBSILICON_PREV_H

#include <QString>
#include <QObject>
#include <QStringList>
#include <QMainWindow>
#include <QIcon>
#include <QList>
#include <QVariantList>

#include "silicon.h"
#include "libsilicon_global.h"

class SPage;
class SPlugin;
class SApplication;
class SProgressListItem;
class SMenuPanel;
class SDialogMethod;
class SDialog;

/*! \class
 * The SiliconSignalsBox class provides a Silicon Signals Box.
 * Silicon have an object from this class for make connection with
 * sapplications and silicon kernel.
 * This class can't use in the applications and just use in silicon kernel.
 */
class LIBSILICON_EXPORT SiliconSignalsBox : public QObject
{
    Q_OBJECT
public:
    /*! Functions that calls with SApplication , SApplicaions to Silicon Kernel */
    void loadApp( const QString & name , const QVariantList & args , void *pack );
    void sendArgument( int app_tab_index , const QVariantList & args );
    void openFile( const QString & fileName );
    void message( Silicon::MessageType type , const QString & title , const QString & message , const QIcon & icon );
    void showProcessess();


    /*! Functions that calls in Silicon Kernel , Silicon Kernel to SApplications */
    void applicationLoaded( const QString & name );
    void applicationGoingToLoad( const QString & name );


    /*! Functions that calls in Plugins */
    void pluginStarted( SPlugin *plugin );
    void pluginStopped( SPlugin *plugin );

    /*! Private Functions */
    void pageVisiblityChange( SPage *page , bool stt );
    void pageTypeChanged( SPage *page );
    void registerPage( SPage *page );
    void pageSettingsChanged( SPage *page );
    void registerApplication( SApplication *app );
    void deleteApplication( SApplication *app );
    void registerProgressItem( SProgressListItem *item );
    void registerDialog( SDialog *dialog );
    void dialogVisiblityChange( SDialog *dialog , bool stt );
    void registerDialogMethod( SDialogMethod *method );

    void menuPanelUpdated( SMenuPanel *panel );

    void writeConf( const QString & head , const QString & child , const QString & value );
    QString readConf(  const QString & head , const QString & child );
    void saveAllConfs();

    void finishInitializing();

public slots:
    void closeRequest( SPage *page );
    void closeRequest( SApplication *app );

signals:
    void loadAppSignal( const QString & name , const QVariantList & args , void *pack );
    void sendArgumentSignal( int app_tab_index , const QVariantList & args );
    void openFileSignal( const QString & fileName );
    void messageSignal( Silicon::MessageType type , const QString & title , const QString & message , const QIcon & icon );
    void showProcessessSignal();

    void applicationLoadedSignal( const QString & name );
    void applicationGoingToLoadSignal( const QString & name );

    void pluginStartedSignal( SPlugin *plugin );
    void pluginStoppedSignal( SPlugin *plugin );

    void pageVisiblityChangeSignal( SPage *page , bool stt );
    void pageTypeChangedSignal( SPage *page );
    void registerPageSignal( SPage *page );
    void closeRequestSignal( SPage *page );
    void pageSettingsChangedSignal( SPage *page );
    void closeRequestSignal( SApplication *page );
    void registerApplicationSignal( SApplication *app );
    void deleteApplicationSignal( SApplication *app );
    void registerProgressItemSignal( SProgressListItem *item );
    void dialogVisiblityChangeSignal( SDialog *dialog , bool stt );
    void registerDialogSignal( SDialog *dialog );
    void registerDialogMethodSignal( SDialogMethod *method );

    void menuPanelUpdatedSignal( SMenuPanel *panel );

    void writeConfSignal( const QString & head , const QString & child , const QString & value );
    void readConfSignal(  const QString & head , const QString & child , QString & value );
    void saveAllConfsSignal();

    void initializingFinished();
};






/*! \class
 * The SiliconDataBox class provides a Silicon Data Box.
 * Silicon have an object from this class for send datas to
 * sapplications from silicon kernel.
 * This class can't use in the applications and just use in silicon kernel.
 */
class LIBSILICON_EXPORT SiliconDataBox
{
public:
    SiliconDataBox( const QList<SAboutData> & appsList , const QList<SAboutData> & loaded_apps );

    const QList<SAboutData> & apps_list;
    const QList<SAboutData> & loadedApps;
    QString config;
    QString temp;
    QMainWindow *main_window;
    QWidget *main_panel;
    SPage *process_page;
    bool on_initialize;
};






/*! \class
 * The SiliconSet class provides a class that
 * Can set public SiliconSignalsBox and SiliconDataBox for
 * use in Silicon class and sapplications.
 * This class can't use in the applications and just use in silicon kernel.
 */
class LIBSILICON_EXPORT SiliconSet
{
public:
    void setSiliconDataBox( SiliconDataBox *d_box );
    void setSiliconSignalsBox( SiliconSignalsBox *s_box );

    void lock();
    void unlock();

    bool isLocked();
    bool isUnLocked();
    bool accessToChange();
};

#endif // LIBSILICON_PREV_H
