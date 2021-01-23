#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LabSocketState=new QLabel("Socket状态：");
    LabSocketState->setMaximumWidth(250);
    ui->statusbar->addWidget(LabSocketState);

    QString localIP=getLocalIP();
    this->setWindowTitle(this->windowTitle()+"------本地IP"+localIP);

    udpSocket=new QUdpSocket(this);

    //Muitcast 路由层次，1表示在同一个局域网内；
    //组播 TTL：生存时间，每跨一个路由会减1,多播无法跨过大多数路由所以为1；
    //默认值是1：表示只能在本地的子网中传送
    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,1);

    connect(udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(udpSocket->state());

    connect(udpSocket,SIGNAL(readyRead()),
            this,SLOT(onSocketReadyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLocalIP()
{
    QString hostName=QHostInfo::localHostName();//本地主机名
    QHostInfo   hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";

    QList<QHostAddress> addList=hostInfo.addresses();//

    if (!addList.isEmpty())
    for (int i=0;i<addList.count();i++)
    {
        QHostAddress aHost=addList.at(i);
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            localIP=aHost.toString();
            break;
        }
    }
    return localIP;
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        LabSocketState->setText("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        LabSocketState->setText("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        LabSocketState->setText("scoket状态：ConnectingState");
        break;

    case QAbstractSocket::ConnectedState:
        LabSocketState->setText("scoket状态：ConnectedState");
        break;

    case QAbstractSocket::BoundState:
        LabSocketState->setText("scoket状态：BoundState");
        break;

    case QAbstractSocket::ClosingState:
        LabSocketState->setText("scoket状态：ClosingState");
        break;

    case QAbstractSocket::ListeningState:
        LabSocketState->setText("scoket状态：ListeningState");
    }
}

void MainWindow::onSocketReadyRead()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray   datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress    peerAddr;
        quint16 peerPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);

//        udpSocket->readDatagram(datagram.data(),datagram.size());
        QString str=datagram.data();

        QString peer="[From "+peerAddr.toString()+":"+QString::number(peerPort)+"] ";

        ui->plainTextEdit->appendPlainText(peer+str);
    }
}


void MainWindow::on_actionStart_triggered()
{
    QString IP=ui->comboIP->currentText();//获取多播组地址，自定义
    groupAddress=QHostAddress(IP);
    quint16 groupPort=ui->spinPort->value();//获取组播端口

    if(udpSocket->bind(QHostAddress::AnyIPv4,groupPort,QUdpSocket::ShareAddress))//绑定端口
    {
         udpSocket->joinMulticastGroup(groupAddress);//加入多播组
         ui->plainTextEdit->appendPlainText("--加入组播成功--");
         ui->plainTextEdit->appendPlainText("组播地址："+IP);
         ui->plainTextEdit->appendPlainText("-- 绑定端口:"+QString::number(ui->spinPort->value()));

         ui->actionStart->setEnabled(false);
         ui->actionStop->setEnabled(true);
         ui->comboIP->setEnabled(false);
    }
    else
    {
       ui->plainTextEdit->appendPlainText("--绑定端口失败--");
    }
}

void MainWindow::on_actionStop_triggered()
{
    udpSocket->leaveMulticastGroup(groupAddress);// 退出组播
    udpSocket->abort();//解除绑定

    ui->actionStart->setEnabled(true);
    ui->actionStop->setEnabled(false);
    ui->comboIP->setEnabled(true);
    ui->plainTextEdit->appendPlainText("--已经退出组播，解除端口绑定--");

}

void MainWindow::on_btnMulticast_clicked()
{
    //发送组播 消息
    quint16 groupPort=ui->spinPort->value();

    QString msg=ui->editMsg->text();
    QByteArray datagram=msg.toUtf8();
    // 向 指定 地址端口发送 消息
    udpSocket->writeDatagram(datagram,groupAddress,groupPort);

    ui->plainTextEdit->appendPlainText("[multicst]"+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}

void MainWindow::on_actionClear_triggered()
{
    ui->plainTextEdit->clear();
}
