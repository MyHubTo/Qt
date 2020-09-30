#include "widget.h"
#include "ui_widget.h"
#include <QStringList>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList theStrList;
    theStrList<<"北京"<<"上海"<<"天津"<<"河北"<<"山东"<<"四川"<<"重庆";
    theModel=new QStringListModel(this);
    //导入list内容
    theModel->setStringList(theStrList);
    //设置数据模型
    ui->listView->setModel(theModel);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked|
                                  QAbstractItemView::SelectedClicked);
    Widget::setMinimumSize(800,600);
    ui->groupBox_2->setMinimumWidth(200);
    ui->groupBox_2->setMaximumWidth(250);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushBu_additem_clicked()
{
    //在列表的最后一行添加一项
    theModel->insertRow(theModel->rowCount());

    //获取当前索引，里面包含行号，列号，父项的模型索引...
    QModelIndex index=theModel->index(theModel->rowCount()-1,0);
    theModel->setData(index,"new Item",Qt::DisplayRole);

    //选中当前行
    ui->listView->setCurrentIndex(index);
    label=new QLabel("");


}

void Widget::on_pushBut_insertItem_clicked()
{

    //获取当前行索引
    QModelIndex index=ui->listView->currentIndex();
    if(index.row()==0)
    {
        on_pushBu_additem_clicked();
    }
    theModel->insertRow(index.row());
    //由于插入的是一个空白行，所以通过setData进行，输入数据；
    theModel->setData(index,"insert Item",Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

void Widget::on_pushBut_deleteItem_clicked()
{

    QModelIndex index=ui->listView->currentIndex();
    theModel->removeRow(index.row());
}


void Widget::on_pushBut_clear_clicked()
{
    //删除列表使用的是removeRows（）
    theModel->removeRows(0,theModel->rowCount());
}

void Widget::on_pushBut_showList_clicked()
{
    QStringList tmpList=theModel->stringList();
    ui->plainTextEdit->clear();
    for(int i=0;i<tmpList.count();i++)
    {
        ui->plainTextEdit->appendPlainText(tmpList.at(i));
    }
}

void Widget::on_pushBut_restore_clicked()
{
    QStringList theStrList;
    theStrList<<"北京"<<"上海"<<"天津"<<"河北"<<"山东"<<"四川"<<"重庆";
    theModel=new QStringListModel(this);
    //导入list内容
    theModel->setStringList(theStrList);
    //设置数据模型
    ui->listView->setModel(theModel);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked|
                                  QAbstractItemView::SelectedClicked);
}

void Widget::on_pushBut_clearText_clicked()
{
    ui->plainTextEdit->clear();
}

void Widget::on_label_objectNameChanged(const QString &objectName)
{
    Q_UNUSED(objectName)
   // ui->listView->currentIndex();
   QString str= theModel->data(ui->listView->currentIndex()).toString();
   ui->label->setText("str");
}

void Widget::on_listView_clicked(const QModelIndex &index)
{
    QString str=theModel->data(ui->listView->currentIndex()).toString();
     ui->label->setText(str);
}
