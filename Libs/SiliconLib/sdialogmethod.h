#ifndef SDIALOGMETHOD_H
#define SDIALOGMETHOD_H

#include <QObject>

#include "sdialog.h"
#include "spage.h"
#include "libsilicon_global.h"

class LIBSILICON_EXPORT SDialogMethod : public QObject
{
    Q_OBJECT
public:
    SDialogMethod();
    ~SDialogMethod();

    virtual void insert( SDialog *dialog );

    virtual void remove( SDialog *dialog );
    virtual void remove( SPage *dialog );

    virtual void pageTypeChanged( SPage::PageType type );

    virtual void currentTabChanged( SPage *page );
    virtual void currentDialogChanged( SDialog *dialog );

    virtual void dialogVisibilityChanged( SDialog *dialog , bool stt );

private:
    void init();
    void kill();
};

#endif // SDIALOGMETHOD_H
