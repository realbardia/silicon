#include "configure.h"

#include <SMasterIcons>
#include <QMessageBox>
#include <QStyleFactory>

Configure::Configure( perConf *conf , QWidget *parent) : QWidget( parent )
{
    prc = conf;

    optionBox=new SOptionBox();
        optionBox->setIconSize( QSize(22,22) );
        this->setupItems();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);

    translator = new QTranslator( this );

    layout=new QVBoxLayout(this);
        layout->addWidget(optionBox);
        layout->addWidget(buttonBox);
        layout->setContentsMargins(6,6,6,6);

    connect( buttonBox                      ,SIGNAL(clicked(QAbstractButton*))           ,this ,SLOT(close_slot())                );
    connect( appearUI->themeList            ,SIGNAL(itemDoubleClicked(QListWidgetItem*)) ,this ,SLOT(setTheme(QListWidgetItem*))  );
    connect( appearUI->menuCheckBox         ,SIGNAL(clicked(bool))                       ,this ,SLOT(setMenuAnimation(bool))      );
    connect( appearUI->searchBarCheckBox    ,SIGNAL(clicked(bool))                       ,this ,SLOT(setSearchBarAnimation(bool)) );
    connect( appearUI->searchingCheckBox    ,SIGNAL(clicked(bool))                       ,this ,SLOT(setSearchingAnimation(bool)) );
    connect( appearUI->tabsCheckBox         ,SIGNAL(clicked(bool))                       ,this ,SLOT(setTabBarAnimation(bool))    );
    connect( appearUI->toolbarCheckBox      ,SIGNAL(clicked(bool))                       ,this ,SLOT(setToolBarAnimation(bool))   );
    connect( appearUI->raster_check         ,SIGNAL(toggled(bool))                       ,this ,SLOT(setRaster(bool))             );
    connect( appearUI->native_check         ,SIGNAL(toggled(bool))                       ,this ,SLOT(setNative(bool))             );
    connect( appearUI->opengl_check         ,SIGNAL(toggled(bool))                       ,this ,SLOT(setOpenGl(bool))             );
    connect( appearUI->auto_check           ,SIGNAL(toggled(bool))                       ,this ,SLOT(setAuto(bool))               );
    connect( appearUI->gradiant_back_check  ,SIGNAL(clicked(bool))                       ,this ,SLOT(setGradiantBack(bool))       );
    connect( appearUI->colorized_menu_check ,SIGNAL(clicked(bool))                       ,this ,SLOT(setColorizedMenu(bool))      );
    connect( appearUI->widget_style_combo   ,SIGNAL(currentIndexChanged(QString))        ,this ,SLOT(widgetStyleChanged(QString)) );

    setupLanguages();
}

void Configure::setupItems()
{
    optionBox->add( SMasterIcons::icon( QSize(48,48) , "format-fill-color.png" ) ,tr("Appearance") );

    setupAppearance();
    setupStyle();
    setupColors();
}

bool Configure::stringToBoolian( const QString & str )
{
    bool a = false;
    if( str.toLower() == "true" )
        a = true ;

    return a;
}

QString Configure::boolianToString( bool stt )
{
    QString str = "false" ;
    if( stt )
        str = "true";

    return str;
}

void Configure::setupLanguages()
{
    /*these codes added by Milad Kakoli(Kitten)
    do it for language Supporting*/

    QString langPath;
        langPath = defaults.Share_Path + "/languages";

    QDir dir(langPath);
    QStringList languages = dir.entryList( QStringList("lang-*.qm") );

    for( int i=0 ; i<languages.size() ; i++ )
     {
         QString locale = languages[i];
             locale.truncate( locale.lastIndexOf('.') );
             locale.remove( 0, locale.indexOf('-') + 1 );

         QString lang = QLocale::languageToString(QLocale(locale).language());
         appearUI->languagecomboBox->addItem( lang, langPath + "/" + languages[i] );

         if( lang.toLower() == prc->readOption(perConf::LANGUAGE) )
         {
             appearUI->languagecomboBox->setCurrentIndex( i );
             setLanguage( i );
         }
     }

    connect( appearUI->languagecomboBox , SIGNAL(currentIndexChanged(int)) , SLOT(languageIndexChanged(int)) );

    //------------------------------
}

