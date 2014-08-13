#include "imageburner.h"
#include "mainpage.h"
#include "ui_extrawidget.h"

#include <Silicon>
#include <SAbstractDiscBurner>
#include <SiDiTools>
#include <SMasterIcons>
#include <SProgressListItem>
#include <SDialogTools>

#include <QFile>
#include <QScrollBar>

class ImageBurnerPrivate
{
public:
    MainPage *page;
    SProgressListItem *progress_item;

    SAbstractDiscBurner *disc_burner;

    Ui::extraWidget *extra_ui;
    QScrollBar *log_scrollBar;

    int  remaining;
    bool silent;
    bool wait_stt;
};

ImageBurner::ImageBurner( const SAboutData & about )
    : SApplication( about )
{
    p = new ImageBurnerPrivate;
    p->page = new MainPage( this );

    p->remaining   = 0;
    p->disc_burner = 0;
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


void ImageBurner::setWait( bool stt )
{
    p->wait_stt = stt;

    if( p->disc_burner != 0 )
        p->disc_burner->setPassupAndWait( wait() );
}

bool ImageBurner::wait() const
{
    return p->wait_stt;
}

void ImageBurner::init_burner()
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
                                SLOT(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) );
}

void ImageBurner::started()
{
    //p->progress_item->showExtra( true );
    p->progress_item->show();

    Silicon::showProcessess();
}

void ImageBurner::go()
{
    if( p->page->isHidden() )
        return;

    p->page->hide();
    p->remaining = p->page->copiesNumber();

    startImageToDisc();
}

void ImageBurner::stop()
{
    if( p->disc_burner && p->disc_burner->isStarted() )
        p->disc_burner->stop();
    else
        p->progress_item->hide();
}

void ImageBurner::showStopDialog()
{
    SDialogTools::message( 0 , tr("Cancel Process") , tr("Do you realy want to cancel this process?") , QMessageBox::Warning ,
                           this,SLOT(stopDialogButtonClicked(int)),
                           QMessageBox::No|QMessageBox::Yes ,QMessageBox::No );
}

void ImageBurner::stopDialogButtonClicked( int ret )
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

void ImageBurner::startImageToDisc()
{
    init_burner();

    const QString     & source      = p->page->sourceImage();
    const SDeviceItem & destination = p->page->destinationDevice();

    p->progress_item->setInfoText( tr("Copy \"%1\" to \"%2\"").arg(source, destination.name()) );

    p->disc_burner->setPassupAndWait( wait() );
    p->disc_burner->setImage( source );
    p->disc_burner->setDummy( p->page->dummy() );
    p->disc_burner->setSpeed( p->page->speed() );
    p->disc_burner->start( destination );
}

void ImageBurner::setLog( const QString & str )
{
    p->extra_ui->log_text->setText( str );
    p->log_scrollBar->setValue( p->log_scrollBar->maximum() );
}

void ImageBurner::itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message )
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

void ImageBurner::setSpeed( double value )
{
    p->extra_ui->speed_label->setText( QString::number(value) + " X" );
}

void ImageBurner::setSize( int value )
{
    QString str =  QString::number(value) + "MB - "
                 + QString::number(p->disc_burner->imageSize()) + "MB";
    p->extra_ui->size_label->setText( str );
}

void ImageBurner::setTime( STime elaps )
{
    QString str = p->disc_burner->remainingTime().toString() + " - " + elaps.toString();
    p->extra_ui->time_label->setText( str );
}

void ImageBurner::finish()
{
    if( p->page->eject() )
        p->page->destinationDevice().eject();

    p->remaining--;
    if( p->remaining != 0 )
    {
        startImageToDisc();
        return;
    }

    if( p->page->remove() )
        QFile::remove( p->page->sourceImage() );

    if( p->silent )
        quit();
    else
        p->page->show();
}

void ImageBurner::runtimeArgsEvent( const QVariantList & args )
{
    for( int i=0 ; i<args.count() ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        QString str = var.toString();

        if(      str.left(4) == "dev=" )
            p->page->setDestinationDevice( str.mid(4) );

        else if( str.left(6) == "speed=" )
            p->page->setSpeed( str.mid(6).toInt() );

        else if( str.left(6) == "count=" )
            p->page->setCopiesNumber( str.mid(6).toInt() );

        else if( str.left(5) == "file=" )
            p->page->setSourceImage( str.mid(5) );

        else if( str.left(5) == "scan=" )
            p->page->setScan( str.mid(5) );

        else if( str.left(5) == "wait=" )
            setWait( str.mid(5).toLower() == "true" );

        else if( str == "-eject" )
            p->page->setEject( true );

        else if( str == "-remove" )
            p->page->setRemove( true );

        else if( str == "-dummy" )
            p->page->setDummy( true );

        else if( str == "-silent" )
            p->silent = true;

        else if( str.left(1) == "/" )
            p->page->setSourceImage( str );
        else
            return;
    }

    if( p->silent )
        go();
}

void ImageBurner::closeEvent( QCloseEvent *event )
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

ImageBurner::~ImageBurner()
{
    delete p->extra_ui;
    delete p;
}
