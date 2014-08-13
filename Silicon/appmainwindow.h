#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include <QWidget>
#include <SPage>

class QStatusBar;
class QToolBar;
class AppMainWindowPrivate;
class AppMainWindow : public QWidget
{
    Q_OBJECT
public:
    AppMainWindow( SPage *page , QWidget *parent = 0 );
    ~AppMainWindow();

    QToolBar *toolBar() const;
    QStatusBar *statusBar() const;

public slots:
    void refresh();

    void setToolBar( QToolBar *toolbar );
    void setStatusBar( QStatusBar *statusbar );

signals:

private slots:
    void updated();
    void resizeSlot( const QSize & size );

protected:
    void resizeEvent( QResizeEvent *event );
    void closeEvent( QCloseEvent *event );

private:
    QSize extraSize() const;

protected:
    AppMainWindowPrivate *p;
};

#endif // APPMAINWINDOW_H
