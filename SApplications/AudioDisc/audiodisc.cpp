#include "audiodisc.h"
#include "audioselector.h"
#include "ui_extrawidget.h"

#include <QQueue>
#include <QHash>
#include <QScrollBar>
#include <QFile>

#include <SProgressListItem>
#include <Silicon>
#include <SAbstractAudioBurner>
#include <SiDiTools>
#include <SMasterIcons>
#include <SDialogTools>

typedef QPair<QString,QString> StringPair;

class AudioDiscPrivate
{
public:
    AudioSelector *page;
    Ui::extraWidget *extra_ui;
    QScrollBar *log_scrollBar;

    SProgressListItem *progress_item;

    QQueue<QString> convert_queue;
    QList<StringPair> converted_files_hash;

    SAbstractAudioBurner *burner;

    int remaining;
    bool wait_stt;

    bool percent_smaller_50;
    int track_burned;
};

AudioDisc::AudioDisc( const SAboutData & about )
    : SApplication( about )
{
    p = new AudioDiscPrivate;
        p->progress_item = 0;
        p->burner = 0;
        p->wait_stt = false;

    p->page = new AudioSelector( this );

    connect( p->page , SIGNAL(go()) , SLOT(start()) );
}

void AudioDisc::init_burner()
{
    if( p->burner != 0 )
        return ;

    p->burner = SiDiTools::createAudioBurner( this );
    progressItem();

    connect( p->burner , SIGNAL(percentChanged(int))           , this                            , SLOT(percentChanged(int)) );
    connect( p->burner , SIGNAL(ringBufferChanged(int))        , p->extra_ui->fifo_progressBar   , SLOT(setValue(int))       );
    connect( p->burner , SIGNAL(bufferChanged(int))            , p->extra_ui->buffer_progressBar , SLOT(setValue(int))       );
    connect( p->burner , SIGNAL(elapsedTimeChanged(STime))     , this                            , SLOT(setTime(STime))      );
    connect( p->burner , SIGNAL(logChanged(QString))           , this                            , SLOT(setLog(QString))     );
    connect( p->burner , SIGNAL(writeSpeedChanged(double))     , this                            , SLOT(setSpeed(double))    );
    connect( p->burner , SIGNAL(writeSizeChenged(int))         , this                            , SLOT(setSize(int))        );
    connect( p->burner , SIGNAL(goEventSignal(SProcessEvent*)) , this                            , SLOT(started())           );
    connect( p->burner , SIGNAL(finished(int))                 , this                            , SLOT(finished())          );
    //connect( p->burner , SIGNAL(error(QString))                , this                            , SLOT(showError(QString))  );
    connect( p->burner , SIGNAL(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) ,
                                 SLOT(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) );
}

void AudioDisc::reset()
{
    p->percent_smaller_50 = true;
    p->track_burned = 0;

    progressItem()->setValue( 0 );
    p->extra_ui->fifo_progressBar->setValue( 0 );
    p->extra_ui->buffer_progressBar->setValue( 0 );
    p->extra_ui->log_text->clear();
    p->extra_ui->itemic_log_list->clear();
}

bool AudioDisc::wait() const
{
    return p->wait_stt;
}

void AudioDisc::setWait( bool stt )
{
    p->wait_stt = stt;

    if( p->burner != 0 )
        p->burner->setPassupAndWait( wait() );
}

void AudioDisc::start()
{
    if( p->page->pageType() == SPage::WindowedPage )
        p->page->hide();

    p->remaining = p->page->copiesNumber();
    p->convert_queue.clear();

    const QStringList & files = p->page->files();
    for( int i=0 ; i<files.count() ; i++ )
        p->convert_queue.enqueue( files.at(i) );

    init_burner();
    p->burner->setSpeed( p->page->speed() );

    step_1();
}

void AudioDisc::step_1()
{
    if( p->convert_queue.isEmpty() )
    {
        step_2();
        return;
    }

    const QString & input_mp3  = p->convert_queue.dequeue();
    const QString & output_wav = Silicon::requestTempFile();

    p->converted_files_hash.append( QPair<QString,QString>(input_mp3,output_wav) );

    SApplication *app = converter( input_mp3 , output_wav );

    connect( app , SIGNAL(destroyed()) , SLOT(step_1()) , Qt::QueuedConnection );
}

