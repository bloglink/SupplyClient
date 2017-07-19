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

#include "delegate.h"

#define CUSTOM_ID 0
#define CUSTOM_NAME 1
#define CUSTOM_SALE 2
#define CUSTOM_AREA 3

#define SALE_ID 0
#define SALE_NAME 1
#define SALE_AREA 2

class SalesPage : public QWidget
{
    Q_OBJECT
public:
    explicit SalesPage(QWidget *parent = 0);
    ~SalesPage();

signals:
    void sendSocket(QUrl url);
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
