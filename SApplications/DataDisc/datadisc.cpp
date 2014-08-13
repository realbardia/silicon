#include "datadisc.h"
#include "selectfiles.h"
#include "ui_extrawidget.h"

#include <Silicon>
#include <SAbstractDataBurner>
#include <SAbstractImageCreator>
#include <SProgressListItem>
#include <SiDiTools>
#include <SMasterIcons>
#include <SDialogTools>
#include <SDialog>

#include <QScrollBar>
#include <QDebug>

class DataDiscPrivate
{
public:
    SelectFiles *page;
    SAbstractDataBurner   *data_burner;
    SAbstractImageCreator *image_creator;

    SProgressListItem *progress_item;

    Ui::extraWidget *extra_ui;
    QScrollBar *log_scrollBar;

    SApplication *image_disc_app;

    int remaining;
    bool wait_stt;
};

DataDisc::DataDisc( const SAboutData & about )
    : SApplication( about )
{
    p = new DataDiscPrivate;

    p->remaining      = 0;
    p->data_burner    = 0;
    p->image_creator  = 0;
    p->image_disc_app = 0;
    p->wait_stt       = false;

    p->progress_item = 0;
    p->extra_ui      = 0;

    init_burner();
    init_creator();

    p->page = new SelectFiles( this , p->data_burner , p->image_creator );

    connect( p->page , SIGNAL(go()) , SLOT(start()) );
}

void DataDisc::init_burner()
{
    if( p->data_burner != 0 )
        return ;

    p->data_burner = SiDiTools::createDataBurner( this );

    connect( p->data_burner , SIGNAL(percentChanged(int))           , progressItem()                  , SLOT(setValue(int))      );
    connect( p->data_burner , SIGNAL(ringBufferChanged(int))        , p->extra_ui->fifo_progressBar   , SLOT(setValue(int))      );
    connect( p->data_burner , SIGNAL(bufferChanged(int))            , p->extra_ui->buffer_progressBar , SLOT(setValue(int))      );
    connect( p->data_burner , SIGNAL(elapsedTimeChanged(STime))     , this                            , SLOT(setTime(STime))     );
    connect( p->data_burner , SIGNAL(logChanged(QString))           , this                            , SLOT(setLog(QString))    );
    connect( p->data_burner , SIGNAL(writeSpeedChanged(double))     , this                            , SLOT(setSpeed(double))   );
    connect( p->data_burner , SIGNAL(writeSizeChenged(int))         , this                            , SLOT(setSize(int))       );
    connect( p->data_burner , SIGNAL(goEventSignal(SProcessEvent*)) , this                            , SLOT(started())          );
    connect( p->data_burner , SIGNAL(finished(int))                 , this                            , SLOT(finish_burn())      );
    //connect( p->data_burner , SIGNAL(error(QString))                , this                            , SLOT(showError(QString)) );
    connect( p->data_burner , SIGNAL(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) ,
                                 SLOT(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) );
}

void DataDisc::init_creator()
{
    if( p->image_creator != 0 )
        return ;

    p->image_creator = SiDiTools::createImageCreator( this );

    connect( p->image_creator , SIGNAL(percentChanged(int))           , progressItem()                  , SLOT(setValue(int))      );
    connect( p->image_creator , SIGNAL(elapsedTimeChanged(STime))     , this                            , SLOT(setTime(STime))     );
    connect( p->image_creator , SIGNAL(logChanged(QString))           , this                            , SLOT(setLog(QString))    );
    connect( p->image_creator , SIGNAL(goEventSignal(SProcessEvent*)) , this                            , SLOT(started())          );
    connect( p->image_creator , SIGNAL(finished(int))                 , this                            , SLOT(finish_create())    );
    //connect( p->image_creator , SIGNAL(error(QString))                , this                            , SLOT(showError(QString)) );
    connect( p->image_creator , SIGNAL(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) ,
                                 SLOT(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) );
}

