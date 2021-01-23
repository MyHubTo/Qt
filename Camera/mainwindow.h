#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraInfo>
#include <QtMultimedia/QCameraImageCapture>
#include <QCameraViewfinder>
#include <QMediaRecorder>
#include <QSound>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
//    void on_btnStart_clicked();

//    void on_btnStop_clicked();

//    void on_btnCapture_clicked();

private:
    Ui::MainWindow *ui;

    QCamera *curCamera;
    QMediaRecorder *mediaRecorder;

    QCameraViewfinder *cameraviewfinder;
    QCameraImageCapture *imageCapture;

    int count=-1;
    QImage CapturedImgae;
    QSound *sound;

private slots:
    void on_imageCaptured(int id,const QImage &preview);
    void on_actionExit_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionCapture_triggered();
    void on_comboBox_Quality_currentIndexChanged(int index);
    void on_btnSaveImage_clicked();
    void on_btnExit_clicked();
};
#endif // MAINWINDOW_H
