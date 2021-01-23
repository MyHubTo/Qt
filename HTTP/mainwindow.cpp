#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->editURL->setClearButtonEnabled(true);//设置后editURL在输入值之后，会有一个清除按钮<[x]；
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_finished()
{//网络响应结束
    QFileInfo fileinfo;
    fileinfo.setFile(downloadedFile->fileName());

    downloadedFile->close();
    delete downloadedFile;

    downloadedFile=Q_NULLPTR;//空指针

    reply->deleteLater();
    reply=Q_NULLPTR;

    if(ui->checkOpen->isChecked())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileinfo.absoluteFilePath()));
    }
    ui->btnDownload->setEnabled(true);

}

void MainWindow::on_readyRead()
{
    downloadedFile->write(reply->readAll());
}

void MainWindow::on_downloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}


void MainWindow::on_btnDefaultPath_clicked()
{
    //设置默认下载路径
    QString curpath=QDir::currentPath();
    QDir dir(curpath);

    QString sub="temp";
    dir.mkdir(sub);//在当前路径下建立temp文件夹

    ui->editPath->setText(curpath+"/"+sub+"/");
}

void MainWindow::on_btnDownload_clicked()
{
    //开始下载 按钮
        QString urlSpec = ui->editURL->text().trimmed();
        if (urlSpec.isEmpty())
        {
            QMessageBox::information(this, "错误",
              "请指定需要下载的URL");
            return;
        }

        QUrl newUrl = QUrl::fromUserInput(urlSpec);//URL地址
        if (!newUrl.isValid())
        {
            QMessageBox::information(this, "错误",
              QString("无效URL: %1 \n 错误信息: %2").arg(urlSpec, newUrl.errorString()));
            return;
        }

        QString tempDir =ui->editPath->text().trimmed();//临时目录
        if (tempDir.isEmpty())
        {
            QMessageBox::information(this, tr("错误"), "请指定保存下载文件的目录");
            return;
        }

        QString fullFileName =tempDir+newUrl.fileName(); //文件名

        if (QFile::exists(fullFileName))
            QFile::remove(fullFileName);

        downloadedFile =new QFile(fullFileName);//创建临时文件
        if (!downloadedFile->open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("错误"),"临时文件打开错误");
            return;
        }

        ui->btnDownload->setEnabled(false);

    //发送玩过请求，创建网络响应
        reply = networkManager.get(QNetworkRequest(newUrl));
        connect(reply, SIGNAL(finished()), this, SLOT(on_finished()));
        connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(on_downloadProgress(qint64,qint64)));
}


void MainWindow::on_pushButton_clicked()
{
    QString currPath=QDir::currentPath();
    QString savePath= QFileDialog::getExistingDirectory(this,"选择一个目录",currPath);

    ui->editPath->setText(savePath);
}
