#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioRecorder>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnExec_clicked();

    void on_btnrtable_clicked();

    void qtimeSlot();

    void on_btnOutput_clicked();

    void on_btnSavePath_clicked();

    void on_btnAboutUs_clicked();

    void on_radQ_clicked();

    void on_radL_clicked();

    void on_zipvideo();

private:
    Ui::MainWindow *ui;
    QAudioRecorder *audioRecorder;
};

#endif // MAINWINDOW_H
