#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recvfile.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "主线程：" << QThread::currentThread();

    m_s = new QTcpServer(this);

    connect(m_s, &QTcpServer::newConnection, this, [=](){
        QTcpSocket* tcp = m_s->nextPendingConnection();

        //创建线程对象
        RecvFile* subThread = new RecvFile(tcp);
        subThread->start();

        //接收子线程的信号
        connect(subThread, &RecvFile::over, this, [=](){
            subThread->quit();
            subThread->wait();
            subThread->deleteLater();
            QMessageBox::information(this, "文件接收", "文件接收完成！！！");
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any, port);
}

