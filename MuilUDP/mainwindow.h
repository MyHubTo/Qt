#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QLabel>
#include <QtNetwork/QHostInfo>
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
    QLabel *LabSocketState;
    QUdpSocket *udpSocket;

    QHostAddress groupAddress;//组播地址
    QString getLocalIP();//获取本机IP

private slots:
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_btnMulticast_clicked();
    void on_actionClear_triggered();
};
#endif // MAINWINDOW_H
