#ifndef WAITFOR_H
#define WAITFOR_H

#include <QWidget>
#include <QThread>
#include <QTimer>
#include "mainwindow.h"

namespace Ui {
class waitFor;
}

static int iSign = 0;

class waitFor : public QWidget
{
    Q_OBJECT

public:
    explicit waitFor(QWidget *parent = 0);
    void getMainWindow(MainWindow *w);
    void runThread(QString zipRecord);
    void closeit();
    ~waitFor();

private slots:
    void runCompleteExec();


private:
    MainWindow *mainwindow;
    QTimer *wTimer;

    Ui::waitFor *ui;
};

class lameThread : public QThread{
public:
    lameThread(QString zipRecord);
protected:
    void run();

private:
    waitFor *mf;
    QString _zipRecord;
};

#endif // WAITFOR_H
