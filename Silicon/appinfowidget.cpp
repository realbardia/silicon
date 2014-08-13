#include "appinfowidget.h"
#include "sdefaults.h"

#include <SMasterIcons>

AppInfoWidget::AppInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    author_widget = new AuthorListWidget();
    app_picture_widget = new SPictureWidget();

    info_widget = new QWidget();
    ui = new Ui::AppInfoWidget;
        ui->setupUi( info_widget );
        ui->picture_frame->layout()->addWidget( app_picture_widget );
        ui->authors_box->layout()->addWidget( author_widget );

    info_action      = new QAction( SMasterIcons::icon(QSize(48,48),"document-sign.png") , tr("Info") , this );
  //about_action     = new QAction( SMasterIcons::icon(QSize(48,48),"help-about.png") , tr("About") , this );
    about_action     = new QAction( SMasterIcons::icon(QSize(48,48),"utilities-terminal.png") , tr("Commands") , this );
    details_action   = new QAction( SMasterIcons::icon(QSize(48,48),"document-preview.png") , tr("Details") , this );
    authors_action   = new QAction( SMasterIcons::icon(QSize(48,48),"im-user.png") , tr("Authors") , this );
    contact_action   = new QAction( SMasterIcons::icon(QSize(48,48),"mail-send.png") , tr("Contact") , this );
    license_action   = new QAction( SMasterIcons::icon(QSize(48,48),"okteta.png") , tr("License") , this );
    translate_action = new QAction( SMasterIcons::icon(QSize(48,48),"applications-education-language.png") , tr("Translators") , this );

    info_action->setCheckable( true );
    about_action->setCheckable( true );
    details_action->setCheckable( true );
    authors_action->setCheckable( true );
    contact_action->setCheckable( true );
    license_action->setCheckable( true );
    translate_action->setCheckable( true );

    ui->depends_list->topLevelItem(0)->setIcon( 0, SMasterIcons::icon( QSize(48,48) , "application-x-executable.png" ) );

    top_panel_widget = new QToolBar();
        top_panel_widget->addAction( info_action );
        top_panel_widget->addAction( about_action );
        top_panel_widget->addAction( details_action );
        top_panel_widget->addAction( authors_action );
        top_panel_widget->addAction( contact_action );
        top_panel_widget->addAction( license_action );
        top_panel_widget->addAction( translate_action );
        top_panel_widget->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
        top_panel_widget->setIconSize( QSize(32,32) );
        top_panel_widget->setStyleSheet("QToolBar{border-style:solid}");

    base_layout = new QVBoxLayout( this );
        base_layout->addWidget( top_panel_widget );
        base_layout->addWidget( info_widget );
        base_layout->setContentsMargins( 0 , 0 , 0 , 0 );

#ifdef Q_OS_WIN32
    ui->stackedWidget->setAutoFillBackground( true );
    ui->stackedWidget->setFrameShape( QFrame::StyledPanel );
#endif

    QObject::connect( info_action      , SIGNAL(triggered()) , this , SLOT(setInfo()) );
    QObject::connect( about_action     , SIGNAL(triggered()) , this , SLOT(setAbout()) );
    QObject::connect( details_action   , SIGNAL(triggered()) , this , SLOT(setDetails()) );
    QObject::connect( authors_action   , SIGNAL(triggered()) , this , SLOT(setAuthors()) );
    QObject::connect( contact_action   , SIGNAL(triggered()) , this , SLOT(setContact()) );
    QObject::connect( license_action   , SIGNAL(triggered()) , this , SLOT(setLicense()) );
    QObject::connect( translate_action , SIGNAL(triggered()) , this , SLOT(setTranslate()) );

    ui->about_box->hide();
    this->setInfo();
}

