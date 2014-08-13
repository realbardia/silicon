#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QWidget>

class QToolBar;
class SPage;
class ToolBarManagerPrivate;
class ToolBarManager : public QWidget
{
    Q_OBJECT
public:
    ToolBarManager( QWidget *parent = 0 );
    ~ToolBarManager();

    QToolBar *current() const;
    int panelHeight() const;

    void setAnimation( bool stt );
    bool animation() const;

public slots:
    void setCurrent( SPage *page , QToolBar *toolbar );

signals:

private slots:
    void animation_finished();

protected:
    void paintEvent( QPaintEvent *event );

private:
    ToolBarManagerPrivate *p;
};

#endif // TOOLBARMANAGER_H
