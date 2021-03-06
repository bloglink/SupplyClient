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
#include "WorksPage.h"

class MainScreen : public QMainWindow
{
    Q_OBJECT

public:
    MainScreen(QWidget *parent = 0);
    ~MainScreen();
signals:
    void transmitJson(QJsonObject obj);
    void transmitShow(QString win);
    void loginJson(QJsonObject obj);
    void rolesJson(QJsonObject obj);
    void usersJson(QJsonObject obj);
    void salesJson(QJsonObject obj);
    void custsJson(QJsonObject obj);
    void orderJson(QJsonObject obj);
    void sendsJson(QJsonObject obj);
    void prodsJson(QJsonObject obj);
    void worksJson(QJsonObject obj);
    void sendJson(QJsonObject obj);
public:
    int login();
private slots:
    void initUI();
    void initUdp(QJsonObject obj);
    void initSql();
    void createTabRoles();
    void excuteCmdRoles(QJsonObject obj);
    void createTabUsers();
    void excuteCmdUsers(QJsonObject obj);
    void createTabSales();
    void excuteCmdSales(QJsonObject obj);
    void createTabCusts();
    void excuteCmdCusts(QJsonObject obj);
    void createTabOrder();
    void excuteCmdOrder(QJsonObject obj);
    void createTabSends();
    void excuteCmdSends(QJsonObject obj);
    void createTabPlans();
    void excuteCmdPlans(QJsonObject obj);
    void createTabProds();
    void excuteCmdProds(QJsonObject obj);
    void createTabPurch();
    void swithMaxNormal();
    void initToolButton(QToolButton *btn);
    void readToolButton();
    void stackChange(QByteArray win);
    void animationHide();
    void animationClose();
    void recvNetJson(QJsonObject obj);
    void recvSqlupdate(QString sql);

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
    WorksPage *works;
    QSqlDatabase db;
    bool isLogin;
    QHostAddress addr;
};

#endif // MAINSCREEN_H
