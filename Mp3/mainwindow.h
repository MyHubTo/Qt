#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
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
    QMediaPlayer *player;//播放器
    QMediaPlaylist *playlList;//播放器列表

    QString durationTime;//总时长
    QString currentPosition;//当前位置
private slots:
    void on_stateChanged(QMediaPlayer::State state);//总共包含三种方式 StoppedState,PlayingState，PausedState
    void on_playListChanged(int position);
    void on_durationChanged(qint64 duration);
    void on_positionChanged(qint64 position);
    void on_btnAdd_clicked();
    void on_btnPlay_clicked();
    void on_btnPause_clicked();
    void on_btnStop_clicked();
    void on_btnPrevious_clicked();
    void on_btnNext_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_btnClear_clicked();
    void on_sliderVolumn_valueChanged(int value);
    void on_btnSound_clicked();
    void on_sliderPosition_valueChanged(int value);
    void on_btnRemove_clicked();
};
#endif // MAINWINDOW_H
