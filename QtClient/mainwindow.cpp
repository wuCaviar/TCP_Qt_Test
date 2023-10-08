#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->port->setText("8899");
    ui->ip->setText("127.0.0.1");
    setWindowTitle("客户端");

    ui->disconnect->setDisabled(true);

    //创建监听的客户端对象
    m_tcp = new QTcpSocket(this);

    connect(ui->connect, &QPushButton::clicked, this, &MainWindow::on_connect_clicked);
    connect(ui->disconnect, &QPushButton::clicked, this, &MainWindow::on_disconnect_clicked);
    connect(ui->sendMsg, &QPushButton::clicked, this, &MainWindow::on_sendMsg_clicked);
    
    connect(m_tcp, &QTcpSocket::readyRead, this, [=](){
        QByteArray data = m_tcp->readAll();
        ui->record->append("服务器say:" + data); 
    });

    connect(m_tcp, &QTcpSocket::disconnected, this, [=](){
        m_tcp->close();
        m_tcp->deleteLater();
        m_status->setText("客户端已断开");
        ui->record->append("已断开服务器连接......");

        ui->connect->setEnabled(true);
    });

    connect(m_tcp, &QTcpSocket::connected, this, [=](){
        m_status->setText("客户端已连接");
        ui->record->append("已成功连接服务器......");

        ui->connect->setDisabled(true);
        ui->disconnect->setEnabled(true);
    });

    // 状态栏显示
    m_status = new QLabel;
    m_status->setText("客户端未连接");
    ui->statusbar->addWidget(new QLabel("服务器状态:"));
    ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("客户端say:" + msg);
}

void MainWindow::on_connect_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    m_tcp->connectToHost(QHostAddress(ip), port);
}

void MainWindow::on_disconnect_clicked()
{
    m_tcp->close();
    ui->connect->setDisabled(false);
    ui->disconnect->setEnabled(false);
}