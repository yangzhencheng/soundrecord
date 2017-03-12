#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTime>
#include <QProcess>

#include "waitfor.h"


int rSign = 0;
QStandardItemModel *setTableBuild = new QStandardItemModel();
QTimer *timer;
QTime rtime;
QAudioEncoderSettings audioSettings;
QString savefileName = "";

// 记录文本
void outputRecordFile(){
    QFile f(savefileName + ".txt");
    f.open(QIODevice::WriteOnly|QIODevice::Text);

    QTextStream s(&f);

    for(int row = 0; row < setTableBuild->rowCount(); row++){
        for(int column = 0; column < setTableBuild->columnCount(); column++){
             s << setTableBuild->item(row, column)->text() << "\t";
        }

        s << "\n";
    }

    s.flush();
    f.close();
}



// 类方法实现
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);

    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(qtimeSlot()));

    setTableBuild->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("序号")));
    setTableBuild->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("开始时间")));
    setTableBuild->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("结束时间")));

    ui->tabShowTime->setModel(setTableBuild);
    ui->tabShowTime->setEditTriggers(QAbstractItemView::NoEditTriggers);

    audioRecorder = new QAudioRecorder;
    audioSettings.setCodec("audio/wav");

    if(savefileName != "") ui->txtSavePath->setText(savefileName + ".txt");

    ui->labinfo->setText("<span style='color: red;'>高音质指原声波录制，低音质是有损压缩，比高音质低五倍！</span>");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnExec_clicked()
{
    if(ui->btnExec->text() == "开始录音"){
        if(savefileName == ""){
            QMessageBox::information(NULL, "警告", "请先选择保存文件！", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }

        ui->btnExec->setText("结束录音");
        ui->btnrtable->setEnabled(true);

        audioRecorder->setOutputLocation((QUrl::fromLocalFile(savefileName + ".wav")));


        // 设置音质为高音质
        audioSettings.setQuality(QMultimedia::HighQuality);

        ui->gbSet->setEnabled(false);

        audioRecorder->setEncodingSettings(audioSettings);
        audioRecorder->record();

        rtime.start();

        if(ui->cbkSynchronous->isChecked()){
            on_btnrtable_clicked();
        }
    }
    else{
        if(QMessageBox::question(this, tr("询问"), tr("完成录音？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes){
            ui->btnExec->setText("开始录音");
            ui->gbSet->setEnabled(true);
            ui->btnrtable->setEnabled(false);

            audioRecorder->stop();

            // 压制录音
            if(true == ui->radL->isChecked()){
                on_zipvideo();
            }
        }
    }
}

void MainWindow::on_btnrtable_clicked()
{
    if(ui->btnrtable->text() == "起始时间"){
        ui->btnrtable->setText("结束时间");

        setTableBuild->setItem(rSign, 0, new QStandardItem(QString::number(rSign + 1)));
        setTableBuild->setItem(rSign, 1, new QStandardItem(ui->labTime->text().replace(0, 4, "")));
        setTableBuild->setItem(rSign, 2, new QStandardItem(""));
    }
    else{
        ui->btnrtable->setText("起始时间");
        setTableBuild->setItem(rSign, 2, new QStandardItem(ui->labTime->text().replace(0, 4, "")));

        rSign++;
    }

    outputRecordFile();
}

void MainWindow::qtimeSlot(){
    if(ui->btnExec->text() == "结束录音"){
        long timeline = rtime.elapsed() / 1000;

        int hour = timeline / 3600;
        int minute = (timeline - (hour * 3600))/ 60;
        int second = timeline - hour * 3600 - minute * 60;

        QString shour = QString::number(hour);
        if(shour.length() == 1) shour.prepend("0");

        QString sminute = QString::number(minute);
        if(sminute.length() == 1) sminute.prepend("0");

        QString ssecond = QString::number(second);
        if(ssecond.length() == 1) ssecond.prepend("0");


        ui->labTime->setText(tr("时间轴：") + shour + ":" + sminute + ":" + ssecond);
    }
}


void MainWindow::on_btnOutput_clicked()
{
    outputRecordFile();
    QMessageBox::information(NULL, "提示", "导出成功！", QMessageBox::Yes, QMessageBox::Yes);
}


void MainWindow::on_btnSavePath_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存对话框"), "", tr("SaveName (*.txt)"));

    if(fileName.isNull()){
        return;
    }

    savefileName = fileName.left(fileName.length() - 4);
    ui->txtSavePath->setText(savefileName + ".txt");
}

void MainWindow::on_btnAboutUs_clicked()
{
    QMessageBox::information(NULL, "关于", "程序名称：共修记录器 V1.1\n版权所有：智悲网络共修技术组\n开发人员：MysteriousMan\n联系邮箱：yangzhencheng_001@163.com\n授权方式：LGPL");
}

void MainWindow::on_radQ_clicked()
{
    if(true == ui->radQ->isChecked()){
        ui->radL->setChecked(false);
    }
}

void MainWindow::on_radL_clicked()
{
    if(true == ui->radL->isChecked()){
        ui->radQ->setChecked(false);
    }
}

void MainWindow::on_zipvideo(){
    // QStringList list;
    // QMessageBox::about(NULL, "1", savefileName.replace("/", "\\") + ".wav");
    // list << "--abr 16" << savefileName.replace("/", "\\") + ".wav" << savefileName.replace("/", "\\") + ".mp3";
    QString zipRecord = "lame.exe --abr 16 " + savefileName.replace("/", "\\") + ".wav" + " " + savefileName.replace("/", "\\") + ".mp3";


    // QProcess::execute(QString("lame.exe --abr 16 " + savefileName.replace("/", "\\") + ".wav" + " " + savefileName.replace("/", "\\") + ".mp3"));
    // QProcess::execute(QString("lame.exe --abr 16 E:\\E\\a.wav E:\\E\\k.mp3"));
    // QMessageBox::information(NULL, "qqq", "qqqq", QMessageBox::Yes, NULL);
    waitFor *waitForm = new waitFor();
    waitForm->show();
    waitForm->getMainWindow(this);
    waitForm->runThread(zipRecord);

    this->setEnabled(false);

    //QProcess::execute(tr("lame.exe"), list);
}