void Configure::languageIndexChanged( int index )
{
    setLanguage( index );
    resetRequest();
}

void Configure::setLanguage( int id )
{
    if( id < 0 )
        return ;

    QString locale = appearUI->languagecomboBox->itemData( id ).toString();

    /*this Code added by Milad Kakoli(Kitten)
    this Slot Contorls the Language Combo Box change*/

    QApplication::removeTranslator(translator);
    translator->load(locale,"languages");
    QApplication::installTranslator(translator);

    prc->setOption( perConf::LANGUAGE , appearUI->languagecomboBox->currentText().toLower() );
    prc->save();
}

void Configure::setupAppearance()
{
    QPalette palette = QApplication::palette();
        palette.setColor( QPalette::Window , palette.color(QPalette::Base) );

    appearUI=new Ui::Appearance;
        appearUI->setupUi( optionBox->widget( tr("Appearance") ) );
        appearUI->tabWidget->setCurrentIndex(0);
        appearUI->colors_widget->setPalette( palette );

    appearUI->themeList->addItems( QDir(defaults.Personal_Theme_Directory_Path).entryList(QDir::Dirs | QDir::NoDotAndDotDot) );
    appearUI->themeList->addItems( QDir(defaults.Share_Theme_Directory_Path).entryList(QDir::Dirs | QDir::NoDotAndDotDot) );

    appearUI->iconList->addItems( QDir(defaults.Personal_Icons_Directory_Path).entryList(QDir::Dirs | QDir::NoDotAndDotDot) );
    appearUI->iconList->addItems( QDir(defaults.Share_Icons_Directory_Path).entryList(QDir::Dirs | QDir::NoDotAndDotDot) );

    appearUI->menuCheckBox->setChecked( stringToBoolian(prc->readOption(perConf::MENU_ANIMATION)) );
    appearUI->searchBarCheckBox->setChecked( stringToBoolian(prc->readOption(perConf::SEARCHBAR_ANIMATION)) );
    appearUI->searchingCheckBox->setChecked( stringToBoolian(prc->readOption(perConf::SEARCHING_ANIMATION)) );
    appearUI->tabsCheckBox->setChecked( stringToBoolian(prc->readOption(perConf::TABBAR_ANIMATION)) );
    appearUI->toolbarCheckBox->setChecked( stringToBoolian(prc->readOption(perConf::TOOLBAR_ANIMATION)) );

    appearUI->gradiant_back_check->setChecked( stringToBoolian(prc->readOption(perConf::GRADIANT_BACK)) );
    appearUI->colorized_menu_check->setChecked( stringToBoolian(prc->readOption(perConf::COLORIZED_MENU)) );


    QString graphic_system = prc->readOption(perConf::GRAPHICS_SYSTEM);
    if( graphic_system == "raster" )
        appearUI->raster_check->setChecked( true );
    else if( graphic_system == "native" )
        appearUI->native_check->setChecked( true );
    else if( graphic_system == "opengl" )
        appearUI->opengl_check->setChecked( true );
    else
        appearUI->auto_check->setChecked( true );
}

void Configure::setupStyle()
{
    QStringList styles = QStyleFactory::keys();
        styles.prepend( tr("System Settings") );

    appearUI->widget_style_combo->addItems( styles );

    QString current_style_str = prc->readOption( perConf::WIDGET_STYLE );
    int     current_style_row = styles.indexOf( current_style_str );

    if( current_style_row < 0 )
        current_style_row = 0;

    appearUI->widget_style_combo->setCurrentIndex( current_style_row );
    QApplication::setStyle( current_style_str );
}

