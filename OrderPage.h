#ifndef ORDERPAGE_H
#define ORDERPAGE_H

#include <QDate>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QDateEdit>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QToolButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "delegate.h"

#define ORDER_ID 0
#define ORDER_NUMBER 1
#define ORDER_CUSTOM 2
#define ORDER_SALE 3
#define ORDER_AREA 4
#define ORDER_QUANTITY 5
#define ORDER_DATE 6
#define ORDER_STOCK 7
#define ORDER_PRODUCE 8
#define ORDER_LACK 9
#define ORDER_DELIVERY 10

class OrderPage : public QWidget
{
    Q_OBJECT
public:
    explicit OrderPage(QWidget *parent = 0);
    ~OrderPage();

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabOrder();
    void autoNumber();
    void appendOrder();
    void deleteOrder();
    void changeOrder();
    void updateOrder();
    void tabSync(QModelIndex index);
    void recvSocket(QUrl url);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_order;
    StandardSqlModel *sql_order;
    StandardSqlModel *sql_custs;
    StandardSqlModel *sql_sales;
    QTableView *tab_order;
    QTableView *tab_iorder;
    QWidget *orderWidget;
    QToolButton *btn_order;
    QSqlDatabase db;
    ComboBoxDelegate *sale_delegate;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *cust_delegate;
    QStringList order_items;
};

#endif // ORDERPAGE_H
