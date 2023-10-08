#include "sendfile.h"

SendFile::SendFile(QObject *parent) : QObject(parent)
{

}

void SendFile::connectServer(unsigned short port, QString ip)
{
    m_tcp = new QTcpSocket;
    m_tcp->connectToHost(QHostAddress(ip), port);

    connect(m_tcp, &QTcpSocket::connected, this, &SendFile::connectOK);
    connect(m_tcp, &QTcpSocket::disconnected, this, [=](){
        m_tcp->close();
        m_tcp->deleteLater();
        emit gameOver();
    });
}

void SendFile::sendFile(QString path)
{
    QFile file(path);
    QFileInfo info(path);
    int fileSize = info.size();
    file.open(QIODevice::ReadOnly);

    while (!file.atEnd())
    {
        static int index = 0;
        if (index == 0){
            m_tcp->write((char*)&fileSize, 4);
        }
        QByteArray line = file.readLine();
        index += line.size();
        int progress = index * 100 / fileSize;
        emit curPercent(progress);
        
        m_tcp->write(line);
    }
    
}
