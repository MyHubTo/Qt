#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <stdlib.h>
#include <QTimer>
#include <QMenu>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    void shutdown();
    QTimer *timer;
    QMenu *menu;
private slots:
    void on_timeout();
    void on_pushButton_2_clicked();

};
#endif // WIDGET_H
