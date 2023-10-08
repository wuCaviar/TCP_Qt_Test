#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->port->setText("8899");

    setWindowTitle("服务器");
    
    //创建监听的服务器对象
    m_s = new QTcpServer(this);

    connect(ui->setListen, &QPushButton::clicked, this, &MainWindow::on_setListen_clicked);
    connect(ui->sendMsg, &QPushButton::clicked, this, &MainWindow::on_sendMsg_clicked);
    
    connect(m_s, &QTcpServer::newConnection, this, [=](){
        m_tcp = m_s->nextPendingConnection();
        ui->record->append("已成功连接客户端......");
        m_status->setText("客户端已连接");
        // 检查是否有数据可读
        connect(m_tcp, &QTcpSocket::readyRead, this, [=](){
            QByteArray data = m_tcp->readAll();
            ui->record->append("客户端say:" + data);
        });

        connect(m_tcp, &QTcpSocket::disconnected, this, [=](){
            m_tcp->close();
            m_tcp->deleteLater();
            m_status->setText("客户端已断开");
        });
    });

    // 状态栏显示
    m_status = new QLabel;
    m_status->setText("服务器未启动");
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
    ui->record->append("服务器say:" + msg);
    ui->msg->clear();
}

void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort(); // 使用toUShort()获取端口号

    if (m_s->isListening())
    {
        // 如果已经在监听，则不执行任何操作
        return;
    }

    // 尝试开始监听
    if (m_s->listen(QHostAddress::Any, port))
    {
        // 监听成功，更新界面元素
        ui->setListen->setEnabled(false);
        ui->port->setEnabled(false);
        m_status->setText("服务器正在监听端口 ");
    }
    else
    {
        // 监听失败，显示错误消息
        QMessageBox::critical(this, "错误", "无法开始监听端口 " + m_s->errorString());
    }
}


