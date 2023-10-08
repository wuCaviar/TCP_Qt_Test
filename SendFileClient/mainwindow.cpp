#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include "sendfile.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "主线程号：" << QThread::currentThread();

    ui->ip->setText("127.0.0.1");
    ui->port->setText("8899");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    //创建线程对象
    QThread* thread = new QThread;
    //创建任务对象
    SendFile* worker = new SendFile;

    //把任务添加到线程
    worker->moveToThread(thread);

    connect(ui->connectServer, &QPushButton::clicked, this, &MainWindow::on_connectServer_clicked);
    connect(ui->selFile, &QPushButton::clicked, this, &MainWindow::on_selFile_clicked);
    connect(ui->sendFile, &QPushButton::clicked, this, &MainWindow::on_sendFile_clicked);

    connect(this, &MainWindow::sendFile, worker, &SendFile::sendFile);

    connect(this, &MainWindow::startConnect, worker, &SendFile::connectServer);

    //连接信号槽
    connect(worker, &SendFile::connectOK, this, [=](){
        QMessageBox::information(this, "提示", "连接成功");
    });

    connect(worker, &SendFile::gameOver, this, [=](){
        // 资源回收
        thread->quit();
        thread->wait();
        worker->deleteLater();
        thread->deleteLater();
    });

    connect(worker, &SendFile::curPercent, ui->progressBar, &QProgressBar::setValue);

    //启动线程
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectServer_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();

    emit startConnect(port, ip);
}

void MainWindow::on_selFile_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    // 判断
    if(path.isEmpty())
    {
        QMessageBox::information(this, "打开文件", "选择的文件路径不能为空");
        return;
    }
    ui->filePath->setText(path);
}

void MainWindow::on_sendFile_clicked()
{
    emit sendFile(ui->filePath->text());
}   
