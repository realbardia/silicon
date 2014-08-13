#include "copydisc.h"
#include "mainpage.h"
#include "ui_extrawidget.h"

#include <Silicon>
#include <SAbstractDiscBurner>
#include <SDialogTools>
#include <SiDiTools>
#include <SMasterIcons>
#include <SProgressListItem>

#include <QDir>
#include <QScrollBar>
#include <QDebug>

class CopyDiscPrivate
{
public:
    MainPage *page;
    SProgressListItem *progress_item;

    SAbstractDiscBurner *disc_burner;

    Ui::extraWidget *extra_ui;
    QScrollBar *log_scrollBar;

    SApplication *image_disc_app;
    SApplication *disc_image_app;

    QString temp;

    int remaining;
    bool wait_stt;
    bool silent;
};

CopyDisc::CopyDisc( const SAboutData & about )
    : SApplication( about )
{
    p = new CopyDiscPrivate;
    p->page = new MainPage( this );

    p->remaining      = 0;
    p->disc_burner    = 0;
    p->wait_stt       = false;
    p->silent         = false;
    p->image_disc_app = 0;
    p->disc_image_app = 0;

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

    connect( p->page                          , SIGNAL(go())      , SLOT(go())             );
    connect( p->progress_item->cancelButton() , SIGNAL(clicked()) , SLOT(showStopDialog()) );
}

void CopyDisc::init_burner()
{
    if( p->disc_burner != 0 )
        return ;

    p->disc_burner = SiDiTools::createDiscBurner( this );

    connect( p->disc_burner , SIGNAL(ringBufferChanged(int))        , p->extra_ui->fifo_progressBar   , SLOT(setValue(int))      );
    connect( p->disc_burner , SIGNAL(bufferChanged(int))            , p->extra_ui->buffer_progressBar , SLOT(setValue(int))      );
    connect( p->disc_burner , SIGNAL(percentChanged(int))           , p->progress_item                , SLOT(setValue(int))      );
    connect( p->disc_burner , SIGNAL(elapsedTimeChanged(STime))     , this                            , SLOT(setTime(STime))     );
    connect( p->disc_burner , SIGNAL(logChanged(QString))           , this                            , SLOT(setLog(QString))    );
    connect( p->disc_burner , SIGNAL(writeSpeedChanged(double))     , this                            , SLOT(setSpeed(double))   );
    connect( p->disc_burner , SIGNAL(writeSizeChenged(int))         , this                            , SLOT(setSize(int))       );
    connect( p->disc_burner , SIGNAL(goEventSignal(SProcessEvent*)) , this                            , SLOT(started())          );
    connect( p->disc_burner , SIGNAL(finished(int))                 , this                            , SLOT(finish())           );
    //connect( p->disc_burner , SIGNAL(error(QString))                , this                            , SLOT(showError(QString)) );
    connect( p->disc_burner , SIGNAL(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) ,
                                 SLOT(itemicLogAdded(SAbstractProcess::ItemicLogType,QString))  );
}

void CopyDisc::started()
{
    //p->progress_item->showExtra( true );
    p->progress_item->show();

    Silicon::showProcessess();
}

bool CopyDisc::wait() const
{
    return p->wait_stt;
}

void CopyDisc::setWait( bool stt )
{
    p->wait_stt = stt;

    if( p->disc_burner != 0 )
        p->disc_burner->setPassupAndWait( wait() );
}

void CopyDisc::go()
{
    if( p->page->isHidden() )
        return;

    p->page->hide();

    if( p->page->onFly() )
        startOnFly();
    else
        startNoOnFly();
}

void CopyDisc::showStopDialog()
{
    SDialogTools::message( 0 , tr("Cancel Process") , tr("Do you realy want to cancel this process?") , QMessageBox::Warning ,
                           this,SLOT(stopDialogButtonClicked(int)),
                           QMessageBox::No|QMessageBox::Yes ,QMessageBox::No );
}

void CopyDisc::stopDialogButtonClicked( int ret )
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

void CopyDisc::stop()
{
    if( p->disc_burner && p->disc_burner->isStarted() )
        p->disc_burner->stop();
    else
        p->progress_item->hide();
}

void CopyDisc::startOnFly()
{
    p->remaining = p->page->copiesNumber();

    init_burner();

    const SDeviceItem & source      = p->page->sourceDevice();
    const SDeviceItem & destination = p->page->destinationDevice();

    p->progress_item->setInfoText( tr("OnFly Copy %1 to %2").arg(source.address(), destination.address()) );

    p->disc_burner->setPassupAndWait( wait() );
    p->disc_burner->setSourceDevice( source );
    p->disc_burner->setDummy( p->page->dummy() );
    p->disc_burner->setSpeed( p->page->speed() );
    p->disc_burner->start( destination );
}

