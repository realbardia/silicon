#ifndef DIALOGWIDGET_H
#define DIALOGWIDGET_H

#include <QWidget>

class DialogWidgetPrivate;
class DialogWidget : public QWidget
{
    Q_OBJECT
public:
    DialogWidget(QWidget *parent = 0);
    ~DialogWidget();

    bool expanded() const;
    bool unexpanded() const;

signals:

public slots:
    void showWidget( QWidget *widget );

    void unexpand();
    void expand();

protected:
    void resizeEvent( QResizeEvent *event );

private slots:
    void update_widget();
    void main_resized();

private:
    DialogWidgetPrivate *p;
};

#endif // DIALOGWIDGET_H