void Configure::setupColors()
{
    default_palette = QApplication::palette();

    colors_hash.insert( perConf::WINDOW         , new SColorButton( tr("Window Color") )          );
    colors_hash.insert( perConf::WINDOW_TEXT    , new SColorButton( tr("Window Text Color") )     );
    colors_hash.insert( perConf::BASE           , new SColorButton( tr("Base Color") )            );
    colors_hash.insert( perConf::BASE_TEXT      , new SColorButton( tr("Base Text Color") )       );
    colors_hash.insert( perConf::BUTTON_TEXT    , new SColorButton( tr("Button Text Color") )     );
    colors_hash.insert( perConf::BUTTON         , new SColorButton( tr("Button Color") )          );
    colors_hash.insert( perConf::BRIGHT_TEXT    , new SColorButton( tr("Bright Text Color") )     );
    colors_hash.insert( perConf::DARK           , new SColorButton( tr("Dark Color") )            );
    colors_hash.insert( perConf::HIGHLIGHT      , new SColorButton( tr("Highlight Color") )       );
    colors_hash.insert( perConf::HIGHLIGHT_TEXT , new SColorButton( tr("Highlight Text Color") )  );
    colors_hash.insert( perConf::LIGHT          , new SColorButton( tr("Light Color") )           );
    colors_hash.insert( perConf::LINK           , new SColorButton( tr("Link Color") )            );
    colors_hash.insert( perConf::LINK_VISITED   , new SColorButton( tr("Link Visited Color") )    );
    colors_hash.insert( perConf::MID            , new SColorButton( tr("Mid Color") )             );
    colors_hash.insert( perConf::MIDLIGHT       , new SColorButton( tr("Midlight Color") )        );
    colors_hash.insert( perConf::SHADOW         , new SColorButton( tr("Shadow Color") )          );

    colors_role.insert( perConf::WINDOW         , QPalette::Window          );
    colors_role.insert( perConf::WINDOW_TEXT    , QPalette::WindowText      );
    colors_role.insert( perConf::BASE           , QPalette::Base            );
    colors_role.insert( perConf::BASE_TEXT      , QPalette::Text            );
    colors_role.insert( perConf::BUTTON_TEXT    , QPalette::ButtonText      );
    colors_role.insert( perConf::BUTTON         , QPalette::Button          );
    colors_role.insert( perConf::BRIGHT_TEXT    , QPalette::BrightText      );
    colors_role.insert( perConf::DARK           , QPalette::Dark            );
    colors_role.insert( perConf::HIGHLIGHT      , QPalette::Highlight       );
    colors_role.insert( perConf::HIGHLIGHT_TEXT , QPalette::HighlightedText );
    colors_role.insert( perConf::LIGHT          , QPalette::Light           );
    colors_role.insert( perConf::LINK           , QPalette::Link            );
    colors_role.insert( perConf::LINK_VISITED   , QPalette::LinkVisited     );
    colors_role.insert( perConf::MID            , QPalette::Mid             );
    colors_role.insert( perConf::MIDLIGHT       , QPalette::Midlight        );
    colors_role.insert( perConf::SHADOW         , QPalette::Shadow          );


    SColorButton *tmp;
    QList<int> list = colors_hash.keys();
    for( int i=0 ; i<list.count() ; i++ )
    {
        QString color_str = prc->readOption( list.at(i) );

        tmp = colors_hash.value( list.at(i) );
            tmp->setCheckable( true );
            tmp->setChecked( !color_str.isEmpty() );

        if( color_str.isEmpty() )
            tmp->setColor( default_palette.color( colors_role.value(list.at(i)) ) );
        else
        {
            tmp->setColor( QColor(color_str) );
            this->setColors( list.at(i) , color_str );
        }

        appearUI->colors_layout->addWidget( tmp );

        connect( tmp , SIGNAL(colorSelected(SColorButton*)) , SLOT(colorChanged(SColorButton*)) );
    }

    appearUI->colors_layout->addSpacerItem(  new QSpacerItem( 2 , 4 , QSizePolicy::Minimum , QSizePolicy::Expanding ) );
}

void Configure::resetRequest()
{
    QMessageBox msgBox;
        msgBox.setText( tr("This Changes needs to restart Silicon to take effect.") );
        msgBox.setIcon( QMessageBox::Information );
        msgBox.setWindowTitle( tr("Restart Needed") );
        msgBox.exec();
}

