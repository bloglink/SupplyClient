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

//#define ORDER_ID 0
//#define ORDER_NUMB 1
//#define ORDER_DATE 2
//#define ORDER_VIEW 3
//#define ORDER_CUST 4
//#define ORDER_SALE 5
//#define ORDER_AREA 6
//#define ORDER_DEAD 7
//#define ORDER_QUAN 8
//#define ORDER_STCK 9
//#define ORDER_PROD 10
//#define ORDER_LACK 11
//#define ORDER_DNUM 12

#define PROD_ID 0
#define PROD_NUMB 1
#define PROD_DATE 2
#define PROD_VIEW 3
#define PROD_CUST 4
#define PROD_SALE 5
#define PROD_AREA 6
#define PROD_DEAD 7
#define PROD_NEED 8
#define PROD_QUAN 9
#define PROD_PNUM 10
#define PROD_TYPE 11
#define PROD_CODE 12
#define PROD_NAME 13
#define PROD_MODE 14

class ProdsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProdsPage(QWidget *parent = 0);
    ~ProdsPage();

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabProd();
    void autoNumber();
    void appendProd();
    void deleteProd();
    void changeProd();
    void updateProd();
    void tabPlanSync(QModelIndex index);
    void tabProdSync(QModelIndex index);
    void recvSocket(QUrl url);

    void showSnd();
    void refresh();
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