void CopyDisc::startNoOnFly()
{
    DiscToImage();
    ImageToDisc();
}

void CopyDisc::DiscToImage()
{
    SAboutData about;
    QList<SAboutData> apps_list = Silicon::appsList();
    for( int i=0 ; i<apps_list.count() ; i++ )
        if( apps_list[i].name() == "DiscImaging" )
        {
            about = apps_list[i];
            break;
        }

    if( about.name().isEmpty() )
        return ;

    const SDeviceItem & source = p->page->sourceDevice();
    p->temp = Silicon::requestTempFile();

    QVariantList arguments;
        arguments << "dev="  + source.toQString();
        arguments << "file=" + p->temp;
        arguments << "-eject";
        arguments << "-silent";

    p->disc_image_app = Silicon::loadApp( about , arguments );

    connect( p->disc_image_app , SIGNAL(destroyed()) , SLOT(SwitchToBurn()) );
}

void CopyDisc::SwitchToBurn()
{
    p->disc_image_app = 0;
    p->image_disc_app->addRuntimeArgs( QVariantList()<<"wait=false" );
}

void CopyDisc::ImageToDisc()
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

    const SDeviceItem & destination = p->page->destinationDevice();

    QVariantList arguments;
        arguments << "dev="    + destination.toQString();
        arguments << "file="   + p->temp;
        arguments << "speed=" + QString::number( p->page->speed() );
        arguments << "count=" + QString::number( p->page->copiesNumber() );
        arguments << "wait=true";
        arguments << "-silent";

    if( p->page->eject() )  arguments << "-eject";
    if( p->page->dummy() )  arguments << "-dummy";

    p->image_disc_app = Silicon::loadApp( about , arguments );

    connect( p->image_disc_app , SIGNAL(destroyed()) , SLOT(non_onfly_finished()) );
}

void CopyDisc::non_onfly_finished()
{
    p->image_disc_app = 0;
    p->page->show();
}

void CopyDisc::setLog( const QString & str )
{
    p->extra_ui->log_text->setText( str );
    p->log_scrollBar->setValue( p->log_scrollBar->maximum() );
}

void CopyDisc::itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message )
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

void CopyDisc::setSpeed( double value )
{
    p->extra_ui->speed_label->setText( QString::number(value) + " X" );
}

void CopyDisc::setSize( int value )
{
    QString str =  QString::number(value) + "MB - "
                 + QString::number(p->disc_burner->imageSize()) + "MB";
    p->extra_ui->size_label->setText( str );
}

void CopyDisc::setTime( STime elaps )
{
    QString str = p->disc_burner->remainingTime().toString() + " - " + elaps.toString();

    p->extra_ui->time_label->setText( str );
}

void CopyDisc::finish()
{
    if( p->page->eject() )
        p->page->destinationDevice().eject();

    p->remaining--;
    if( p->remaining != 0 )
    {
        startOnFly();
        return;
    }

    if( p->silent )
        quit();
    else
        p->page->show();
}

void CopyDisc::runtimeArgsEvent( const QVariantList & args )
{
    for( int i=0 ; i<args.count() ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        QString str = var.toString();

        if(      str.left(4) == "dst=" )
            p->page->setDestinationDevice( str.mid(4) );

        else if( str.left(4) == "src=" )
            p->page->setSourceDevice( str.mid(4) );

        else if( str.left(6) == "speed=" )
            p->page->setSpeed( str.mid(6).toInt() );

        else if( str.left(6) == "count=" )
            p->page->setCopiesNumber( str.mid(6).toInt() );

        else if( str.left(5) == "scan=" )
            p->page->setScan( str.mid(5) );

        else if( str.left(5) == "wait=" )
            setWait( str.mid(5).toLower() == "true" );

        else if( str.left(6) == "onfly=" )
            p->page->setOnFly( str.mid(6).toLower() == "true" );

        else if( str == "-eject" )
            p->page->setEject( true );

        else if( str == "-dummy" )
            p->page->setDummy( true );

        else if( str == "-silent" )
            p->silent = true;

        else
            return;
    }

    if( p->silent )
        go();
}

void CopyDisc::closeEvent( QCloseEvent *event )
{
    if( p->disc_burner && p->disc_burner->isStarted() )
    {
        showStopDialog();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

CopyDisc::~CopyDisc()
{
    if( p->disc_image_app != 0 )
        p->disc_image_app->quit();
    if( p->image_disc_app != 0 )
        p->image_disc_app->quit();

    delete p->extra_ui;
    delete p;
}
