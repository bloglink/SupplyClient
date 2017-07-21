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
    guid.setMachineId(20,20);
    QTimer *t_send = new QTimer(this);
    connect(t_send,SIGNAL(timeout()),this,SLOT(sendJsonMsg()));
    t_send->start(5);
    QTimer *t_recv = new QTimer(this);
    connect(t_recv,SIGNAL(timeout()),this,SLOT(readJsonMsg()));
    t_recv->start(5);
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
        QJsonObject obj = QJsonDocument::fromJson(QByteArray::fromBase64(msg)).object();
        qDebug() << "rcev" << obj;
        recv_queue.enqueue(obj);
    }
}

void UdpSocket::readJson(QJsonObject obj)
{
    send_queue.enqueue(obj);
}

void UdpSocket::sendJsonMsg()
{
    if (!send_queue.isEmpty()) {
        QJsonObject obj = send_queue.dequeue();
        if (obj.value("logs_guid").toDouble() == 0)
            obj.insert("logs_guid",qint64(guid.getId()));
        QByteArray msg = QJsonDocument(obj).toJson();
        this->writeDatagram(msg.toBase64(),QHostAddress::Broadcast,LOCAL_PORT);
        this->waitForBytesWritten();
    }
}

void UdpSocket::readJsonMsg()
{
    if (!recv_queue.isEmpty())
        emit sendJson(recv_queue.dequeue());
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
