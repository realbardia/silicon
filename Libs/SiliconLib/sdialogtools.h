#ifndef SDIALOGTOOLS_H
#define SDIALOGTOOLS_H

#include <QWidget>
#include <QLineEdit>
#include <QMessageBox>

#include "libsilicon_global.h"

class QFileInfo;
class SPage;
class SDialog;

class LIBSILICON_EXPORT SDialogTools
{
public:
    static SDialog *getColor(  QObject *receiver, const char *member );
    static SDialog *getLineEdit(   SPage *parent
                             , QObject *receiver, const char *member
                             , const QString & title
                             , const QString & label
                             , QLineEdit::EchoMode mode = QLineEdit::Normal
                             , const QString & text = QString());

    static SDialog *getOpenFileNames( SPage *parent
                                 , QObject *receiver, const char *member
                                 , const QString & caption = QString()
                                 , const QString & dir = QString()
                                 , const QString & filter = QString() );

    static SDialog *getOpenFileName( SPage *parent
                                 , QObject *receiver, const char *member
                                 , const QString & caption = QString()
                                 , const QString & dir = QString()
                                 , const QString & filter = QString() );

    static SDialog *getSaveFileName( SPage *parent
                                 , QObject *receiver, const char *member
                                 , const QString & caption = QString()
                                 , const QString & dir = QString()
                                 , const QString & filter = QString() );

    static SDialog *getExistingDirectory( SPage *parent
                                     , QObject *receiver, const char *member
                                     , const QString & caption = QString()
                                     , const QString & dir = QString() );

    static SDialog *getFileProperties( SPage *parent , const QFileInfo & file
                                  , QObject *receiver = 0, const char *member = 0 );

    static SDialog *getTimer( SPage *parent , const QString & message , int milisecond
                                  , QObject *receiver = 0, const char *member = 0 , bool slot_bool_on = false );


    static SDialog *message( SPage * parent, const QString & title, const QString & text, QMessageBox::Icon icon, QObject *receiver = 0, const char *member = 0
                         , QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton );

};

#endif // SDIALOGTOOLS_H