void AppInfoWidget::setAboutData( const SAboutData & about )
{
    this->current = about;

    app_picture_widget->drawPixmap( about.pixmap() );

    ui->name_line->setText( about.name() );
    ui->version_line->setText( about.version() );
    ui->organizer_line->setText( about.organizer() );
    ui->copyright_line->setText( about.copyRight() );
    ui->description_text->setText( about.description() );

    ui->about_text->setText( about.about() );
    ui->command_text->setText( about.commandMap() );

    this->setBinaryDepends( about.binaryDepends() );
    this->setSAppDepends( about.sappDepends() );

    ui->translators_table->clearContents();
    ui->translators_table->setRowCount( SAboutData::translators().count() );
    for( int i=0 ; i<SAboutData::translators().count() ; i++ )
    {
        const QHash<SAboutData::TranslatorDatas,QVariant> & hash = SAboutData::translators().at(i);

        QTableWidgetItem *lang = new QTableWidgetItem( hash.value(SAboutData::Language).toString() );
        QTableWidgetItem *mail = new QTableWidgetItem( hash.value(SAboutData::Email).toString() );
        QTableWidgetItem *home = new QTableWidgetItem( hash.value(SAboutData::HomePage).toString() );
        QTableWidgetItem *name = new QTableWidgetItem( hash.value(SAboutData::Name).toString() );
        QTableWidgetItem *icon = new QTableWidgetItem( hash.value(SAboutData::Picture).value<QIcon>() ,
                                                       hash.value(SAboutData::Country).toString() );

        ui->translators_table->setItem( i , 0 , icon );
        ui->translators_table->setItem( i , 1 , lang );
        ui->translators_table->setItem( i , 2 , name );
        ui->translators_table->setItem( i , 3 , home );
        ui->translators_table->setItem( i , 4 , mail );
    }

    ui->type_list->clear();

    for( int i=0 ; i<about.type().count() ; i++ )
    {
        QListWidgetItem *item = new QListWidgetItem();
            item->setText( SDefaults::catGroup(about.type().at(i)) );
            item->setIcon( SDefaults::catIcons(about.type().at(i)) );

        ui->type_list->addItem( item );
    }

    if( about.multiRun() )
        ui->multi_run_line->setText( tr("Yes") );
    else
        ui->multi_run_line->setText( tr("No") );

    author_widget->setAuthors( about.authors() );

    ui->home_page_line->setText( about.homePage() );
    ui->mail_line->setText( about.mail() );

    ui->license_text->setText( about.license() );

    this->setInfo();
}

const SAboutData & AppInfoWidget::currentAboutData()
{
    return this->current;
}

void AppInfoWidget::setBinaryDepends( const QStringList & list )
{
    QTreeWidgetItem *item = ui->depends_list->topLevelItem(0);
    this->clearItem( item );
    this->addChilds( item , list );
}

void AppInfoWidget::setSAppDepends( const QStringList & list )
{
    QTreeWidgetItem *item = ui->depends_list->topLevelItem(1);
    this->clearItem( item );
    this->addChilds( item , list );
}

void AppInfoWidget::addChilds(  QTreeWidgetItem *item , const QStringList & list )
{
    QTreeWidgetItem *temp;

    for( int i=0 ; i<list.count() ; i++ )
    {
        temp = new QTreeWidgetItem( QStringList()<<list.at(i) , QTreeWidgetItem::Type );
        item->addChild( temp );
    }
}

void AppInfoWidget::clearItem( QTreeWidgetItem *item )
{
    QTreeWidgetItem *temp;

    int cnt = item->childCount();
    for( int i=0 ; i<cnt ; i++ )
    {
        temp = item->child(0);
        item->removeChild( temp );
    }
}

void AppInfoWidget::setInfo()
{
    ui->stackedWidget->setCurrentIndex( 0 );

    info_action->setChecked( true );
    about_action->setChecked( false );
    details_action->setChecked( false );
    authors_action->setChecked( false );
    contact_action->setChecked( false );
    license_action->setChecked( false );
    translate_action->setChecked( false );
}

void AppInfoWidget::setAbout()
{
    ui->stackedWidget->setCurrentIndex( 1 );

    info_action->setChecked( false );
    about_action->setChecked( true );
    details_action->setChecked( false );
    authors_action->setChecked( false );
    contact_action->setChecked( false );
    license_action->setChecked( false );
    translate_action->setChecked( false );
}

void AppInfoWidget::setDetails()
{
    ui->stackedWidget->setCurrentIndex( 2 );

    info_action->setChecked( false );
    about_action->setChecked( false );
    details_action->setChecked( true );
    authors_action->setChecked( false );
    contact_action->setChecked( false );
    license_action->setChecked( false );
    translate_action->setChecked( false );
}

void AppInfoWidget::setAuthors()
{
    ui->stackedWidget->setCurrentIndex( 3 );

    info_action->setChecked( false );
    about_action->setChecked( false );
    details_action->setChecked( false );
    authors_action->setChecked( true );
    contact_action->setChecked( false );
    license_action->setChecked( false );
    translate_action->setChecked( false );
}

void AppInfoWidget::setContact()
{
    ui->stackedWidget->setCurrentIndex( 4 );

    info_action->setChecked( false );
    about_action->setChecked( false );
    details_action->setChecked( false );
    authors_action->setChecked( false );
    contact_action->setChecked( true );
    license_action->setChecked( false );
    translate_action->setChecked( false );
}

void AppInfoWidget::setLicense()
{
    ui->stackedWidget->setCurrentIndex( 5 );

    info_action->setChecked( false );
    about_action->setChecked( false );
    details_action->setChecked( false );
    authors_action->setChecked( false );
    contact_action->setChecked( false );
    license_action->setChecked( true );
    translate_action->setChecked( false );
}

void AppInfoWidget::setTranslate()
{
    ui->stackedWidget->setCurrentIndex( 6 );

    info_action->setChecked( false );
    about_action->setChecked( false );
    details_action->setChecked( false );
    authors_action->setChecked( false );
    contact_action->setChecked( false );
    license_action->setChecked( false );
    translate_action->setChecked( true );
}
