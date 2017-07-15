/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#ifndef SocketUdp_H
#define SocketUdp_H

#include <QUrl>
#include <QUuid>
#include <QObject>
#include <QUdpSocket>
#include <QElapsedTimer>
#include <QApplication>
#include <QNetworkInterface>

#define LOCAL_PORT 10000
#define VERSION "erp-0.0.0.1"

class UdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject *parent = 0);

signals:
    void recvSocket(QUrl url);
public:
    void initSocket(void);
    void quitSocket(void);

private slots:
    void sendSocket(QUrl url);
    void readSocket(void);
    QString getLocalHostIP();
    QString getUid();
    void Delay(int ms);
private:
    QHostAddress addr;
    QUrl userinfo;
    QString uid;
};

#endif // SocketUdp_H
