#include "discimaging.h"
#include "mainpage.h"
#include "ui_extrawidget.h"

#include <Silicon>
#include <SAbstractDiscReader>
#include <SiDiTools>
#include <SMasterIcons>
#include <SProgressListItem>
#include <SDialogTools>

#include <QFile>
#include <QScrollBar>

class DiscImagingPrivate
{
public:
    MainPage *page;
    SProgressListItem *progress_item;

    SAbstractDiscReader *disc_reader;

    Ui::extraWidget *extra_ui;
    QScrollBar *log_scrollBar;

    bool silent;
    bool wait_stt;
};

DiscImaging::DiscImaging( const SAboutData & about )
    : SApplication( about )
{
    p = new DiscImagingPrivate;
    p->page = new MainPage( this );

    p->disc_reader = 0;
    p->silent      = false;
    p->wait_stt    = false;

    p->progress_item = new SProgressListItem( this );
        p->progress_item->setIcon( parent().icon() );
        p->progress_item->setTitle( parent().name() );
        p->progress_item->hide();

    p->extra_ui = new Ui::extraWidget;
        p->extra_ui->setupUi( p->progress_item->extraWidget() );

    p->log_scrollBar = new QScrollBar( Qt::Vertical );
        p->extra_ui->log_text->setVerticalScrollBar( p->log_scrollBar );
        p->extra_ui->tabWidget->setTabIcon( 0 , SMasterIcons::icon(QSize(48,48),"format-list-unordered.png"));
        p->extra_ui->tabWidget->setTabIcon( 1 , SMasterIcons::icon(QSize(48,48),"document-edit.png"));

    connect( p->page , SIGNAL(go()) , SLOT(go()) );
    connect( p->progress_item->cancelButton() , SIGNAL(clicked()) , SLOT(showStopDialog()) );
}


void DiscImaging::setWait( bool stt )
{
    p->wait_stt = stt;

    if( p->disc_reader != 0 )
        p->disc_reader->setPassupAndWait( wait() );
}

bool DiscImaging::wait() const
{
    return p->wait_stt;
}

void DiscImaging::init_reader()
{
    if( p->disc_reader != 0 )
        return ;

    p->disc_reader = SiDiTools::createDiscReader( this );

    connect( p->disc_reader , SIGNAL(logChanged(QString))           , this             , SLOT(setLog(QString))  );
    connect( p->disc_reader , SIGNAL(percentChanged(int))           , p->progress_item , SLOT(setValue(int))    );
    connect( p->disc_reader , SIGNAL(doneSizeChanged(int))          , this             , SLOT(sizeChanged(int)) );
    connect( p->disc_reader , SIGNAL(finished(int))                 , this             , SLOT(finish())         );
    connect( p->disc_reader , SIGNAL(elapsedTimeChanged(STime))     , this             , SLOT(setTime(STime))   );
    connect( p->disc_reader , SIGNAL(goEventSignal(SProcessEvent*)) , this             , SLOT(started())        );
    connect( p->disc_reader , SIGNAL(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) ,
                                SLOT(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) );
}

void DiscImaging::started()
{
    //p->progress_item->showExtra( true );
    p->progress_item->show();

    Silicon::showProcessess();
}

void DiscImaging::go()
{
    if( p->page->isHidden() )
        return ;

    p->page->hide();

    startImageToDisc();
}

void DiscImaging::stop()
{
    if( p->disc_reader && p->disc_reader->isStarted() )
        p->disc_reader->stop();
    else
        p->progress_item->hide();
}

void DiscImaging::showStopDialog()
{
    SDialogTools::message( 0 , tr("Cancel Process") , tr("Do you realy want to cancel this process?") , QMessageBox::Warning ,
                           this,SLOT(stopDialogButtonClicked(int)),
                           QMessageBox::No|QMessageBox::Yes ,QMessageBox::No );
}

void DiscImaging::stopDialogButtonClicked( int ret )
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

void DiscImaging::startImageToDisc()
{
    init_reader();

    const SDeviceItem & source      = p->page->sourceDevice();
    const QString     & destination = p->page->destinationImage();

    p->progress_item->setInfoText( tr("Copy \"%1\" to \"%2\"").arg(source.name(), destination) );

    p->disc_reader->setPassupAndWait( wait() );
    p->disc_reader->setDevice( source );
    p->disc_reader->setPath( destination );
    p->disc_reader->setSectors( p->page->startSector() , p->page->endSector() );

    p->disc_reader->start( source );
}

void DiscImaging::setLog( const QString & str )
{
    p->extra_ui->log_text->setText( str );
    p->log_scrollBar->setValue( p->log_scrollBar->maximum() );
}

void DiscImaging::itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message )
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

void DiscImaging::sizeChanged( int value )
{
    int done_per_mb = (value/1024)*2;
    int size_per_mb = ( ( p->disc_reader->endSector() - p->disc_reader->startSector() ) / 1024 )*2;

    QString str =  QString::number(done_per_mb) + "MB - "
                 + QString::number(size_per_mb) + "MB";

    p->extra_ui->size_label->setText( str );
}

void DiscImaging::setSpeed( double value )
{
    p->extra_ui->speed_label->setText( QString::number(value) + " X" );
}

void DiscImaging::setTime( STime elaps )
{
    QString str = p->disc_reader->remainingTime().toString() + " - " + elaps.toString();
    p->extra_ui->time_label->setText( str );
}

void DiscImaging::finish()
{
    if( p->page->eject() )
        p->page->sourceDevice().eject();

    if( p->page->mount() )
        mount( p->page->destinationImage() );

    if( p->page->library() )
        addToLibrary( p->page->destinationImage() , p->page->libraryName() , p->page->libraryTags().split(",") );

    if( p->silent )
        quit();
    else
        p->page->show();
}

void DiscImaging::mount( const QString & fileName )
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

void DiscImaging::addToLibrary( const QString & path , const QString & name , const QStringList & tags )
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

void DiscImaging::runtimeArgsEvent( const QVariantList & args )
{
    for( int i=0 ; i<args.count() ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        QString str = var.toString();

        if(      str.left(4) == "dev=" )
            p->page->setSourceDevice( str.mid(4) );

        else if( str.left(5) == "file=" )
            p->page->setDestinationImage( str.mid(5) );

        else if( str.left(8) == "sectors=" )
        {
            QString tmp = str.mid(8);
            QStringList list = tmp.split("-");
            if( list.count() != 2 )
                continue;

            p->page->setSectors( list.first().toInt() , list.last().toInt() );
        }

        else if( str.left(5) == "wait=" )
            setWait( str.mid(5).toLower() == "true" );

        else if( str == "-eject" )
            p->page->setEject( true );

        else if( str == "-silent" )
            p->silent = true;

        else
            return;
    }

    if( p->silent )
        go();
}

void DiscImaging::closeEvent( QCloseEvent *event )
{
    if( p->disc_reader && p->disc_reader->isStarted() )
    {
        showStopDialog();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

DiscImaging::~DiscImaging()
{
    delete p->extra_ui;
    delete p;
}
