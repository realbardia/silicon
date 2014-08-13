#include "audioitem.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <Silicon>
#include <SPictureWidget>
#include <SMasterIcons>

#include <tag.h>
#include <fileref.h>

class AudioItemPrivate
{
public:
    SPictureWidget *pic_widget;

    QLabel *address_des;
    QLabel *title_des;
    QLabel *artist_des;
    QLabel *album_des;

    QLineEdit *address;
    QLineEdit *title;
    QLineEdit *artist;
    QLineEdit *album;

    QHBoxLayout *layout;
    QVBoxLayout *v_layout;
    QHBoxLayout *top_hlayout;
    QHBoxLayout *btn_hlayout;

    QPushButton *play_button;

    QIcon icon;
    QSize icon_size;
};


AudioItem::AudioItem(QWidget *parent) :
    QWidget(parent)
{
    p = new AudioItemPrivate;
        p->icon_size   = QSize(48,48);
        p->icon        = SMasterIcons::icon( QSize(48,48) , "audio-x-generic.png" );

    QFont font = QWidget::font();
        font.setBold( true );

    p->pic_widget = new SPictureWidget();
        p->pic_widget->setFixedSize( p->icon_size );
        p->pic_widget->drawIcon( p->icon , p->icon_size );


    p->address_des = new QLabel( tr("Address:") );
    p->title_des   = new QLabel( tr("Title:"  ) );
    p->artist_des  = new QLabel( tr("Artist:" ) );
    p->album_des   = new QLabel( tr("Album:"  ) );

    p->address_des->setFont(  font );
    p->title_des->setFont(  font );
    p->artist_des->setFont( font );
    p->album_des->setFont(  font );

    p->address = new QLineEdit();
        p->address->setReadOnly( true );
        p->address->setFrame( false );
        p->address->setStyleSheet( "QLineEdit{background-color: transparent ;}" );
        p->address->setSizePolicy( QSizePolicy::MinimumExpanding , QSizePolicy::Fixed );

    p->title = new QLineEdit();
        p->title->setReadOnly( true );
        p->title->setFrame( false );
        p->title->setStyleSheet( "QLineEdit{background-color: transparent ;}" );
        p->title->setSizePolicy(   QSizePolicy::MinimumExpanding , QSizePolicy::Fixed );

    p->artist = new QLineEdit();
        p->artist->setReadOnly( true );
        p->artist->setFrame( false );
        p->artist->setStyleSheet( "QLineEdit{background-color: transparent ;}" );
        p->artist->setSizePolicy(  QSizePolicy::MinimumExpanding , QSizePolicy::Fixed );

    p->album = new QLineEdit();
        p->album->setReadOnly( true );
        p->album->setFrame( false );
        p->album->setStyleSheet( "QLineEdit{background-color: transparent ;}" );
        p->album->setSizePolicy(   QSizePolicy::MinimumExpanding , QSizePolicy::Fixed );


    p->play_button = new QPushButton();
        p->play_button->setIcon( SMasterIcons::icon( QSize(48,48) , "media-playback-start.png" ) );
        p->play_button->setIconSize( QSize(22,22) );
        p->play_button->setFlat( true );

    p->top_hlayout = new QHBoxLayout();
        p->top_hlayout->addWidget( p->title_des );
        p->top_hlayout->addWidget( p->title );
        p->top_hlayout->addWidget( p->artist_des );
        p->top_hlayout->addWidget( p->artist     );
        p->top_hlayout->addWidget( p->album_des  );
        p->top_hlayout->addWidget( p->album      );
        p->top_hlayout->addStretch();
        p->top_hlayout->setSpacing( 17 );
        p->top_hlayout->setContentsMargins( 4 , 4 , 4 , 4 );

    p->btn_hlayout = new QHBoxLayout();
        p->btn_hlayout->addWidget( p->address_des );
        p->btn_hlayout->addWidget( p->address );
        p->btn_hlayout->setSpacing( 17 );
        p->btn_hlayout->setContentsMargins( 4 , 4 , 4 , 4 );

    p->v_layout = new QVBoxLayout();
        p->v_layout->setContentsMargins( 17 , 0 , 0 , 0 );
        p->v_layout->addLayout( p->top_hlayout );
        p->v_layout->addLayout( p->btn_hlayout );

    p->layout   = new QHBoxLayout( this );
        p->layout->addWidget( p->pic_widget  );
        p->layout->addLayout( p->v_layout    );
        p->layout->addWidget( p->play_button );
        p->layout->setContentsMargins( 4 , 4 , 4 , 4 );

    connect( p->play_button , SIGNAL(clicked()) , SLOT(play()) );
}

void AudioItem::play()
{
    Silicon::loadApp( "Tagarg" , QVariantList()<<p->address->text() );
}

bool AudioItem::setAddress( const QString & address )
{
    TagLib::FileRef file( address.toUtf8().constData() );
    TagLib::Tag    *tag = file.tag();
    if( !tag )
        return false;

    p->address->setText( address );

    p->artist->setText( QString::fromUtf8(tag->artist().to8Bit(true).c_str()) );
    p->album->setText(  QString::fromUtf8(tag->album().to8Bit(true).c_str())  );
    p->title->setText(  QString::fromUtf8(tag->title().to8Bit(true).c_str())  );

    return true;
}

QString AudioItem::address() const
{
    return p->address->text();
}

QString AudioItem::title() const
{
    return p->title->text();
}

QString AudioItem::artist() const
{
    return p->artist->text();
}

QString AudioItem::album() const
{
    return p->album->text();
}

void AudioItem::setIcon( const QIcon & icon )
{
    p->icon = icon;
    p->pic_widget->drawIcon( icon , p->icon_size );
}

const QIcon & AudioItem::icon() const
{
    return p->icon;
}

AudioItem::~AudioItem()
{
    delete p;
}
