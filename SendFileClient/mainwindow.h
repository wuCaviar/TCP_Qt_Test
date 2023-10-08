#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "sendfile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_connectServer_clicked();
    void on_selFile_clicked();
    void on_sendFile_clicked();

signals:
    void startConnect(unsigned short, QString);
    void sendFile(QString);
private:
    Ui::MainWindow *ui;
    QThread* thread;
    SendFile* worker;
};
#endif // MAINWINDOW_H
