#include "converter.h"
#include "converterpage.h"
#include "ui_extrawidget.h"

#include <QDebug>

#include <SConverter>
#include <SProgressListItem>
#include <Silicon>
#include <SMasterIcons>

class ConverterPrivate
{
public:
    ConverterPage *page;

    SConverter        *converter;
    SProgressListItem *progress;

    Ui::extraWidget *extra_ui;

    bool silent;
    bool wait_stt;
};

Converter::Converter( const SAboutData & parent ) :
    SApplication(parent)
{
    p = new ConverterPrivate;
    p->progress = 0;
    p->silent = false;
    p->wait_stt = false;

    p->converter = new SConverter( this );
    p->page = new ConverterPage( this , p->converter );

    connect( p->page , SIGNAL(go()) , SLOT(start()) );
    connect( p->converter , SIGNAL(finished(int)) , SLOT(finished(int)));
}

void Converter::setWait( bool stt )
{
    p->wait_stt = stt;

    if( p->converter != 0 )
        p->converter->setPassupAndWait( wait() );
}

bool Converter::wait() const
{
    return p->wait_stt;
}

void Converter::start()
{
    if( p->page->isHidden() )
        return;

    p->page->hide();

    progressItem()->setTitle( parent().translatedName() );
    progressItem()->setIcon(  parent().icon() );
    progressItem()->setInfoText( tr("Converting \"%1\" to \"%2\" from \"%3\" type to \"%4\" type")
                                 .arg(p->converter->source())
                                 .arg(p->converter->destination())
                                 .arg(p->converter->sourceType())
                                 .arg(p->converter->destinationType()) );
    progressItem()->show();

    p->extra_ui->itemic_log_list->clear();
    p->extra_ui->log_text->clear();

    p->converter->start();
    Silicon::showProcessess();
}

void Converter::finished( int exit_code )
{
    Q_UNUSED( exit_code )

    if( p->silent )
        quit();
    else
        p->page->show();
}

void Converter::runtimeArgsEvent( const QVariantList & args )
{
    for( int i=0 ; i<args.count() ; i++ )
    {
        QVariant var = args.at(i);
        if( var.type() != QVariant::String )
            return;

        QString str = var.toString();

        if(      str.left(4) == "src=" )
            p->page->setSource( str.mid(4) );

        else if( str.left(4) == "dst=" )
            p->page->setDestination( str.mid(4) );

        else if( str.left(9) == "src-type=" )
            p->page->setSourceType( str.mid(9) );

        else if( str.left(9) == "dst-type=" )
            p->page->setDestinationType( str.mid(9) );

        else if( str.left(5) == "wait=" )
            setWait( str.mid(5).toLower() == "true" );

        else if( str == "-silent" )
            p->silent = true;

        else
            return;
    }

    if( p->silent )
        start();
}

SProgressListItem *Converter::progressItem()
{
    if( !p->progress )
    {
        p->progress = new SProgressListItem( this );
        p->extra_ui = new Ui::extraWidget;
            p->extra_ui->setupUi( p->progress->extraWidget() );
            p->extra_ui->tabWidget->setTabIcon( 0 , SMasterIcons::icon(QSize(48,48),"format-list-unordered.png"));
            p->extra_ui->tabWidget->setTabIcon( 1 , SMasterIcons::icon(QSize(48,48),"document-edit.png"));

        connect( p->converter , SIGNAL(percentChanged(int)) , progressItem() , SLOT(setValue(int)) );
    }

    return p->progress;
}

Converter::~Converter()
{
    delete p;
}
