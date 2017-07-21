/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#include "UdpSocket.h"

UdpSocket::UdpSocket(QObject *parent) : QUdpSocket(parent)
{

}

void UdpSocket::initSocket()
{
    this->bind(QHostAddress::AnyIPv4,LOCAL_PORT);
    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket()));
    addr = QHostAddress(getLocalHostIP());
    uid = getUid();
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Delay(int)));
    timer->start(10);
}

void UdpSocket::quitSocket()
{
    this->close();
}

void UdpSocket::readSocket()
{
    while (this->hasPendingDatagrams()) {
        QByteArray msg;
        msg.resize(this->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        this->readDatagram(msg.data(), msg.size(), &sender, &senderPort);
        qDebug() << sender.toString() << msg;

        QUrl url(QByteArray::fromBase64(msg));
        emit recvSocket(url);
    }
}

void UdpSocket::excuteMessage()
{
    if (!send_queue.isEmpty())
        sendSocket(send_queue.dequeue());
}

void UdpSocket::sendSocket(QUrl url)
{
    QString cmd = url.query();
    if (cmd == "login") {
        url.setFragment(uid.toUtf8().toBase64());
        userinfo = url;
    }
    QHostAddress host = QHostAddress(userinfo.host());
    int port = userinfo.port();
    url.setScheme(VERSION);
    url.setUserInfo(userinfo.userInfo());
    url.setHost(addr.toString());
    url.setPort(LOCAL_PORT);
    url.setPath(QString("/%1").arg(getUid()));
    QByteArray msg = url.toString().toUtf8();
    this->writeDatagram(msg, QHostAddress::Broadcast, port);
}

QString UdpSocket::getLocalHostIP()
{
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    QHostAddress result;
    foreach(QHostAddress address,  AddressList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::Null &&
                address != QHostAddress::LocalHost) {
            if (address.toString().contains("127.0."))
                continue;
            result = address;
            break;
        }
    }
    return result.toString();
}

QString UdpSocket::getUid()
{
    return QUuid::createUuid().toString().mid(1,6);
}

void UdpSocket::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}
/*********************************END OF FILE**********************************/
