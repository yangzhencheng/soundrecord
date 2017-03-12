#include "waitfor.h"
#include "ui_waitfor.h"
#include <QMessageBox>
#include <QProcess>

waitFor::waitFor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waitFor)
{
    this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint&~Qt::WindowCloseButtonHint);

    ui->setupUi(this);

    wTimer = new QTimer(this);
}

waitFor::~waitFor()
{
    delete ui;
}

void waitFor::getMainWindow(MainWindow *w){
    mainwindow = w;
}


void waitFor::runThread(QString zipRecord){
    lameThread *lt = new lameThread(zipRecord);
    lt->start();

    wTimer->start(1000);
    connect(wTimer, SIGNAL(timeout()), this, SLOT(runCompleteExec()));
}

void waitFor::runCompleteExec(){
    if(100 == iSign){
        closeit();
    }
}


void waitFor::closeit(){
    wTimer->stop();
    QMessageBox::about(NULL, tr("信息"), tr("压制成功！"));
    mainwindow->setEnabled(true);
    this->close();
}

lameThread::lameThread(QString zipRecord){
    _zipRecord = zipRecord;
}

void lameThread::run(){
    // QProcess::execute(QString("lame.exe --abr 16 E:\\E\\a.wav E:\\E\\k.mp3"));
    QProcess::execute(_zipRecord);
    iSign = 100;
}