void AudioDisc::step_2()
{
    reset();

    const SDeviceItem & device = p->page->currentDevice();

    progressItem()->setInfoText( tr("Burning AudioDisc on %1").arg(device.address()) );
    progressItem()->show();

    p->burner->setVolumeID(      p->page->volumeLabel()   );
    p->burner->setCopyRight(     p->page->copyRight()     );
    p->burner->setApplicationID( p->page->applicationId() );
    p->burner->setSystemID(      p->page->systemId()      );
    p->burner->setBiblioGraphic( p->page->biblio()        );
    p->burner->setAbstract(      p->page->abstract()      );
    p->burner->setTrackAtOnce(   p->page->trackAtOnce()   );
    p->burner->setSessionAtOnce( p->page->discAtOnce()    );
    p->burner->setSwab(          p->page->swab()          );

    QStringList values;
    foreach( const StringPair & pair, p->converted_files_hash )
        values << pair.second;

    p->burner->setDevice( device );
    p->burner->setDummy( p->page->dummy() );
    p->burner->setPassupAndWait( wait() );
    p->burner->setFiles( values );
    p->burner->start( device );
}

void AudioDisc::stop()
{
    if( p->burner && p->burner->isStarted() )
        p->burner->stop();
    else
        progressItem()->hide();
}

void AudioDisc::showStopDialog()
{
    SDialogTools::message( 0 , tr("Cancel Process") , tr("Do you realy want to cancel this process?") , QMessageBox::Warning ,
                           this,SLOT(stopDialogButtonClicked(int)),
                           QMessageBox::No|QMessageBox::Yes ,QMessageBox::No );
}

void AudioDisc::stopDialogButtonClicked( int ret )
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

void AudioDisc::finished()
{
    if( p->page->eject() )
        p->page->currentDevice().eject();

    p->remaining--;
    if( p->remaining != 0 )
    {
        step_2();
        return;
    }

    QStringList files;
    foreach( const StringPair & pair, p->converted_files_hash )
        files << pair.second;

    QString errors;
    for( int i=0 ; i<files.count() ; i++ )
    {
        const QString & file = files.at(i);
        if( QFile::remove(file) )
            continue;

        errors += "\n\t" + file;
    }

    if( !errors.isEmpty() )
        SDialogTools::message( 0 , tr("Delete Error") , tr("Can't delete following temp files:%1").arg(errors) , QMessageBox::Critical );

    p->converted_files_hash.clear();

    p->page->setEnabled( true );
    if( p->page->pageType() == SPage::WindowedPage )
        p->page->show();
}

void AudioDisc::setLog( const QString & str )
{
    p->extra_ui->log_text->setText( str );
    p->log_scrollBar->setValue( p->log_scrollBar->maximum() );
}

void AudioDisc::itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message )
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

void AudioDisc::setSpeed( double value )
{
    p->extra_ui->speed_label->setText( QString::number(value) + " X" );
}

void AudioDisc::setSize( int value )
{
    QString str =  QString::number(value) + "MB - ";
    if( p->burner != 0 )
        str += QString::number(p->burner->imageSize()) + "MB";

    p->extra_ui->size_label->setText( str );
}

void AudioDisc::percentChanged( int value )
{
    if( !p->percent_smaller_50 && value < 50 )
    {
        p->percent_smaller_50 = true;
        p->track_burned++;
    }
    else
    if(  p->percent_smaller_50 && value > 50 )
    {
        p->percent_smaller_50 = false;
    }

    qreal single_percent = (qreal)100 / p->converted_files_hash.count();
    int percent = p->track_burned*single_percent + value*single_percent/100;

    progressItem()->setValue( percent );
}

void AudioDisc::setTime( const STime & elaps )
{
    QString str;
    if( p->burner != 0 ) str = p->burner->remainingTime().toString()   + " - " + elaps.toString();
    p->extra_ui->time_label->setText( str );
}

SApplication *AudioDisc::converter( const QString & input_mp3 , const QString & output_wav )
{
    QVariantList arguments;
        arguments << "src=" + input_mp3;
        arguments << "dst=" + output_wav;
        arguments << "src-type=mp3";
        arguments << "dst-type=wav";
        arguments << "-silent";

    SApplication *app = Silicon::loadApp( "Converter" , arguments );

    return app;
}

SProgressListItem *AudioDisc::progressItem()
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

        connect( p->progress_item->cancelButton() , SIGNAL(clicked()) , SLOT(stop())  );
    }

    return p->progress_item;
}

void AudioDisc::runtimeArgsEvent( const QVariantList & args )
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

        else if( str.left(14) == "little-endian=" )
            p->page->setSwab( str.mid(14) == "true" );

        else if( str == "-eject" )
            p->page->setEject( true );

        else if( str == "-tao" )
            p->page->setTrackAtOnce( true );

        else if( str == "-dao" )
            p->page->setDiscAtOnce( true );

        else if( str == "-dummy" )
            p->page->setDummy( true );

        else if( str[0] != '-' )
            p->page->addFiles( QStringList() << str );

        else
            return;
    }
}

void AudioDisc::closeEvent( QCloseEvent *event )
{
    if( p->burner && p->burner->isStarted() )
    {
        showStopDialog();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

AudioDisc::~AudioDisc()
{
    delete p;
}
