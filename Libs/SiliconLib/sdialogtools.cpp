#include "sdialogtools.h"
#include "sdialog.h"
#include "sgraphictimer.h"

#include <QFileInfo>
#include <QInputDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QDateTime>

SDialog *SDialogTools::getColor(  QObject *receiver, const char *member )
{
    return 0;
}

SDialog *SDialogTools::getLineEdit(  SPage * parent
                                , QObject *receiver, const char *member
                                , const QString & title
                                , const QString & label
                                , QLineEdit::EchoMode mode
                                , const QString & text )
{


    QInputDialog *input_dialog = new QInputDialog( 0 , Qt::Widget );
        input_dialog->setLabelText( label );
        input_dialog->setTextEchoMode( mode );
        input_dialog->setTextValue( text );

    SDialog *dialog = new SDialog( parent , 400 , 113 );
        dialog->setWindowTitle( title );

    QHBoxLayout *layout = new QHBoxLayout( dialog );
        layout->addWidget( input_dialog );

    QObject::connect( input_dialog , SIGNAL(accepted())                 , dialog   , SLOT(accept()) );
    QObject::connect( input_dialog , SIGNAL(rejected())                 , dialog   , SLOT(reject()) );
    QObject::connect( input_dialog , SIGNAL(textValueSelected(QString)) , receiver , member         );

    return dialog;
}


SDialog *SDialogTools::getOpenFileNames( SPage * parent
                                     , QObject *receiver, const char *member
                                     , const QString & caption
                                     , const QString & dir
                                     , const QString & filter)
{


    QFileDialog *file_dialog = new QFileDialog( 0 , caption , dir , filter );
        file_dialog->setFileMode( QFileDialog::ExistingFiles );
        file_dialog->setOption( QFileDialog::DontUseNativeDialog , true );
        file_dialog->setWindowFlags( Qt::Widget );

    SDialog *dialog = new SDialog( parent , 573 , 313 );
    QHBoxLayout *layout = new QHBoxLayout( dialog );
        layout->addWidget( file_dialog );

    QObject::connect( file_dialog , SIGNAL(accepted())                 , dialog   , SLOT(accept()) );
    QObject::connect( file_dialog , SIGNAL(rejected())                 , dialog   , SLOT(reject()) );
    QObject::connect( file_dialog , SIGNAL(filesSelected(QStringList)) , receiver , member         );

    return dialog;
}


SDialog *SDialogTools::getOpenFileName( SPage * parent
                                     , QObject *receiver, const char *member
                                     , const QString & caption
                                     , const QString & dir
                                     , const QString & filter)
{


    QFileDialog *file_dialog = new QFileDialog( 0 , caption , dir , filter );
        file_dialog->setFileMode( QFileDialog::ExistingFile );
        file_dialog->setWindowFlags( Qt::Widget );
        file_dialog->setOption( QFileDialog::DontUseNativeDialog , true );

    SDialog *dialog = new SDialog( parent , 573 , 313 );
    QHBoxLayout *layout = new QHBoxLayout( dialog );
        layout->addWidget( file_dialog );

    QObject::connect( file_dialog , SIGNAL(accepted())            , dialog   , SLOT(accept()) );
    QObject::connect( file_dialog , SIGNAL(rejected())            , dialog   , SLOT(reject()) );
    QObject::connect( file_dialog , SIGNAL(fileSelected(QString)) , receiver , member         );

    return dialog;
}


SDialog *SDialogTools::getSaveFileName( SPage * parent
                                     , QObject *receiver, const char *member
                                     , const QString & caption
                                     , const QString & dir
                                     , const QString & filter)
{


    QFileDialog *file_dialog = new QFileDialog( 0 , caption , dir , filter );
        file_dialog->setFileMode( QFileDialog::AnyFile );
        file_dialog->setAcceptMode( QFileDialog::AcceptSave );
        file_dialog->setOption( QFileDialog::DontUseNativeDialog , true );
        file_dialog->setWindowFlags( Qt::Widget );

    SDialog *dialog = new SDialog( parent , 573 , 313 );
    QHBoxLayout *layout = new QHBoxLayout( dialog );
        layout->addWidget( file_dialog );

    QObject::connect( file_dialog , SIGNAL(accepted())             , dialog   , SLOT(accept()) );
    QObject::connect( file_dialog , SIGNAL(rejected())             , dialog   , SLOT(reject()) );
    QObject::connect( file_dialog , SIGNAL(fileSelected(QString))  , receiver , member         );

    return dialog;
}


