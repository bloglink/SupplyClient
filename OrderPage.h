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

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "delegate.h"

#define ORDER_ID 0
#define ORDER_NUMB 1
#define ORDER_DATE 2
#define ORDER_VIEW 3
#define ORDER_CUST 4
#define ORDER_SALE 5
#define ORDER_AREA 6
#define ORDER_DEAD 7
#define ORDER_QUAN 8
#define ORDER_PROD 9
#define ORDER_STCK 10
#define ORDER_DNUM 11

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
    void initData();
    void showTabOrder();
    void autoNumber();
    void appendOrder();
    void deleteOrder();
    void changeOrder();
    void updateOrder();
    void tabSync(QModelIndex index);
    void recvOrderJson(QJsonObject obj);
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
