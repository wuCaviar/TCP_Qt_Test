#include "recvfile.h"
#include <QFile>

RecvFile::RecvFile(QTcpSocket* tcp, QObject *parent)
    : QThread{parent}
{
    m_tcp = tcp;
}

void RecvFile::run()
{
    QFile* file = new QFile("recv.txt");
    file->open(QIODevice::WriteOnly);

    //接收数据
    connect(m_tcp, &QTcpSocket::readyRead, this, [=](){
        static int count = 0;
        static int total = 0;
        if (count == 0)
        {
            m_tcp->read((char*)&total, 4);
        }
        
        //读取数据
        QByteArray all = m_tcp->readAll();
        count += all.size();
        file->write(all);

        if (count == total)
        {
            m_tcp->close();
            m_tcp->deleteLater();
            file->close();
            file->deleteLater();
            emit over();
        }
    });

    //进入事件循环
    exec();
}
