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
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QAbstractItemView>
#include <QSqlQuery>

#include "delegate.h"

#define CUSTS_UUID 0
#define CUSTS_GUID 1
#define CUSTS_SIGN 2
#define CUSTS_NAME 3
#define CUSTS_SALE 4
#define CUSTS_AREA 5

#define SALES_UUID 0
#define SALES_GUID 1
#define SALES_SIGN 2
#define SALES_NAME 3
#define SALES_AREA 4

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
    void tabCustSyncExp(QStandardItem *item);
    void appendCusts();
    void deleteCusts();
    void changeCusts();
    void updateCusts();
    void appendSales();
    void deleteSales();
    void changeSales();
    void updateSales();
    void recvNetJson(QJsonObject obj);
    void recvAppShow(QString win);
private:
    StandardItemModel *m_custs;
    StandardItemModel *m_sales;
    SqlQueryModel *sql_custs;
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
