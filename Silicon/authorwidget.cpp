#include "authorwidget.h"

#include <smastericons.h>

AuthorWidget::AuthorWidget(QWidget *parent) :
    QWidget(parent)
{
    ui_widget = new QWidget();
    ui = new Ui::AuthorWidget;
        ui->setupUi( ui_widget );

    basic_info_action = new QAction( SMasterIcons::icon(QSize(48,48),"document-sign.png") , tr("Basic Info") ,this );
        basic_info_action->setCheckable( true );

    contact_action = new QAction( SMasterIcons::icon(QSize(48,48),"mail-send.png") , tr("Contact") , this );
        contact_action->setCheckable( true );

    about_action = new QAction( SMasterIcons::icon(QSize(48,48),"help-about.png") , tr("About") , this );
        about_action->setCheckable( true );

    top_cat_widget = new QToolBar();
        top_cat_widget->addAction( basic_info_action );
        top_cat_widget->addAction( contact_action );
        top_cat_widget->addAction( about_action );
        top_cat_widget->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
        top_cat_widget->setIconSize( QSize(32,32) );
        top_cat_widget->setOrientation( Qt::Vertical );
        top_cat_widget->setStyleSheet("QToolBar{border-style:solid}");

    avatar = new SPictureWidget();

    ui->avatar_frame->layout()->addWidget( avatar );

    base_layout = new QHBoxLayout( this );
        base_layout->addWidget( top_cat_widget );
        base_layout->addWidget( ui_widget );

    QObject::connect( basic_info_action , SIGNAL(triggered()) , this , SLOT(setBasic()) );
    QObject::connect( contact_action , SIGNAL(triggered()) , this , SLOT(setContact()) );
    QObject::connect( about_action , SIGNAL(triggered()) , this , SLOT(setAbout()) );

    this->setBasic();
}

void AuthorWidget::clear()
{
    this->setAuthor( SAuthorInfo() );
}

void AuthorWidget::setAuthor( const SAuthorInfo & author )
{
    avatar->drawPixmap( author.avatar() );
    ui->first_name_line->setText( author.firstName() );
    ui->last_name_line->setText( author.lastName() );
    ui->nick_name_line->setText( author.nickName() );
    ui->born_date_edit->setDate( author.bornDate() );

    ui->rolls_list->clear();
    ui->programing_languages_list->clear();
    ui->projects_list->clear();
    ui->rolls_list->addItems( author.rollList() );
    ui->programing_languages_list->addItems( author.programingLanguagesList() );
    ui->projects_list->addItems( author.projectsList() );


    ui->home_page_line->setText( author.homePage() );
    ui->email_line->setText( author.mail() );
    ui->irc_line->setText( author.irc() );

    ui->gtalk_line->setText( author.gtalkIM() );
    ui->jabber_line->setText( author.jabberIM() );
    ui->skype_line->setText( author.skypeIM() );
    ui->msn_line->setText( author.msnIM() );
    ui->aim_line->setText( author.aimIM() );
    ui->yahoo_line->setText( author.yahooIM() );


    ui->about_text->setText( author.aboutAuthor() );

    current_author = author;

    this->setBasic();
}

const SAuthorInfo & AuthorWidget::currentAuthor()
{
    return this->current_author;
}

void AuthorWidget::setBasic()
{
    ui->stackedWidget->setCurrentIndex( 0 );

    basic_info_action->setChecked( true );
    contact_action->setChecked( false );
    about_action->setChecked( false );
}

void AuthorWidget::setContact()
{
    ui->stackedWidget->setCurrentIndex( 1 );

    basic_info_action->setChecked( false );
    contact_action->setChecked( true );
    about_action->setChecked( false );
}

void AuthorWidget::setAbout()
{
    ui->stackedWidget->setCurrentIndex( 2 );

    basic_info_action->setChecked( false );
    contact_action->setChecked( false );
    about_action->setChecked( true );
}
