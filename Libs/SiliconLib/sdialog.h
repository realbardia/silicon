#ifndef SDIALOG_H
#define SDIALOG_H

#include <QWidget>
#include <QList>

#include "spage.h"
#include "libsilicon_global.h"

class SDialogPrivate;
class LIBSILICON_EXPORT SDialog : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Type of the Parent.
     * \sa parentType
     */
    enum ParentType{
        Dialog,      /*! SDialog parent */
        Page         /*! SPage Parent */
    };

    SDialog( SDialog *parent , int width , int height );
    SDialog( SPage *parent , int width , int height );
    SDialog( int width , int height );   // Showing on Process Page
    ~SDialog();

    SPage *pageParent();
    SDialog *dialogParent();

    ParentType parentType();

    void addDialog( SDialog *dialog );
    const QList<SDialog *> & dialogs();

    void exec( QObject *receiver, const char *member );

public slots:
    void setVisible( bool visible );

    void accept();
    void reject();

    void setAccept( bool stt );

signals:
    void accepted( bool stt );
    void accepted();
    void rejected();

    void destroyed( SDialog *dialog );

private slots:
    void dialogDestroyed( SDialog *dialog );

private:
    void init();

    void setPageParent( SPage *parent );
    void setDialogParent( SDialog *parent );

private:
    SDialogPrivate *p;
};

#endif // SDIALOG_H
