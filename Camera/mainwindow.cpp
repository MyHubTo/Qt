#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("video.ico)"));
    ui->actionCapture->setEnabled(false);
    ui->actionStop->setEnabled(false);
    //QCamera
    QCameraInfo cameraInfo=QCameraInfo::defaultCamera();
    ui->comboBox->addItem(cameraInfo.description());

    QCameraViewfinderSettings cameraViewFinderSettings;
    cameraViewFinderSettings.setResolution(640,480);
    cameraViewFinderSettings.setMinimumFrameRate(30.0);
    curCamera=new QCamera(cameraInfo,this);
    curCamera->setViewfinderSettings(cameraViewFinderSettings);
    /*
     * Camera.CaptureViewfinder:Camera is only configured to display viewfinder.
     * Camera.CaptureStillImage:Prepares the Camera for capturing still images.
     *  Camera.CaptureVideo:Prepares the Camera for capturing video.
     */
    curCamera->setCaptureMode(QCamera::CaptureMode::CaptureStillImage);
    curCamera->setViewfinder(ui->videoWidget);

    //QimageCapture
    imageCapture=new QCameraImageCapture(curCamera,this);
    QImageEncoderSettings imageEncodeSettings;
    imageEncodeSettings.setCodec("video/jpeg");
    imageEncodeSettings.setQuality(QMultimedia::EncodingQuality::HighQuality);
    imageEncodeSettings.setResolution(640,480);

    //添加质量
    ui->comboBox_Quality->addItem("VeryLowQuality",0);
    ui->comboBox_Quality->addItem("LowQuality",1);
    ui->comboBox_Quality->addItem("NormalQuality",2);
    ui->comboBox_Quality->addItem("HighQuality",3);
    ui->comboBox_Quality->addItem("VeryHighQuality",4);
    ui->comboBox_Quality->setCurrentIndex(3);
    imageCapture->setEncodingSettings(imageEncodeSettings);

    //setCaptureDestination设置抓图存储目标为文件，抓取的图片文件自动保存到用户目录的“图片”文件夹；
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    ui->checkVideo->setChecked(curCamera->isCaptureModeSupported(QCamera::CaptureMode::CaptureVideo));//检测是否支持视频
    ui->checkImage->setChecked(curCamera->isCaptureModeSupported(QCamera::CaptureMode::CaptureStillImage));//检测是否支持抓图
    this->setFixedSize(1035,635);
    connect(imageCapture,SIGNAL(imageCaptured(int,const QImage &)),this,SLOT(on_imageCaptured(int,const QImage &)));
    QString soundFile=":/wav/wav/Windows Shutdown.wav";
    sound =new QSound(soundFile,this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_btnStart_clicked()
//{
//    curCamera->start();
//}

//void MainWindow::on_btnStop_clicked()
//{
//    curCamera->stop();
//}

//void MainWindow::on_btnCapture_clicked()
//{
//    if(curCamera->captureMode()!=QCamera::CaptureMode::CaptureStillImage)
//    {
//        curCamera->setCaptureMode(QCamera::CaptureMode::CaptureStillImage);

//    }
//    QString title="选择保存路径";
//    QString curPath=QDir::currentPath();
//    //QString filename=QFileDialog::getExistingDirectory(this,title,curPath);
//    //QString filename=QFileDialog::getSaveFileName(this,title,curPath);
//    imageCapture->capture();
//   // QMessageBox::information(this,"information","capturebtn called");

//}

void MainWindow::on_imageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id);
    CapturedImgae=preview;
    QImage image=preview.scaled(ui->labCapture->size(),Qt::AspectRatioMode::KeepAspectRatio,Qt::TransformationMode::SmoothTransformation);
    ui->labCapture->setPixmap(QPixmap::fromImage(image));
    sound->play();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionStart_triggered()
{
    if(ui->comboBox->itemText(0).isEmpty())
    {
        QMessageBox::critical(this,"错误","未找到可用设备，请检查！");
        return;
    }
    curCamera->start();
    ui->actionCapture->setEnabled(true);
    ui->actionStop->setEnabled(true);
}

void MainWindow::on_actionStop_triggered()
{
    curCamera->stop();
    ui->actionCapture->setEnabled(false);
    ui->actionStop->setEnabled(false);
}

void MainWindow::on_actionCapture_triggered()
{
    if(curCamera->captureMode()!=QCamera::CaptureMode::CaptureStillImage)
    {
        curCamera->setCaptureMode(QCamera::CaptureMode::CaptureStillImage);

    }

    imageCapture->capture();
    count++;
  }

void MainWindow::on_comboBox_Quality_currentIndexChanged(int index)
{
    QImageEncoderSettings imageEncodeSettings;
    switch (index)
    {
    case 0:
        imageEncodeSettings.setQuality(QMultimedia::EncodingQuality::VeryLowQuality);
        break;
    case 1:
        imageEncodeSettings.setQuality(QMultimedia::EncodingQuality::LowQuality);
        break;
    case 2:
        imageEncodeSettings.setQuality(QMultimedia::EncodingQuality::NormalQuality);
        break;
    case 3:
        imageEncodeSettings.setQuality(QMultimedia::EncodingQuality::HighQuality);
        break;
    case 4:
        imageEncodeSettings.setQuality(QMultimedia::EncodingQuality::VeryHighQuality);
        break;
    }
    imageCapture->setEncodingSettings(imageEncodeSettings);
}

void MainWindow::on_btnSaveImage_clicked()
{
    QString title="选择保存路径";
    QString curPath=QDir::currentPath();
    QString filename=QFileDialog::getExistingDirectory(this,title,curPath);
    ui->lineEdit->setText(filename);
}

void MainWindow::on_btnExit_clicked()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this,"Error","请先选择保存路径");
        return;
    }

    if(count<0)
    {
        QMessageBox::critical(this,"Error","请先截图");
    }
    else {
        QString filename=ui->lineEdit->text();
         CapturedImgae.save(filename+"/"+QString::number(count)+".jpeg");
    }

}
