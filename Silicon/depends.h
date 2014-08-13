#ifndef DEPENDS_H
#define DEPENDS_H

#include <QDialog>
#include <QStringList>
#include <QTreeWidgetItem>

namespace Ui {
    class Depends;
}

class Depends : public QDialog {
    Q_OBJECT
public:
    Depends(QWidget *parent = 0);
    ~Depends();

    void setBinaryDepends( const QStringList & list );
    void setSAppDepends( const QStringList & list );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Depends *ui;

    void clearItem( QTreeWidgetItem *item );
    void addChilds(  QTreeWidgetItem *item , const QStringList & list );
};

#endif // DEPENDS_H