SDialog *SDialogTools::getExistingDirectory( SPage * parent
                                         , QObject *receiver, const char *member
                                         , const QString & caption
                                         , const QString & dir)
{


    QFileDialog *file_dialog = new QFileDialog( 0 , caption , dir );
        file_dialog->setFileMode( QFileDialog::Directory );
        file_dialog->setOption( QFileDialog::DontUseNativeDialog , true );
        file_dialog->setOption( QFileDialog::ShowDirsOnly        , true );
        file_dialog->setWindowFlags( Qt::Widget );

    SDialog *dialog = new SDialog( parent , 573 , 313 );
    QHBoxLayout *layout = new QHBoxLayout( dialog );
        layout->addWidget( file_dialog );

    QObject::connect( file_dialog , SIGNAL(accepted())            , dialog   , SLOT(accept()) );
    QObject::connect( file_dialog , SIGNAL(rejected())            , dialog   , SLOT(reject()) );
    QObject::connect( file_dialog , SIGNAL(fileSelected(QString)) , receiver , member         );

    return dialog;
}


SDialog *SDialogTools::getFileProperties( SPage *parent, const QFileInfo & file
                                     , QObject *receiver, const char *member
                                      )
{
    QDialogButtonBox *buttons = new QDialogButtonBox( QDialogButtonBox::Close );
    QFormLayout *layout = new QFormLayout();
        layout->addRow( QObject::tr("Name")          , new QLineEdit( file.fileName()                              ) );
        layout->addRow( QObject::tr("Address")       , new QLineEdit( file.canonicalPath()                         ) );
        layout->addRow( QObject::tr("Type")          , new QLineEdit( file.suffix()                                ) );
        layout->addRow( QObject::tr("Owner")         , new QLineEdit( file.owner()                                 ) );
        layout->addRow( QObject::tr("size")          , new QLineEdit( QString::number( file.size() )               ) );
        layout->addRow( QObject::tr("Created")       , new QLineEdit( file.created().toString("dd MMMM yyyy")      ) );
        layout->addRow( QObject::tr("Last Modified") , new QLineEdit( file.lastModified().toString("dd MMMM yyyy") ) );
        layout->addRow( QObject::tr("Last Read")     , new QLineEdit( file.lastRead().toString("dd MMMM yyyy")     ) );
        layout->addRow( QString() , buttons );

    SDialog *dialog = new SDialog( parent , 437 , 273 );
        dialog->setLayout( layout );

    QObject::connect( buttons , SIGNAL(clicked(QAbstractButton*)) , dialog   , SLOT(close()) );

    if( receiver != 0 )
        QObject::connect( buttons , SIGNAL(clicked(QAbstractButton*)) , receiver , member );

    return dialog;
}


SDialog *SDialogTools::getTimer( SPage *parent , const QString & message , int milisecond
                              , QObject *receiver, const char *member , bool slot_bool_on )
{
    SGraphicTimer *timer    = new SGraphicTimer();
        timer->setFixedSize( 54 , 48 );

    QLabel        *label    = new QLabel( message );
    QHBoxLayout   *h_layout = new QHBoxLayout();
        h_layout->addWidget( timer );
        h_layout->addWidget( label );

    QDialogButtonBox *buttons = new QDialogButtonBox( QDialogButtonBox::Cancel );

    SDialog *dialog = new SDialog( parent , 373 , 87 );
    QVBoxLayout *layout = new QVBoxLayout( dialog );
        layout->addLayout( h_layout );
        layout->addWidget( buttons  );

    timer->start( milisecond );

    QObject::connect( timer   , SIGNAL(finished(bool))            , dialog , SLOT(setAccept(bool)) );
    QObject::connect( buttons , SIGNAL(clicked(QAbstractButton*)) , timer  , SLOT(stop())          );

    if( receiver != 0 )
    {
        if( slot_bool_on )
            QObject::connect( timer , SIGNAL(finished(bool)) , receiver , member );
        else
            QObject::connect( timer , SIGNAL(finished())     , receiver , member );
    }

    return dialog;
}


SDialog *SDialogTools::message( SPage * parent, const QString & title, const QString & text, QMessageBox::Icon icon, QObject *receiver, const char *member
                     , QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton )
{


    QMessageBox *message = new QMessageBox( icon, title, text , buttons , 0 , Qt::Widget );
        message->setDefaultButton( defaultButton );

    SDialog *dialog = new SDialog( parent , 437 , 137 );
    QHBoxLayout *layout = new QHBoxLayout( dialog );
        layout->addWidget( message );

    if( receiver != 0 && member != 0 )
        QObject::connect( message , SIGNAL(finished(int)) , receiver , member );

    QObject::connect( message , SIGNAL(buttonClicked(QAbstractButton*)) , dialog   , SLOT(close())  );
    QObject::connect( message , SIGNAL(accepted())                      , dialog   , SLOT(accept()) );
    QObject::connect( message , SIGNAL(rejected())                      , dialog   , SLOT(reject()) );

    return dialog;
}
