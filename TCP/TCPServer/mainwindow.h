#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTcpServer>
#include <QEvent>
#include <QCloseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *LabListen;//状态栏标签
    QLabel *LabSocketState;//套接字状态

    QTcpServer *tcpServer;//Tcp服务器
    QTcpSocket *tcpSocket;//套接字，用于TCP通讯

    QString getLocalTP();//获取本地IP
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void onSocketStateChange(QAbstractSocket::SocketState socketstate);
    void onNewConnection();//QTcpserver的newConnection()信号
    void onClientConnected();//Client Socket connected
    void onClientDisconneted();//Client Socket disconnect
    void onSocketReadyRead();//读取socket传入的数据
    void on_act_art_triggered();
    void on_act_stop_triggered();
    void on_act_clear_triggered();
    void on_act_Exit_triggered();
    void on_btnSend_5_clicked();

};
#endif // MAINWINDOW_H
