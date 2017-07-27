#ifndef ORDERPAGE_H
#define ORDERPAGE_H

#include <QDate>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QDateEdit>
#include <QSplitter>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QToolButton>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "delegate.h"

#define ORDER_ID 0
#define ORDER_GUID 1
#define ORDER_SIGN 2
#define ORDER_NUMB 3
#define ORDER_DATE 4
#define ORDER_AREA 5
#define ORDER_SALE 6
#define ORDER_CUST 7
#define ORDER_VIEW 8
#define ORDER_QUAN 9
#define ORDER_DEAD 10
#define ORDER_MARK 11
#define ORDER_PROD 12
#define ORDER_STCK 13
#define ORDER_LNUM 14
#define ORDER_DNUM 15

#define SEND_ID 0
#define SEND_GUID 1
#define SEND_SIGN 2
#define SEND_NUMB 3
#define SEND_VIEW 4
#define SEND_MODE 5
#define SEND_CODE 6
#define SEND_PRCE 7
#define SEND_MARK 8

class OrderPage : public QWidget
{
    Q_OBJECT
public:
    explicit OrderPage(QWidget *parent = 0);
    ~OrderPage();

signals:
    void sendSocket(QUrl url);
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initSql();
    void showTabOrder();
    void showTabSends();
    void autoNumber();
    void appendOrder();
    void deleteOrder();
    void changeOrder();
    void appendSends();
    void deleteSends();
    void changeSends();
    void updateOrder();
    void updateSends();
    void updateCusts();
    void updateSales();
    void tabOrderSync(QModelIndex index);
    void tabSendsSync(QModelIndex index);
    void recvOrderJson(QJsonObject obj);
    void recvSendsJson(QJsonObject obj);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_order;
    StandardSqlModel *sql_order;
    QTableView *tab_order;
    QTableView *tab_iorder;
    QWidget *orderWidget;
    QToolButton *btn_order;
    QStringList order_items;

    StandardItemModel *m_sends;
    StandardSqlModel *sql_sends;
    QTableView *tab_sends;
    QTableView *tab_isend;
    QWidget *sendsWidget;
    QToolButton *btn_sends;
    QStringList sends_items;

    StandardSqlModel *sql_custs;
    StandardSqlModel *sql_sales;

    QSqlDatabase db;
    ComboBoxDelegate *sale_delegate;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *cust_delegate;

};

#endif // ORDERPAGE_H
