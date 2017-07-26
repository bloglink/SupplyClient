#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QFile>
#include <QMainWindow>
#include <QApplication>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSqlTableModel>
#include <QSqlError>

#include "UdpSocket.h"

#include "LoginPage.h"
#include "AboutPage.h"
#include "HumanPage.h"
#include "SalesPage.h"
#include "OrderPage.h"
#include "ProdsPage.h"
#include "PurchPage.h"

class MainScreen : public QMainWindow
{
    Q_OBJECT

public:
    MainScreen(QWidget *parent = 0);
    ~MainScreen();
signals:
    void sendSocket(QUrl url);
    void sendMsg(QUrl url);
    void loginJson(QJsonObject obj);
    void rolesJson(QJsonObject obj);
    void usersJson(QJsonObject obj);
    void salesJson(QJsonObject obj);
    void custsJson(QJsonObject obj);
    void orderJson(QJsonObject obj);
    void prodsJson(QJsonObject obj);
    void sendJson(QJsonObject obj);
public:
    int login();
private slots:
    void initUI();
    void initUdp(QJsonObject obj);
    void initSql();
    void swithMaxNormal();
    void initToolButton(QToolButton *btn);
    void readToolButton();
    void stackChange(QByteArray win);
    void animationHide();
    void animationClose();
    void recvSocket(QUrl url);
    void recvNetJson(QJsonObject obj);

    virtual void cloudAntimation();
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
private:
    bool leftbuttonpressed;
    QPoint dragPosition;
    QStackedWidget *stack;
    UdpSocket udp;

    int preindex;
    HumanPage *human;
    SalesPage *sales;
    OrderPage *order;
    ProdsPage *prods;
    PurchPage *purch;
    QSqlDatabase db;
    bool isLogin;
    QHostAddress addr;
};

#endif // MAINSCREEN_H
