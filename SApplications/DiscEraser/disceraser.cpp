#include "disceraser.h"
#include "mainpage.h"

#include <Silicon>
#include <SAbstractDiscEraser>
#include <SiDiTools>
#include <SMasterIcons>
#include <SProgressListItem>
#include <SDialogTools>

#include <QFile>
#include <QScrollBar>

class DiscEraserPrivate
{
public:
    MainPage *page;
    SProgressListItem *progress_item;

    SAbstractDiscEraser *disc_eraser;

    int  remaining;
    bool silent;
    bool wait_stt;
};

DiscEraser::DiscEraser( const SAboutData & about )
    : SApplication( about )
{
    p = new DiscEraserPrivate;
    p->page = new MainPage( this );

    p->remaining   = 0;
    p->disc_eraser = 0;
    p->silent      = false;
    p->wait_stt    = false;

    p->progress_item = new SProgressListItem( this );
        p->progress_item->setIcon( parent().icon() );
        p->progress_item->setTitle( parent().name() );
        p->progress_item->hide();

    connect( p->page , SIGNAL(go()) , SLOT(go()) );
    connect( p->progress_item->cancelButton() , SIGNAL(clicked()) , SLOT(showStopDialog()) );
}


void DiscEraser::setWait( bool stt )
{
    p->wait_stt = stt;

    if( p->disc_eraser != 0 )
        p->disc_eraser->setPassupAndWait( wait() );
}

bool DiscEraser::wait() const
{
    return p->wait_stt;
}

void DiscEraser::init_eraser()
{
    if( p->disc_eraser != 0 )
        return ;

    p->disc_eraser = SiDiTools::createDiscEraser( this );
        p->disc_eraser->setPassupAndWait( wait() );

    connect( p->disc_eraser , SIGNAL(percentChanged(int))           , p->progress_item , SLOT(setValue(int)) );
    connect( p->disc_eraser , SIGNAL(finished(int))                 , this             , SLOT(finish())      , Qt::QueuedConnection );
    connect( p->disc_eraser , SIGNAL(goEventSignal(SProcessEvent*)) , this             , SLOT(started())     );
}

void DiscEraser::started()
{
    //p->progress_item->showExtra( true );
    p->progress_item->show();

    Silicon::showProcessess();
}

void DiscEraser::go()
{
    if( p->page->isHidden() )
        return;

    p->page->hide();
    StartErasing();
}

void DiscEraser::stop()
{
    if( p->disc_eraser && p->disc_eraser->isStarted() )
        p->disc_eraser->stop();
    else
        p->progress_item->hide();
}

void DiscEraser::showStopDialog()
{
    SDialogTools::message( 0 , tr("Cancel Process") , tr("Do you realy want to cancel this process?") , QMessageBox::Warning ,
                           this,SLOT(stopDialogButtonClicked(int)),
                           QMessageBox::No|QMessageBox::Yes ,QMessageBox::No );
}

void DiscEraser::stopDialogButtonClicked( int ret )
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

void DiscEraser::StartErasing()
{
    init_eraser();

    const SDeviceItem & destination = p->page->destinationDevice();

    p->progress_item->setInfoText( tr("Erase \"%1\"").arg(destination.name()) );

    p->disc_eraser->setDevice( destination );
    p->disc_eraser->setForce( p->page->force() );
    p->disc_eraser->setLeadOut( p->page->leadOut() );
    p->disc_eraser->start( destination );
}

void DiscEraser::finish()
{
    if( p->page->eject() )
        p->page->destinationDevice().eject();

    if( p->silent )
        quit();
    else
        p->page->show();

    delete p->disc_eraser;
    p->disc_eraser = 0;
}

void DiscEraser::runtimeArgsEvent( const QVariantList & args )
{
    for( int i=0 ; i<args.count() ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        QString str = var.toString();

        if(      str.left(4) == "dev=" )
            p->page->setDestinationDevice( str.mid(4) );

        else if( str == "-eject" )
            p->page->setEject( true );

        else if( str == "-leadout" )
            p->page->setLeadOut( true );

        else if( str == "-force" )
            p->page->setForce( true );

        else if( str == "-silent" )
            p->silent = true;

        else
            return;
    }

    if( p->silent )
        go();
}

void DiscEraser::closeEvent( QCloseEvent *event )
{
    if( p->disc_eraser && p->disc_eraser->isStarted() )
    {
        showStopDialog();
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

DiscEraser::~DiscEraser()
{
    delete p;
}
