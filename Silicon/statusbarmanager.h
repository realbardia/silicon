#ifndef STATUSBARMANAGER_H
#define STATUSBARMANAGER_H

#include <QWidget>

class QStatusBar;
class StatusBarManagerPrivate;
class StatusBarManager : public QWidget
{
    Q_OBJECT
public:
    StatusBarManager(QWidget *parent = 0);
    ~StatusBarManager();

    QStatusBar *current() const;

    int panelHeight() const;

public slots:
    void setCurrent( QStatusBar *widget );

signals:

protected:
    void paintEvent( QPaintEvent *event );

private:
    StatusBarManagerPrivate *p;
};

#endif // STATUSBARMANAGER_H
