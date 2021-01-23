#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QUrl>
#include <QAudioEncoderSettings>
#include <QDir>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    recorder=new QAudioRecorder(this);
    probe=new QAudioProbe;

    connect(recorder,SIGNAL(stateChanged(QMediaRecorder::State)),
            this,SLOT(on_stateChanged(QMediaRecorder::State)));

    connect(probe,SIGNAL(audioBufferProbed(QAudioBuffer)),
            this,SLOT(ProcssBuffer(QAudioBuffer)));
    probe->setSource(recorder);

    connect(recorder,SIGNAL(durationChanged(qint64)),
            this,SLOT(on_DurationChanged(qint64)));

    if(recorder->defaultAudioInput().isEmpty())//如果没有音频录入设备
    {
        return;
    }

    foreach(const QString &device,recorder->audioInput())
    {
        ui->comboDevices->addItem(device);//音频录入设备列表

    }
    foreach(const QString &codeName,recorder->supportedAudioCodecs())
    {
        ui->comboCodec->addItem(codeName);//支持的音频编码
    }
    foreach(int sampleRate,recorder->supportedAudioSampleRates())
    {//支持的采样频率
        ui->comboSampleRate->addItem(QString::number(sampleRate));
    }

    //channel
    ui->comboChannels->addItem("1");
    ui->comboChannels->addItem("2");
    ui->comboChannels->addItem("3");

    //quliaty
    /*VeryLowQuality,LowQuality,NormalQuality, HighQuality, VeryHighQuality*/
    ui->sliderQuality->setRange(0,int(QMultimedia::VeryHighQuality));

    ui->sliderQuality->setValue(int(QMultimedia::NormalQuality));

    //biteates:
    ui->comboBitrate->addItem("32000",32000);
    ui->comboBitrate->addItem("64000",64000);
    ui->comboBitrate->addItem("96000");
    ui->comboBitrate->addItem("1280000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stateChanged(QMediaRecorder::State state)
{
    //录音状态变换
    ui->actRecord->setEnabled(state!=QMediaRecorder::RecordingState);
    ui->actPause->setEnabled(state==QMediaRecorder::RecordingState);
    ui->actStop->setEnabled(state==QMediaRecorder::RecordingState);

    ui->btnGetFile->setEnabled(state==QMediaRecorder::StoppedState);
    ui->editOutputFile->setEnabled(state==QMediaRecorder::StoppedState);
}

void MainWindow::on_DurationChanged(qint64 duration)
{
    //录音持续时间变换
    ui->LabPassTime->setText(QString("已录制 %1秒").arg(duration/1000));
}

void MainWindow::ProcssBuffer(const QAudioBuffer &buffer)
{
    //处理探测到的缓冲区
    ui->spin_byteCount->setValue(buffer.byteCount());//缓冲区字节数
    ui->spin_duration->setValue(buffer.duration());//缓冲区时长
    ui->spin_frameCount->setValue(buffer.frameCount());//缓冲区帧数
    ui->spin_sampleCount->setValue(buffer.sampleCount());//缓冲区采样数

    QAudioFormat audioFormate=buffer.format();//缓冲区格式
    ui->spin_channelCount->setValue(audioFormate.channelCount());//通道数
    ui->spin_sampleSize->setValue(audioFormate.sampleSize());//采样大小
    ui->spin_sampleRate->setValue(audioFormate.sampleRate());//采样率
    ui->spin_bytesPerFrame->setValue(audioFormate.bytesPerFrame());//每一帧的字节数

    if(audioFormate.byteOrder()==QAudioFormat::LittleEndian)
        ui->edit_byteOrder->setText("LittleEndian");//字节序
    else
    {
        ui->edit_byteOrder->setText("BigEndian");
    }

    ui->edit_codec->setText(audioFormate.codec());//编码格式

    if(audioFormate.sampleType()==QAudioFormat::SignedInt)//采样点类型
    {
        ui->edit_sampleType->setText("SignedInt");
    }
    else if(audioFormate.sampleType()==QAudioFormat::UnSignedInt)
    {
        ui->edit_sampleType->setText("UnSigneInt");
    }
    else if (audioFormate.sampleType()==QAudioFormat::Float) {
        ui->edit_sampleType->setText("Float");

    }
    else
    {
        ui->edit_sampleType->setText("UnKnown");
    }
}


void MainWindow::on_actRecord_triggered()
{
    if(recorder->state()==QMediaRecorder::StoppedState)
    {
        QString selectFile=ui->editOutputFile->text().trimmed();
        if(selectFile.isEmpty())
        {
            QMessageBox::critical(this,"错误","请先设置银屏输出文件");
            return;
        }
        if(QFile::exists(selectFile))
        {
            if(!QFile::remove(selectFile))
            {
                QMessageBox::critical(this,"错误","所设置录音输出文件被占用，无法删除");
                return;
            }
        }
        recorder->setOutputLocation(QUrl::fromLocalFile(selectFile));//设置输出路径,当按STop时将音频保存；
        recorder->setAudioInput(ui->comboDevices->currentText());//设置输入设备

        QAudioEncoderSettings settings;//音频编码设置

        settings.setCodec(ui->comboCodec->currentText());//设置编码
        settings.setSampleRate(ui->comboSampleRate->currentText().toInt());//采样率
        settings.setBitRate(ui->comboBitrate->currentText().toInt());//比特率
        settings.setChannelCount(ui->comboChannels->currentText().toInt());//通道数
        settings.setQuality(QMultimedia::EncodingQuality(ui->sliderQuality->value()));

        if(ui->radioQuality->isChecked())//编码模式为固定品质，自动决定采样率，采样点大小
        {
            settings.setEncodingMode(QMultimedia::EncodingMode::ConstantQualityEncoding);
        }
        else
        {
            settings.setEncodingMode(QMultimedia::EncodingMode::ConstantBitRateEncoding);//固定比特率
        }
        recorder->setAudioSettings(settings);
    }
    recorder->record();//开始录音
}

void MainWindow::on_actPause_triggered()
{
    recorder->pause();
}

void MainWindow::on_actStop_triggered()
{
    recorder->stop();
}

void MainWindow::on_btnGetFile_clicked()
{
    QString curPath=QDir::currentPath();
    QString title="选择输出文件";
    QString filter="wav文件(*.wav)";
    QString selectFile=QFileDialog::getSaveFileName(this,title,curPath,filter);

    if(!selectFile.isEmpty())
    {
        ui->editOutputFile->setText(selectFile);
    }
}