SProgressListItem *DataDisc::progressItem()
{
    if( p->progress_item == 0 )
    {
        p->progress_item = new SProgressListItem( this );
            p->progress_item->setIcon( parent().icon() );
            p->progress_item->setTitle( parent().name() );
            p->progress_item->hide();

        p->extra_ui = new Ui::extraWidget;
            p->extra_ui->setupUi( p->progress_item->extraWidget() );
            p->extra_ui->tabWidget->setTabIcon( 0 , SMasterIcons::icon(QSize(48,48),"format-list-unordered.png"));
            p->extra_ui->tabWidget->setTabIcon( 1 , SMasterIcons::icon(QSize(48,48),"document-edit.png"));

        p->log_scrollBar = new QScrollBar( Qt::Vertical );
            p->extra_ui->log_text->setVerticalScrollBar( p->log_scrollBar );

        connect( p->progress_item->cancelButton() , SIGNAL(clicked()) , SLOT(showStopDialog()) );
    }

    return p->progress_item;
}

void DataDisc::reset()
{
    progressItem()->setValue( 0 );
    p->extra_ui->fifo_progressBar->setValue( 0 );
    p->extra_ui->buffer_progressBar->setValue( 0 );
    p->extra_ui->log_text->clear();
    p->extra_ui->itemic_log_list->clear();
}

void DataDisc::started()
{
    //progressItem()->showExtra( true );
    progressItem()->show();

    Silicon::showProcessess();
}

bool DataDisc::wait() const
{
    return p->wait_stt;
}

void DataDisc::setWait( bool stt )
{
    p->wait_stt = stt;

    if( p->data_burner != 0 )
        p->data_burner->setPassupAndWait( wait() );
}

void DataDisc::start()
{
    if( p->page->pageType() == SPage::WindowedPage )
        p->page->hide();

    switch( static_cast<int>(p->page->type()) )
    {
    case SelectFiles::OnFlyDataDisc :
        startOnFly();
        break;

    case SelectFiles::NoOnFlyDataDisc :
        startNoOnFly();
        break;

    case SelectFiles::DataIso :
        startImaging();
        break;
    }
}

void DataDisc::showStopDialog()
{
    SDialogTools::message( 0 , tr("Cancel Process") , tr("Do you realy want to cancel this process?") , QMessageBox::Warning ,
                           this,SLOT(stopDialogButtonClicked(int)),
                           QMessageBox::No|QMessageBox::Yes ,QMessageBox::No );
}

void DataDisc::stopDialogButtonClicked( int ret )
{
    switch( ret )
    {
    case QMessageBox::Yes:
        stop();
        break;

    case QMessageBox::No:
        break;
    }
}

void DataDisc::stop()
{
    if( p->data_burner && p->data_burner->isStarted() )
        p->data_burner->stop();
    else if( p->image_creator && p->image_creator->isStarted() )
        p->image_creator->stop();
    else
        progressItem()->hide();
}

void DataDisc::startOnFly()
{
    p->remaining = p->page->copiesNumber();

    init_burner();
    reset();

    const SDeviceItem & device = p->page->currentDevice();

    progressItem()->setInfoText( tr("Burning OnFly DataDisc on %1").arg(device.address()) );

    p->data_burner->setVolumeID(      p->page->volumeLabel()   );
    p->data_burner->setCopyRight(     p->page->copyRight()     );
    p->data_burner->setApplicationID( p->page->applicationId() );
    p->data_burner->setSystemID(      p->page->systemId()      );
    p->data_burner->setBiblioGraphic( p->page->biblio()        );
    p->data_burner->setAbstract(      p->page->abstract()      );

    p->data_burner->setPassupAndWait( wait() );
    p->data_burner->setDevice( device );
    p->data_burner->setPathSpec( p->page->pathSpec() );
    p->data_burner->setDummy( p->page->dummy() );
    p->data_burner->setSpeed( p->page->speed() );
    p->data_burner->start( device );
}

void DataDisc::startNoOnFly()
{
    startImaging();
    ImageToDisc();
}

void DataDisc::startImaging()
{
    init_creator();
    reset();

    const QStringList & paths = p->page->pathSpec();

    progressItem()->setInfoText( tr("Start Make Disc Image to \"%1\"").arg( p->page->output() ) );

    p->image_creator->setPassupAndWait( wait() );
    p->image_creator->setVolumeID(      p->page->volumeLabel()   );
    p->image_creator->setCopyRight(     p->page->copyRight()     );
    p->image_creator->setApplicationID( p->page->applicationId() );
    p->image_creator->setSystemID(      p->page->systemId()      );
    p->image_creator->setBiblioGraphic( p->page->biblio()        );
    p->image_creator->setAbstract(      p->page->abstract()      );
    p->image_creator->setPublisher(     p->page->publisher()     );

    p->image_creator->setPathSpec( paths );
    p->image_creator->start( p->page->output() );
}

