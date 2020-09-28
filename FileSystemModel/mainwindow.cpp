#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model=new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());//设置根目录
    ui->treeView->setModel(model);//设置数据模型
    ui->listView->setModel(model);//设置数据模型
    ui->tableView->setModel(model);//设置数据模型

    //信号和槽关联，treeView单击时，其目录设置为ListView和TabelView的根节点
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),ui->listView,SLOT(setRootIndex(QModelIndex)));
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),ui->tableView,SLOT(setRootIndex(QModelIndex)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    //判断是否是目录
    ui->checkBox->setChecked(model->isDir(index));
    //输出当前文件的路径，类型，名称，大小
    ui->label_path->setText(model->filePath(index));
    ui->label_Type->setText(model->type(index));
    ui->label_FileName->setText(model->fileName(index));
    int sz=model->size(index)/1024;
    if(sz<1024)
    {
        ui->label_filesize->setText(QString("%1 KB").arg(sz));
    }
    else
    {
        ui->label_filesize->setText(QString::asprintf("%.1f MB",sz/1024.0));
    }
}