void Configure::colorChanged( SColorButton *button )
{
    int type = colors_hash.key( button );

    QString res;
    if( button->isChecked() )
        res = button->color().name();

    prc->setOption( type , res );
    prc->save();

    this->setColors( type , res );
    emit this->themeChanged();
}

void Configure::setColors( int type , const QString & color_str )
{
    QColor color( color_str );
    if( color_str.isEmpty() )
        color = default_palette.color( colors_role.value(type) );

    QPalette palette = QApplication::palette();
        palette.setColor( colors_role.value(type) , color );

    QApplication::setPalette( palette );
}

void Configure::widgetStyleChanged( const QString & str )
{
    QString style = str;
    if( style == tr("System Settings") )
        style.clear();

    QApplication::setStyle( style );

    prc->setOption( perConf::WIDGET_STYLE , style );
    prc->save();
}

void Configure::close_slot()
{
    close();
    emit closed();
}

void Configure::addItem(const QIcon & icon, const QString & name, QWidget *widget)
{
    optionBox->add( icon , name );
    QWidget *parent = optionBox->widget( name );
        parent->setLayout( new QHBoxLayout() );
        parent->layout()->addWidget( widget );
        parent->layout()->setContentsMargins(0,0,0,0);
}

void Configure::setTheme(QListWidgetItem *item)
{
    prc->setOption( perConf::CURRENT_THEME , item->text() );
    prc->save();
    emit this->themeChanged();
}

void Configure::setMenuAnimation(bool stt)
{
    prc->setOption( perConf::MENU_ANIMATION , boolianToString(stt) );
    prc->save();

    emit this->graphicEffectsChanged();
}

void Configure::setSearchBarAnimation(bool stt)
{
    prc->setOption( perConf::SEARCHBAR_ANIMATION , boolianToString(stt) );
    prc->save();

    emit this->graphicEffectsChanged();
}

void Configure::setSearchingAnimation(bool stt)
{
    prc->setOption( perConf::SEARCHING_ANIMATION , boolianToString(stt) );
    prc->save();

    emit this->graphicEffectsChanged();
}

void Configure::setTabBarAnimation(bool stt)
{
    prc->setOption( perConf::TABBAR_ANIMATION , boolianToString(stt) );
    prc->save();

    emit this->graphicEffectsChanged();
}

void Configure::setToolBarAnimation(bool stt)
{
    prc->setOption( perConf::TOOLBAR_ANIMATION , boolianToString(stt) );
    prc->save();

    emit this->graphicEffectsChanged();
}

void Configure::setRaster( bool stt )
{
    if( !stt )
        return ;

    this->setGraphicSystem( "raster" );
}

void Configure::setNative( bool stt )
{
    if( !stt )
        return ;

    this->setGraphicSystem( "native" );
}

void Configure::setOpenGl( bool stt )
{
    if( !stt )
        return ;

    this->setGraphicSystem( "opengl" );
}

void Configure::setAuto( bool stt )
{
    if( !stt )
        return ;

    this->setGraphicSystem( "auto" );
}

void Configure::setGradiantBack( bool stt )
{
    prc->setOption( perConf::GRADIANT_BACK , boolianToString(stt) );
    prc->save();

    emit this->graphicEffectsChanged();
}

void Configure::setColorizedMenu( bool stt )
{
    prc->setOption( perConf::COLORIZED_MENU , boolianToString(stt) );
    prc->save();

    emit this->graphicEffectsChanged();
}

void Configure::setGraphicSystem( const QString & str )
{
    prc->setOption( perConf::GRAPHICS_SYSTEM , str );
    prc->set( "Appearance","graphics_system" , str );
    prc->save();

    resetRequest();
}


/*this function added by Milad Kakoli(Kitten)
  to do Dynamic language changing*/
void Configure::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        appearUI->retranslateUi(this);
        break;
    default:
        break;
    }
}

Configure::~Configure()
{
    delete optionBox;
    delete layout;
}
