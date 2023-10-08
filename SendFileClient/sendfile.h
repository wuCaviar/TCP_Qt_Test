#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>

class SendFile : public QObject
{
    Q_OBJECT
public:
    SendFile(QObject *parent = nullptr);

    //连接服务器
    void connectServer(unsigned short port, QString ip);

    //发送文件
    void sendFile(QString path);
signals:
    void connectOK();
    void gameOver();
    void curPercent(int);
private:
    QTcpSocket* m_tcp;

};

#endif // SENDFILE_H
