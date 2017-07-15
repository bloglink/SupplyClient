#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QFile>
#include <QMainWindow>
#include <QApplication>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "udpsocket.h"
#include "loginscreen.h"
#include "aboutusscreen.h"
#include "usermanagerment.h"
#include "rolemanagerment.h"
#include "ordermanagement.h"
#include "productionmanagement.h"
#include "lackmanagement.h"

class MainScreen : public QMainWindow
{
    Q_OBJECT

public:
    MainScreen(QWidget *parent = 0);
    ~MainScreen();
signals:
    void sendSocket(QUrl url);
    void sendMsg(QUrl url);
public:
    int login();
private slots:
    void initUI();
    void initUdp();
    void swithMaxNormal();
    void initToolButton(QToolButton *btn);
    void readToolButton();
    void stackChange(QByteArray win);
    void animationHide();
    void animationClose();
    void recvSocket(QUrl url);

    virtual void cloudAntimation();
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
private:
    bool leftbuttonpressed;
    QPoint dragPosition;
    QStackedWidget *stack;
    UdpSocket udp;
    UserManagerment *user;
    RoleManagerment *role;
    OrderManagement *order;
    ProductionManagement *prod;
    LackManagement *lack;
    int preindex;
};

#endif // MAINSCREEN_H
