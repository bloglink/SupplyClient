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

#include <QSplitter>

#include "delegate.h"

#include "OrderPage.h"

#define PROD_ID 0
#define PROD_GUID 1
#define PROD_SIGN 2
#define PROD_NUMB 3 //订单编号
#define PROD_DATE 4 //订单日期
#define PROD_VIEW 5 //评审编号
#define PROD_CUST 6 //客户名称
#define PROD_DEAD 7 //交货日期
#define PROD_QUAN 8 //在产数量
#define PROD_PNUM 9 //生产单号
#define PROD_TYPE 10//产品大类
#define PROD_CODE 11//产品编号
#define PROD_NAME 12//产品名称
#define PROD_MODE 13//产品规格
#define PROD_MNUM 14//仪表编号

class ProdsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProdsPage(QWidget *parent = 0);
    ~ProdsPage();

signals:
    void sendSocket(QUrl url);
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabProd();
    void autoNumber();
    void appendProd();
    void deleteProd();
    void changeProd();
    void updateProds();
    void updateOrder();
    void tabPlanSync(QModelIndex index);
    void tabProdSync(QModelIndex index);
    void matchProds();
    void matchCancel();
    void recvProdsJson(QJsonObject obj);
    void recvOrderJson(QJsonObject obj);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_prod;
    StandardSqlModel *sql_plan;
    StandardSqlModel *sql_prod;
    QTableView *tab_plan;
    QTableView *tab_prod;
    QTableView *tab_iprod;
    QWidget *prodWidget;
    QToolButton *btn_prods;
    QSqlDatabase db;
    ComboBoxDelegate *sale_delegate;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *cust_delegate;
    QStringList prod_items;
};

#endif // PRODSPAGE_H
