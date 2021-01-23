#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioRecorder>
#include <QtMultimedia/QAudioProbe>

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
    QAudioRecorder *recorder;//音频录音
    QAudioProbe *probe;//音频探测器；可以获取音频输入缓冲区的参数和原始文件；包含缓冲区大小，帧数，采样数，采样字长，采样率；
private slots:
    void on_stateChanged(QMediaRecorder::State state);
    void on_DurationChanged(qint64 duration);
    void ProcssBuffer(const QAudioBuffer &buffer);//音频缓冲

    void on_actRecord_triggered();
    void on_actPause_triggered();
    void on_actStop_triggered();
    void on_btnGetFile_clicked();
};
#endif // MAINWINDOW_H
