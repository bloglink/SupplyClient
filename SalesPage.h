#ifndef SALESPAGE_H
#define SALESPAGE_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSplitter>
#include <QToolButton>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QAbstractItemView>
#include <QSqlQuery>

#include "delegate.h"

#define CUST_ID 0
#define CUST_GUID 1
#define CUST_SIGN 2
#define CUST_NAME 3
#define CUST_SALE 4
#define CUST_AREA 5

#define SALE_ID 0
#define SALE_GUID 1
#define SALE_SIGN 2
#define SALE_NAME 3
#define SALE_AREA 4

class SalesPage : public QWidget
{
    Q_OBJECT
public:
    explicit SalesPage(QWidget *parent = 0);
    ~SalesPage();

signals:
    void sendSocket(QUrl url);
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabCust();
    void showTabSale();
    void tabCustSync(QModelIndex index);
    void tabSaleSync(QModelIndex index);
    void appendCust();
    void deleteCust();
    void changeCust();
    void updateCust();
    void appendSale();
    void deleteSale();
    void changeSale();
    void updateSale();
    void recvSocket(QUrl url);
    void recvSalesJson(QJsonObject obj);
    void recvCustsJson(QJsonObject obj);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_custs;
    StandardItemModel *m_sales;
    StandardSqlModel *sql_custs;
    StandardSqlModel *sql_sales;
    QTableView *tab_custs;
    QTableView *tab_icust;
    QTableView *tab_sales;
    QTableView *tab_isale;
    QToolButton *btn_custs;
    QToolButton *btn_sales;
    QWidget *custWidget;
    QWidget *saleWidget;
    QSqlDatabase db;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *sale_delegate;
    QStringList cust_items;
    QStringList sale_items;
};

#endif // SALESPAGE_H