void DataDisc::setLog( const QString & str )
{
    p->extra_ui->log_text->setText( str );
    p->log_scrollBar->setValue( p->log_scrollBar->maximum() );
}

void DataDisc::itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message )
{
    QListWidgetItem *item = new QListWidgetItem();
        item->setText( message );

    switch( static_cast<int>(type) )
    {
    case SAbstractProcess::Information :
        item->setIcon( SMasterIcons::icon( QSize(48,48) , "dialog-information.png" ) );
        break;
    case SAbstractProcess::Warning :
        item->setIcon( SMasterIcons::icon( QSize(48,48) , "dialog-warning.png" ) );
        break;
    case SAbstractProcess::Fatal :
        item->setIcon( SMasterIcons::icon( QSize(48,48) , "dialog-error.png" ) );
        break;
    case SAbstractProcess::Critical :
        item->setIcon( SMasterIcons::icon( QSize(48,48) , "dialog-warning.png" ) );
        break;
    }

    p->extra_ui->itemic_log_list->addItem( item );
    p->extra_ui->itemic_log_list->scrollToBottom();
}

void DataDisc::setSpeed( double value )
{
    p->extra_ui->speed_label->setText( QString::number(value) + " X" );
}

void DataDisc::setSize( int value )
{
    QString str =  QString::number(value) + "MB - ";
    if( p->data_burner != 0 )
        str += QString::number(p->data_burner->imageSize()) + "MB";
    else if( p->image_creator != 0 )
        str += QString::number(p->image_creator->imageSize()) + "MB";

    p->extra_ui->size_label->setText( str );
}

void DataDisc::setTime( const STime & elaps )
{
    QString str;
    if( p->data_burner != 0 )        str = p->data_burner->remainingTime().toString()   + " - " + elaps.toString();
    else if( p->image_creator != 0 ) str = p->image_creator->remainingTime().toString() + " - " + elaps.toString();

    p->extra_ui->time_label->setText( str );
}

void DataDisc::finish_burn()
{
    if( p->page->eject() )
        p->page->currentDevice().eject();

    if( p->page->type() == SelectFiles::NoOnFlyDataDisc )
    {
        p->page->setEnabled( true );
        if( p->page->pageType() == SPage::WindowedPage )
            p->page->show();

        return;
    }

    p->remaining--;
    if( p->remaining != 0 )
    {
        startOnFly();
        return;
    }

    p->page->setEnabled( true );
    if( p->page->pageType() == SPage::WindowedPage )
        p->page->show();
}

void DataDisc::finish_create()
{
    if( p->page->mount() )
        mount( p->page->output() );

    if( p->page->library() )
        addToLibrary( p->page->output() , p->page->libraryName() , p->page->libraryTags().split(",") );

    if( p->page->type() == SelectFiles::NoOnFlyDataDisc )
    {
        p->image_disc_app->addRuntimeArgs( QVariantList()<<"wait=false" );
        return;
    }

    p->page->setEnabled( true );
    if( p->page->pageType() == SPage::WindowedPage )
        p->page->show();
}

void DataDisc::ImageToDisc()
{
    SAboutData about;
    QList<SAboutData> apps_list = Silicon::appsList();
    for( int i=0 ; i<apps_list.count() ; i++ )
        if( apps_list[i].name() == "ImageBurner" )
        {
            about = apps_list[i];
            break;
        }

    if( about.name().isEmpty() )
        return ;

    const SDeviceItem & destination = p->page->currentDevice();
    const QString     & image       = p->page->output();

    QVariantList arguments;
        arguments << "dev="    + destination.toQString();
        arguments << "file="   + image;
        arguments << "speed="  + QString::number( p->page->speed() );
        arguments << "count="  + QString::number( p->page->copiesNumber() );
        arguments << "wait=true";
        arguments << "-silent";

    if( p->page->eject() )  arguments << "-eject";
    if( p->page->dummy() )  arguments << "-dummy";

    p->image_disc_app = Silicon::loadApp( about , arguments );

    connect( p->image_disc_app , SIGNAL(destroyed()) , SLOT(finish_burn()) , Qt::QueuedConnection );
}

