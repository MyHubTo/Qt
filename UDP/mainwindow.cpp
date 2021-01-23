#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QAbstractSocket>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LabSockeState=new QLabel("Socket 状态：");
    LabSockeState->setMidLineWidth(250);
    ui->statusbar->addWidget(LabSockeState);

    QString localIP=getLocalIp();
    this->setWindowTitle(this->windowTitle()+"----"+localIP);
    ui->comboTargetIP->addItem(localIP);

    udpStocket =new QUdpSocket(this);//用于与连接的客户端通讯

    connect(udpStocket,SIGNAL(stateChanged(onSocketStateChange::SocketState)),
            this,SLOT(onSocketStateChange::SocketState));
    onSocketStateChange(udpStocket->state());

    connect(udpStocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));

}

MainWindow::~MainWindow()
{
    delete ui;
    udpStocket->abort();
    delete udpStocket;
}

QString MainWindow::getLocalIp()
{
    QString hostname=QHostInfo::localHostName();
    QHostInfo hostinfo=QHostInfo::fromName(hostname);
    QString LocalIP="";
    QList<QHostAddress> addrList=hostinfo.addresses();
    if(!addrList.isEmpty())
    {
        for (int i=0;i<addrList.count();i++)
        {
            QHostAddress aHost=addrList.at(i);

            if(QAbstractSocket::IPv4Protocol==aHost.protocol())
            {
                LocalIP=aHost.toString();
                break;
            }
        }
    }
    return LocalIP;
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState SocketState)
{
    switch(SocketState)
    {
    case QAbstractSocket::UnconnectedState:
        LabSockeState->setText("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        LabSockeState->setText("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        LabSockeState->setText("scoket状态：ConnectingState");
        break;

    case QAbstractSocket::ConnectedState:
        LabSockeState->setText("scoket状态：ConnectedState");
        break;

    case QAbstractSocket::BoundState:
        LabSockeState->setText("scoket状态：BoundState");
        break;

    case QAbstractSocket::ClosingState:
        LabSockeState->setText("scoket状态：ClosingState");
        break;

    case QAbstractSocket::ListeningState:
        LabSockeState->setText("scoket状态：ListeningState");
    }
}

void MainWindow::onSocketReadyRead()
{
    //读取收到的信息
    while (udpStocket->hasPendingDatagrams())//当至少有一个数据需要读取时，返回true；
    {
        QByteArray datagram;
        datagram.resize(udpStocket->pendingDatagramSize());//返回要发数据第一个数据报大小

        QHostAddress peerAddr;
        quint16 peerPort;

        //当点击发送按钮时，会通过udpStocket->writeDatagram(str,targetAddr,targetPort);将数据包发送。
        udpStocket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);//依次，要接受的数据，大小，发送端地址，发送端端口
        //udpStocket->readDatagram(datagram.data(),datagram.size());//不需要发送端的地址与端口；
        QString str=datagram.data();

        QString peer="[form"+peerAddr.toString()+":"+QString::number(peerPort)+"]\n";

        ui->plainTextEdit->appendPlainText(peer+str);
    }
}


void MainWindow::on_actStart_triggered()
{
    //绑定端口
    quint16 port=ui->spinBindPort->value();//int 类型
    if(udpStocket->bind(port))//判断是否绑定端口；这是由于一个IP地址，可以有多个端口，所以需要指定端口
    {
        ui->plainTextEdit->appendPlainText("--已成功绑定端口--");
        ui->plainTextEdit->appendPlainText("--绑定端口："+QString::number(port));

        ui->actStart->setEnabled(false);
        ui->actStop->setEnabled(true);
    }
    else {
        ui->plainTextEdit->appendPlainText("--绑定失败--");
    }
}

void MainWindow::on_actStop_triggered()
{
    udpStocket->abort();//解除端口绑定

    ui->actStop->setEnabled(false);
    ui->actStart->setEnabled(true);
    ui->plainTextEdit->appendPlainText("--已解除绑定--");
}

void MainWindow::on_actClear_triggered()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_btnSend_clicked()
{
    //发送按钮
    QString targetIP=ui->comboTargetIP->currentText();//目标IP
    QHostAddress targetAddr(targetIP);

    quint16 targetPort=ui->spinTargetPort->value();//目标端口

    QString msg=ui->editMsg->text();

    QByteArray str=msg.toUtf8();
    udpStocket->writeDatagram(str,targetAddr,targetPort);//向指定地址与端口发送数据包

    ui->plainTextEdit->appendPlainText("[out]"+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();

}

void MainWindow::on_btnBroadcast_clicked()
{
    //广播按钮
    quint16 targetPort=ui->spinTargetPort->value();
    QString msg=ui->editMsg->text();

    QByteArray str=msg.toUtf8();

    //不同,广播地址：QHostAddress::BroadCast
   udpStocket->writeDatagram(str,QHostAddress::Broadcast,targetPort);

   ui->plainTextEdit->appendPlainText("[broadcast]"+msg);
   ui->editMsg->clear();
   ui->editMsg->setFocus();


}
