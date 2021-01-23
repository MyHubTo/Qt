#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QtNetwork>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LabListen=new QLabel(tr("监听状态："));
    LabListen->setMinimumWidth(150);
    ui->statusbar->addWidget(LabListen);

    LabSocketState=new QLabel(tr("Socket状态："));
    LabSocketState->setMinimumWidth(200);
    ui->statusbar->addWidget(LabSocketState);


    QString localIp=getLocalTP();
    this->setWindowTitle(this->windowTitle()+"--------本机Ip"+localIp);
    ui->comboIP_5->addItem(localIp);

    tcpServer=new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLocalTP()
{
    //获取本机Ipv4地址
    QString hostname= QHostInfo::localHostName();//获取主机名
    QHostInfo hostInfo=QHostInfo::fromName(hostname);
    QList<QHostAddress>addList=hostInfo.addresses();
    QString localIp("");
    if(!addList.isEmpty())
    {
        for(int i=0;i<addList.count();i++)
        {
            QHostAddress aHost=addList.at(i);
            if(aHost.protocol()==QAbstractSocket::IPv4Protocol)
            {
                localIp=aHost.toString();
                break;
            }
        }
    }
    return localIp;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //关闭窗口时停止监听
    if(tcpServer->isListening())
        tcpServer->close();//关闭监听
    event->accept();
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketstate)
{
    switch (socketstate)
    {
        case QAbstractSocket::UnconnectedState:
        LabSocketState->setText(tr("scoket状态：UnconnectedState"));
        break;
    case QAbstractSocket::HostLookupState:
        LabSocketState->setText(tr("scoket状态：HostLookupState"));
         break;
    case QAbstractSocket::ConnectingState:
        LabSocketState->setText(tr("scoket状态：ConnectingState"));
         break;
     case QAbstractSocket::ConnectedState:
         LabSocketState->setText(tr("scoket状态：ConnectedState"));
          break;
    case QAbstractSocket::BoundState:
        LabSocketState->setText(tr("scoket状态：BoundState"));
         break;
    case QAbstractSocket::ListeningState:
        LabSocketState->setText(tr("scoket状态：ListeningState"));
         break;
    case QAbstractSocket::ClosingState:
        LabSocketState->setText(tr("scoket状态：ClosingState"));
         break;

    }
}

void MainWindow::onNewConnection()
{
    //Socket连接成功
    tcpSocket = tcpServer->nextPendingConnection(); //创建socket

    connect(tcpSocket, SIGNAL(connected()),this, SLOT(onClientConnected()));//调用信号函数需要添加头文件#include<QtNetwork>
    connect(tcpSocket,SIGNAL(disconnect()),this,SLOT(onClientDisconneted()));

    //QAbstractSocket::SocketState 输入参数类型
    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));

}

void MainWindow::onClientConnected()
{
    //客户端接入
    ui->plainTextEdit->appendPlainText(tr("client socket connected"));
        //preeAddress 如果套接字处于ConnectedState，则返回已连接对等方的地址；否则，返回0。 否则返回QHostAddress :: Null。
    ui->plainTextEdit->appendPlainText("peer address："+tcpSocket->peerAddress().toString());

    //Port:端口
    //如果套接字处于ConnectedState，则返回已连接对等方的端口；否则，返回0。 否则返回0。
    ui->plainTextEdit->appendPlainText("peer port:"+QString::number(tcpSocket->peerPort()));

}

void MainWindow::onClientDisconneted()
{
    //客户端连接断开
    ui->plainTextEdit->appendPlainText("Client Socket disconnected");
    tcpSocket->deleteLater();//删除；连接
}

void MainWindow::onSocketReadyRead()
{
    //读取缓冲区的文本
    while(tcpSocket->canReadLine())
    {
        ui->plainTextEdit->appendPlainText("[in]"+tcpSocket->readLine());
    }
}


void MainWindow::on_act_art_triggered()
{
    //开始监听
    QString IP=ui->comboIP_5->currentText();//Ip地址
    auto port=ui->spinPort_5->value();

    QHostAddress addr(IP);
    tcpServer->listen(addr,port);//开始监听指定地址和端口

    ui->plainTextEdit->appendPlainText("开始监听：");
    ui->plainTextEdit->appendPlainText("服务器地址："+tcpServer->serverAddress().toString());
    ui->plainTextEdit->appendPlainText("服务器端口:"+QString::number(tcpServer->serverPort()));

    ui->act_art->setEnabled(false);
    ui->act_stop->setEnabled(true);
    LabListen->setText("监听状态:正在监听");
}

void MainWindow::on_act_stop_triggered()
{
    //停止监听
    if(tcpServer->isListening())
    {
        tcpServer->close();//停止监听
        ui->plainTextEdit->appendPlainText("停止监听");
        LabListen->setText("停止监听");
        ui->act_art->setEnabled(true);
        ui->act_stop->setEnabled(false);
    }
}

void MainWindow::on_act_clear_triggered()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_act_Exit_triggered()
{
    this->close();
}

void MainWindow::on_btnSend_5_clicked()
{
    //发送一行字符串
    QString str=ui->editMsg_5->text();
    ui->plainTextEdit->appendPlainText("[out]"+str);
    ui->editMsg_5->clear();
    ui->editMsg_5->setFocus();

    QByteArray str2=str.toUtf8();//将字符串转换成Utf8编码形式
    str2.append('\n');//换行
    tcpSocket->write(str2);//写入缓存区
}
