#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStringListModel>
#include<QLabel>
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
    void on_pushBu_additem_clicked();

    void on_pushBut_insertItem_clicked();

    void on_pushBut_deleteItem_clicked();

    void on_pushBut_clear_clicked();

    void on_pushBut_showList_clicked();

    void on_pushBut_restore_clicked();

    void on_pushBut_clearText_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QStringListModel *theModel;
    QLabel *label;
};
#endif // WIDGET_H
