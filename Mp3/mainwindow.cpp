#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(750,550);
//   ui->sliderVolumn->setFixedSize(250,25);
//   ui->sliderPosition->setFixedSize(550,25);
    ui->setupUi(this);
    player=new QMediaPlayer(this);
    playlList=new QMediaPlaylist(this);

    //列表播放模式
    /*CurrentItemOnce, CurrentItemInLoop, Sequential, Loop, Random */
    playlList->setPlaybackMode(QMediaPlaylist::Loop);//对列表里面的内容随机播放：

    player->setPlaylist(playlList);//设置播放列表

    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(on_stateChanged(QMediaPlayer::State)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(on_positionChanged(qint64)));

    connect(player,SIGNAL(durationChanged(qint64)),SLOT(on_durationChanged(qint64)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stateChanged(QMediaPlayer::State state)
{
    //播放器状态变化，更新按钮状态
    ui->btnPlay->setEnabled(!(state==QMediaPlayer::PlayingState));//非播放状态有效
    ui->btnStop->setEnabled(state==QMediaPlayer::PlayingState);//处于播放状态时有效
    ui->btnPause->setEnabled(state==QMediaPlayer::PlayingState);//同上
}

void MainWindow::on_playListChanged(int position)
{
    //播放列表变化，更新当前播放文件名显示
    ui->listWidget->setCurrentRow(position);//当前行即当前位置
    QListWidgetItem *item=ui->listWidget->currentItem();
    if(item)
    {
        ui->LabCurMedia->setText(item->text());
    }
}

void MainWindow::on_durationChanged(qint64 duration)
{
    ui->sliderPosition->setMaximum(duration);//总时长

    int secs=duration/1000;//ms->秒
    int mins=secs/60;//分钟
    secs=secs % 60;//剩下的秒数

    durationTime=QString::asprintf("%.2d:%.2d",mins,secs);
    ui->LabRatio->setText(currentPosition+"/"+durationTime);//显示当前时间/总时长,在切换歌曲的时候用

}

void MainWindow::on_positionChanged(qint64 position)
{
    //当前文件播放位置变化，更新进度
    if(ui->sliderPosition->isSliderDown())//更改向下滚动条的属性会发出sliderPressed（）和sliderReleased（）信号
        return;
    ui->sliderPosition->setSliderPosition(position);

    int sec=position/1000;//ms->s
    int min=sec/60;//min
    sec=sec % 60;
    currentPosition=QString::asprintf("%.2d:%.2d",min,sec);
    ui->LabRatio->setText(currentPosition+"/"+durationTime);//在一个歌里面用
}


void MainWindow::on_btnAdd_clicked()
{
    //添加文件
    QString currPath=QDir::currentPath();//当前路径
    QString title="选择音频文件";
    QString format="mp3文件(*.mp3);;wav文件(*.mav)";
    QStringList fileList=QFileDialog::getOpenFileNames(this,title,currPath,format);//获得播放列表

    if(fileList.count()<1)
    {
        return;
    }

    for(int i=0;i<fileList.count();i++)
    {
        QString aFile=fileList.at(i);
        QFileInfo fileInfo(aFile);
        int itemCount=ui->listWidget->count();

        for(int j=0;j<itemCount;j++)
        {
            if(fileInfo.fileName()==ui->listWidget->item(i)->text())
            {
                QString title="文件已存在";
                QString infor=fileInfo.fileName()+"已经包含在播放列表中";
                QMessageBox::information(this,title,infor,QMessageBox::Ok,QMessageBox::NoButton);
                return;
            }

        }

        playlList->addMedia(QUrl::fromLocalFile(aFile));//添加文件
        ui->listWidget->addItem(fileInfo.fileName()); //添加到界面文件列表中

        if(player->state()==QMediaPlayer::PlayingState)
            playlList->setCurrentIndex(0);//重头播放

        player->play();//播放
    }
}

void MainWindow::on_btnPlay_clicked()
{//播放
    if(playlList->currentIndex()<0)//没有文件
        playlList->setCurrentIndex(0);
    player->play();
}

void MainWindow::on_btnPause_clicked()
{//暂停
    player->pause();
}

void MainWindow::on_btnStop_clicked()
{//停止播放
    player->stop();
}

void MainWindow::on_btnPrevious_clicked()
{//播放前一个文件
    playlList->previous();
}

void MainWindow::on_btnNext_clicked()
{
    playlList->next();//播放下一个文件
}



void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    //双击时切换播放文件
    int rowNo=index.row();
    playlList->setCurrentIndex(rowNo);
    player->play();
}

void MainWindow::on_btnClear_clicked()
{//清空列表
    playlList->clear();
    ui->listWidget->clear();
    player->stop();
}

void MainWindow::on_sliderVolumn_valueChanged(int value)
{
    player->setVolume(value);//设置音量大小
}

void MainWindow::on_btnSound_clicked()
{
    bool mute=player->isMuted();//静音时返回true
    player->setMuted(!mute);
    if(mute)
    {
        ui->btnSound->setIcon(QIcon(":/images/images/volumn.bmp"));
    }
    else {
        ui->btnSound->setIcon(QIcon(":/images/images/mute.bmp"));
    }
}

void MainWindow::on_sliderPosition_valueChanged(int value)
{
    player->setPosition(value);
}

void MainWindow::on_btnRemove_clicked()
{
    //移除一个文件
    int pos=ui->listWidget->currentRow();
    QListWidgetItem *item=ui->listWidget->takeItem(pos);//移除
    delete item;//删除item,但未从播放列表中删除，所以仍可播放

    if(playlList->currentIndex()==pos)//删除的是当前播放的曲目
    {
        int nextPose=0;
        if(pos>1)
        {
            nextPose=pos-1;
        }
        playlList->removeMedia(pos);//从播放列表中删除

        if(ui->listWidget->count()>0)
        {
            playlList->setCurrentIndex(nextPose);
            on_playListChanged(nextPose);//playList列表更新
        }
        else
        {
            player->stop();
            ui->LabCurMedia->setText("请添加音频文件");
        }

    }
    else
    {
        playlList->removeMedia(pos);
    }

}
