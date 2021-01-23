#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>//协调网络操作
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QUrl>
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
    QNetworkAccessManager networkManager;//网络管理
    QNetworkReply *reply;//网络响应
    QFile *downloadedFile;

private slots:
    void on_finished();
    void on_readyRead();
    void on_downloadProgress(qint64 bytesRead,qint64 totalBytes);
    void on_btnDefaultPath_clicked();
    void on_btnDownload_clicked();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