void DataDisc::mount( const QString & fileName )
{
    SAboutData about;
    QList<SAboutData> apps_list = Silicon::appsList();
    for( int i=0 ; i<apps_list.count() ; i++ )
        if( apps_list[i].name() == "Mounter" )
        {
            about = apps_list[i];
            break;
        }

    if( about.name().isEmpty() )
        return ;

    int item_index = -1;
    QList<SAboutData> loaded_list = Silicon::loadedApps();
    for( int i=0 ; i<loaded_list.count() ; i++ )
        if( loaded_list[i].name() == "Mounter" )
        {
            item_index = i;
            break;
        }

    if( item_index == -1 )
        Silicon::loadApp( about , QVariantList()<< fileName );
    else
        Silicon::sendArgument( item_index , QVariantList()<< fileName );
}

void DataDisc::addToLibrary( const QString & path , const QString & name , const QStringList & tags )
{
    QVariantList sending_arg;
        sending_arg << "path=" + path;
        sending_arg << "name=" + name;
        sending_arg << "tags=" + tags.join(",");

    SAboutData about;
    QList<SAboutData> apps_list = Silicon::appsList();
    for( int i=0 ; i<apps_list.count() ; i++ )
        if( apps_list[i].name() == "Library" )
        {
            about = apps_list[i];
            break;
        }

    if( about.name().isEmpty() )
        return ;

    int item_index = -1;
    QList<SAboutData> loaded_list = Silicon::loadedApps();
    for( int i=0 ; i<loaded_list.count() ; i++ )
        if( loaded_list[i].name() == "Library" )
        {
            item_index = i;
            break;
        }

    if( item_index == -1 )
        Silicon::loadApp( about , sending_arg );
    else
        Silicon::sendArgument( item_index , sending_arg );
}

void DataDisc::runtimeArgsEvent( const QVariantList & args )
{
    for( int i=0 ; i<args.count() ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        QString str = var.toString();
        if( str.isEmpty() )
            return;

        if(      str.left(4) == "dev=" )
            p->page->setDevice( str.mid(4) );

        else if( str.left(6) == "speed=" )
            p->page->setSpeed( str.mid(6).toInt() );

        else if( str.left(6) == "count=" )
            p->page->setCopiesNumber( str.mid(6).toInt() );

        else if( str.left(5) == "scan=" )
            p->page->setScan( str.mid(5) );

        else if( str.left(5) == "wait=" )
            setWait( str.mid(5).toLower() == "true" );

        else if( str.left(6) == "onfly=" )
            p->page->setOnFly( str.mid(6) == "true" );

        else if( str.left(7) == "volume=" )
            p->page->setVolumeLabel( str.mid(7) );

        else if( str.left(7) == "app_id=" )
            p->page->setApplicationId( str.mid(7) );

        else if( str.left(7) == "biblio=" )
            p->page->setBiblio( str.mid(7) );

        else if( str.left(9) == "abstract=" )
            p->page->setAbstract( str.mid(9) );

        else if( str.left(10) == "copyright=" )
            p->page->setCopyRight( str.mid(10) );

        else if( str.left(10) == "system_id=" )
            p->page->setSystemId( str.mid(10) );

        else if( str.left(10) == "publisher=" )
            p->page->setPublisher( str.mid(10) );

        else if( str == "-eject" )
            p->page->setEject( true );

        else if( str == "-dummy" )
            p->page->setDummy( true );

        else if( str[0] != '-' )
            p->page->add( str );

        else
            return;
    }
}

void DataDisc::closeEvent( QCloseEvent *event )
{
    if( p->data_burner && p->data_burner->isStarted() )
    {
        showStopDialog();
        event->ignore();
    }
    else if( p->image_creator && p->image_creator->isStarted() )
    {
        showStopDialog();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

DataDisc::~DataDisc()
{
    if( p->image_disc_app != 0 )
        p->image_disc_app->quit();

    if( p->extra_ui )
        delete p->extra_ui;

    delete p;
}
