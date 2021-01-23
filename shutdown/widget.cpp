#include "widget.h"
#include "ui_widget.h"
#include <string>
#include <iostream>
#include <QMenu>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    menu =new QMenu(this);
    ui->lineEdit->setText("30");
    ui->lcdNumber->setLineWidth(0);
    ui->lineEdit->setPlaceholderText("请输入关机时间(秒)");

    connect(timer,SIGNAL(timeout()),this,SLOT(on_timeout()));

}

Widget::~Widget()
{
    delete ui;
}


void Widget::shutdown()
{
    QString str=ui->lineEdit->text();
    int displaytime=str.toUInt();
    ui->lcdNumber->display(displaytime);
    std::string time=str.toStdString();
    std::string shutdown="shutdown -s -t "+time;
    const char *p=shutdown.c_str();
    system(p);
}

void Widget::on_timeout()
{
    double num=ui->lcdNumber->value();
    num--;
    if(0==num)
    {
        timer->stop();
    }
    QString str_=QString::number(num);
    ui->lcdNumber->display(str_);

}


void Widget::on_pushButton_clicked()
{
    shutdown();
    timer->start(1000);
}

void Widget::on_pushButton_2_clicked()
{
    system("shutdown -a");
    timer->stop();
    close();
}


