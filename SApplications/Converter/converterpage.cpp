#define ICON_SIZE         QSize(48,48)
#define EXPANDED_HEIGHT   373
#define UNEXPANDED_HEIGHT 183

#include "converterpage.h"
#include "converterline.h"
#include "ui_options.h"

#include <SMasterIcons>
#include <SDialogTools>
#include <SDialog>
#include <SConverter>

#include <QLineEdit>
#include <QToolBar>
#include <QAction>
#include <QVBoxLayout>
#include <QToolButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

class ConverterPagePrivate
{
public:
    QToolBar *toolbar;
    QAction  *start_action;
    QAction  *more_action;

    QWidget *spacer_widget;

    QLabel *src_label;
    QLabel *dst_label;

    QLineEdit   *src_line;
    QLineEdit   *dst_line;

    QToolButton *src_button;
    QToolButton *dst_button;

    QHBoxLayout *src_layout;
    QHBoxLayout *dst_layout;

    ConverterLine *cline;
    SConverter    *converter;

    QVBoxLayout *layout;

    Ui::OptionsUi *options_ui;
    QWidget       *options_widget;
};

ConverterPage::ConverterPage( SApplication *parent , SConverter *converter ) :
    SPage( tr("Converter") , parent , SPage::WindowedPage )
{
    p = new ConverterPagePrivate;
    p->converter = converter;

    p->spacer_widget = new QWidget();
        p->spacer_widget->setSizePolicy( QSizePolicy::MinimumExpanding , QSizePolicy::MinimumExpanding );

    p->src_line = new QLineEdit();
        p->src_line->setReadOnly( true );
        p->src_line->setFocusPolicy( Qt::NoFocus );

    p->src_button = new QToolButton();
        p->src_button->setAutoRaise( true );
        p->src_button->setFixedSize( 26 , 26 );
        p->src_button->setIcon( SMasterIcons::icon( ICON_SIZE , "document-open.png" ) );
        p->src_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

    p->src_label = new QLabel( tr("Source:") );
    p->src_layout = new QHBoxLayout();
        p->src_layout->addWidget( p->src_line );
        p->src_layout->addWidget( p->src_button );


    p->dst_line = new QLineEdit();
        p->dst_line->setReadOnly( true );
        p->dst_line->setFocusPolicy( Qt::NoFocus );

    p->dst_button = new QToolButton();
        p->dst_button->setAutoRaise( true );
        p->dst_button->setFixedSize( 26 , 26 );
        p->dst_button->setIcon( SMasterIcons::icon( ICON_SIZE , "document-open.png" ) );
        p->dst_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

    p->dst_label = new QLabel( tr("Destination:") );
    p->dst_layout = new QHBoxLayout();
        p->dst_layout->addWidget( p->dst_line );
        p->dst_layout->addWidget( p->dst_button );


    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setIconSize( QSize(22,22) );

    p->cline = new ConverterLine( p->converter );

    p->options_widget = new QWidget();
    p->options_ui = new Ui::OptionsUi;
        p->options_ui->setupUi( p->options_widget );
        p->options_widget->hide();

    p->layout = new QVBoxLayout( this );
        p->layout->addWidget( p->src_label      );
        p->layout->addLayout( p->src_layout     );
        p->layout->addWidget( p->dst_label      );
        p->layout->addLayout( p->dst_layout     );
        p->layout->addWidget( p->cline          );
        p->layout->addWidget( p->options_widget );
        p->layout->addWidget( p->toolbar        );
        p->layout->setSpacing( 1 );

    init_actions();
    setFixedSize( 373,UNEXPANDED_HEIGHT );

    connect( p->src_button , SIGNAL(clicked()) , SLOT(setSource())      );
    connect( p->dst_button , SIGNAL(clicked()) , SLOT(setDestination()) );
    connect( p->cline      , SIGNAL(updated()) , SLOT(updated())        );

}

void ConverterPage::init_actions()
{
    p->start_action = new QAction( SMasterIcons::icon(QSize(48,48),"media-playback-start.png") , tr("Start") , this );
    p->more_action  = new QAction( SMasterIcons::icon(QSize(48,48),"edit-rename.png")          , tr("More" ) , this );

    p->start_action->setShortcut( Qt::Key_F5 );
    p->start_action->setDisabled( true );

    menuPanel()->addAction( p->start_action );

    p->toolbar->addAction( p->more_action   );
    p->toolbar->addWidget( p->spacer_widget );
    p->toolbar->addAction( p->start_action  );

    connect( p->start_action , SIGNAL(triggered()) , SLOT(start_timer()) );
    connect( p->more_action  , SIGNAL(triggered()) , SLOT(more_prev())   );
}

void ConverterPage::setSource( const QString & source )
{
    if( source.isEmpty() )
    {
        SDialogTools::getOpenFileName( this , this , SLOT(setSource(QString)) , tr("Set Source file") );
        p->start_action->setDisabled( true );
        return;
    }

    p->start_action->setEnabled( !p->dst_line->text().isEmpty() );
    p->src_line->setText( source );
    p->converter->setSource( source , p->cline->sourceType() );
}

void ConverterPage::setDestination( const QString & destination )
{
    if( destination.isEmpty() )
    {
        SDialogTools::getSaveFileName( this , this , SLOT(setDestination(QString)) , tr("Set Destination path") );
        p->start_action->setDisabled( true );
        return;
    }

    p->start_action->setEnabled( !p->src_line->text().isEmpty() );
    p->dst_line->setText( destination );
    p->converter->setDestination( destination , p->cline->destinationType() );
}

void ConverterPage::setSourceType( const QString & type )
{
    p->cline->setSourceType( type );
}

void ConverterPage::setDestinationType( const QString & type )
{
    p->cline->setDestinationType( type );
}

void ConverterPage::start_timer()
{
    if( !isEnabled() )
        return;

    SDialog *dialog = SDialogTools::getTimer( this , tr("Your Request will be starting after count down.") , 7000 );
    connect( dialog , SIGNAL(accepted(bool)) , SLOT(start_prev(bool)) , Qt::QueuedConnection );
}

void ConverterPage::start_prev( bool stt )
{
    if( stt )
        emit go();
}

void ConverterPage::more_prev()
{
    if( height() == UNEXPANDED_HEIGHT )
    {
        setFixedHeight( EXPANDED_HEIGHT );
        p->options_widget->show();
        p->more_action->setText( tr("Less") );
    }
    else
    {
        setFixedHeight( UNEXPANDED_HEIGHT );
        p->options_widget->hide();
        p->more_action->setText( tr("More") );
    }
}

void ConverterPage::updated()
{
    if( !p->src_line->text().isEmpty() )    setSource(      p->src_line->text() );
    if( !p->dst_line->text().isEmpty() )    setDestination( p->dst_line->text() );
}

ConverterPage::~ConverterPage()
{
    delete p;
}
