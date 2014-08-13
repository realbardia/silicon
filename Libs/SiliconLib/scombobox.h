#ifndef SCOMBOBOX_H
#define SCOMBOBOX_H

#include <QComboBox>

#include "libsilicon_global.h"

class SComboBoxPrivate;
class LIBSILICON_EXPORT SComboBox : public QComboBox
{
    Q_OBJECT
public:
    SComboBox( QWidget *parent = 0 );
    ~SComboBox();

private slots:
    void indexChanged( int index );

protected:
    void actionEvent( QActionEvent *event );

private:
    SComboBoxPrivate *p;
};

#endif // SCOMBOBOX_H
