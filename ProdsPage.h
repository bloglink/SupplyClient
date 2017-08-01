#ifndef PRODSPAGE_H
#define PRODSPAGE_H

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

#include <QSqlError>

#include <QSplitter>

#include "delegate.h"

#include "OrderPage.h"

#define PRODS_UUID 0x00 //生产ID
#define PRODS_GUID 0x01 //操作ID
#define PRODS_SIGN 0x02 //操作标识
#define PRODS_NUMB 0x03 //生产单号
#define PRODS_QUAN 0x04 //生产数量
#define PRODS_TYPE 0x05 //产品大类
#define PRODS_CODE 0x06 //产品编号
#define PRODS_NAME 0x07 //产品名称
#define PRODS_MODE 0x08 //产品规格
#define PRODS_MNUM 0x09 //仪表编号


class ProdsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProdsPage(QWidget *parent = 0);
    ~ProdsPage();

signals:
    void updateSql(QString sql);
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabProd();
    void autoNumber();
    void appendPlans();
    void deletePlans();
    void changePlans();
    void tabPlanSync(QModelIndex index);
    void tabProdSync(QModelIndex index);
    void matchProds();
    void matchCancel();
    void recvNetJson(QJsonObject obj);
    void recvAppShow(QString win);
    void updateOrder();
    void updateProds();
private:
    StandardItemModel *m_prod;
    SqlQueryModel *sql_order;
    SqlQueryModel *sql_prods;
    SqlQueryModel *sql_query;
    QTableView *tab_order;
    QTableView *tab_prods;
    QTableView *tab_iprod;
    QWidget *prodWidget;
    QToolButton *btn_prods;
    QSqlDatabase db;
    ComboBoxDelegate *sale_delegate;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *cust_delegate;
    QStringList prod_items;
    QStringList order_items;
};

#endif // PRODSPAGE_H
